/**CPP*************************************************************************
 File            : ResUnpack.cpp
 Subsystem       : 
 Function Name(s): CResUnpack
 Author          : YiQiu
 Date            : 2010-11-11  
 Time            : 15:02
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "ResReader.h"
#include <stdio.h>
#include "Common.h"
#include "FileSystem.h"
#include "ResFileUtil.h"
#include "LzmaUtil.h"

namespace ResFile
{

class CResUnpack :
    public Util::CUnpackVolumeUtil
{
public:
    inline CResUnpack();
    inline ~CResUnpack();
    void CheckSrcFile(const char* pResFilePath);
    inline void InitTmpFile(char* pTmpFilePath);
    void UnPackData();
    void SortIndex();
    void WriteIndex();

    // CUnpackVolumeUtil
protected:
    virtual void DataReadCallBack(
        DataHead1* pHead, BYTE* pData);

    virtual DWORD Read(
        DWORD dwOffset, BYTE* pBuf, DWORD dwLen);

protected:
    inline void CreateTmpFile(char* pTmpFilePath);
    inline void WriteBaseHead();
    inline void ReserveIndexSpace();

private:
    TFileHeadBase m_HeadBase1;
    FileSystem::CFile m_File0, m_File1;
    FileHead1* m_pHead1;
    DataIndex0* m_pDataIndex0;
    DWORD m_dwIndexCount0;
    size_t m_nIndexPos;
    size_t m_nFilePosNow;
};

int IsNeedUnpack(const char* pResFilePath)
{
    TFileHeadBase HeadBase;
    ResCodeDef Code = Util::GetFileBaseHead(pResFilePath, HeadBase);
    if ( Code != No_Error )
    {
        return Code;
    }
    
    switch (HeadBase.Version)
    {
    case File_Version_1_0:
        return Is_Unpacked_File;
    case File_Version_1_1:
        return Is_Packed_File;
    default:
        return File_Format_Error;
    }
}

bool UnpackFile(const char* pResFilePath)
{
    try
    {
        CResUnpack ResUnpack;
        ResUnpack.CheckSrcFile(pResFilePath);

        char szResBkPath[260] = {0};
        sprintf(szResBkPath, "%s%s", pResFilePath, ".bk");
        ResUnpack.InitTmpFile(szResBkPath);

        ResUnpack.UnPackData();
        ResUnpack.SortIndex();
        ResUnpack.WriteIndex();

        bool bRc = !!FileSystem::CFile::Remove(pResFilePath);
        if ( !bRc )
        {
            return false;
        }

        return !!FileSystem::CFile::Rename(
            szResBkPath, pResFilePath);
    }
    catch (ResCodeDef e)
    {
        if ( e == Is_Unpacked_File )
        {
            return true;
        }

        return false;
    }
}

CResUnpack::CResUnpack()
    : m_pHead1(NULL)
    , m_pDataIndex0(NULL)
    , m_nIndexPos(0){}

CResUnpack::~CResUnpack()
{
    Util::DestroyFileHead(m_pHead1);
    safeDeleteArray(m_pDataIndex0);
}

void CResUnpack::CheckSrcFile(const char* pResFilePath)
{
    ResCodeDef Code = Util::GetFileBaseHead(
        pResFilePath, m_HeadBase1, m_File1);
    if ( Code != No_Error ||
         m_HeadBase1.dwFileCount == 0)
    {
        throw Code;
    }

    switch (m_HeadBase1.Version)
    {
    case File_Version_1_0:
        throw Is_Unpacked_File;
    case File_Version_1_1:
        break;
    default:
        throw File_Format_Error;
    }

    m_pHead1 = (FileHead1*)new char[m_HeadBase1.dwSize];
    memcpy(m_pHead1, &m_HeadBase1, sizeof(m_HeadBase1));
    char* pBuf = (char*)m_pHead1 + sizeof(TFileHeadBase);
    m_File1.Read(pBuf, m_HeadBase1.dwSize-sizeof(TFileHeadBase));
}

void CResUnpack::InitTmpFile(char* pTmpFilePath)
{
    CreateTmpFile(pTmpFilePath);
    WriteBaseHead();
    ReserveIndexSpace();
}

void CResUnpack::CreateTmpFile(char* pTmpFilePath)
{ 
    m_File0.OpenByWrite(pTmpFilePath);
    if ( !m_File0.IsOpen() )
    {
        delete[] (char*)m_pHead1;
        throw File_Can_Not_Open;
    }
}

void CResUnpack::WriteBaseHead()
{
    size_t nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
        m_File0, m_pHead1->dwRealFileCount,
        false, m_pHead1->eAlgo, m_pHead1->szKey);
    {
        // 这个部分我暂时不用
        DWORD dwRawDataMem[TDataMemInfo::Max_Num] = {0};
        DWORD (&dwCompressDataMem)[TDataMemInfo::Max_Num] = dwRawDataMem;
        m_File0.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
        m_File0.Write((void*)dwCompressDataMem, sizeof(dwRawDataMem));
    }
    m_nFilePosNow = nHeadSize;
}

void CResUnpack::ReserveIndexSpace()
{
    /**
    *@note 先预留一个写索引的空间
    */
    m_dwIndexCount0 = m_pHead1->dwRealFileCount;
    m_pDataIndex0 = new DataIndex0[m_dwIndexCount0];
    DWORD dwIndexOffset = m_dwIndexCount0*sizeof(DataIndex0);
    m_File0.Seek(dwIndexOffset, FileSystem::CFile::current);
}

void CResUnpack::UnPackData()
{
    CUnpackVolumeUtil::Unpack(m_pHead1);
    //    m_File1, OnDataReadCallBack, this);
    m_File1.Close();
}

DWORD CResUnpack::Read(
    DWORD dwOffset, BYTE* pBuf, DWORD dwLen)
{
    m_File1.Seek(dwOffset);
    return m_File1.Read(pBuf, dwLen);
}

void CResUnpack::DataReadCallBack(
    DataHead1* pHead,
    BYTE* pData )
{
    /**
    *@note 写数据头和数据，先用不带压缩和加密
    */
    DataHead0 TmpDataHead0;
    TmpDataHead0.dwRawDataLen = pHead->dwRawDataLen;
    TmpDataHead0.nCompressAlgo = Raw_C_Algo;
    TmpDataHead0.nCompressLevel = Compress_Normal;
    TmpDataHead0.nIsDecrypt = 0;
    m_File0.Write(&TmpDataHead0, sizeof(DataHead0));
    m_File0.Write(pData, pHead->dwRawDataLen);

    /**
    *@note 将数据信息放进索引队列
    */
    DataIndex0& TmpDataIndex0 = m_pDataIndex0[m_nIndexPos++];
    TmpDataIndex0.HashValue = pHead->HashValue;
    size_t nDataSize0 = pHead->dwRawDataLen + sizeof(DataHead0);
    TmpDataIndex0.dwLen = nDataSize0;
    TmpDataIndex0.dwOffset = m_nFilePosNow;

    /**
    *@note 更新文件当前位置
    */
    m_nFilePosNow += nDataSize0;
}

void CResUnpack::SortIndex()
{
    /**
    *@note 排序索引数据，使用冒泡排序
    */
    for (int i=m_dwIndexCount0-1; i>0; --i)
    {
        for (int j=0; j<i; ++j)
        {
            if ( m_pDataIndex0[j] > m_pDataIndex0[j+1] )
            {
                DataIndex0 Tmp = m_pDataIndex0[j];
                m_pDataIndex0[j] = m_pDataIndex0[j+1];
                m_pDataIndex0[j+1] = Tmp;
            }
        }
    }  
}

void CResUnpack::WriteIndex()
{
    /**
    *@note 将索引写进预留的索引文件数据区
    */
    DWORD dwIndexSize = m_dwIndexCount0*sizeof(DataIndex0);;
    m_File0.Seek(m_pHead1->dwSize-dwIndexSize);
    m_File0.Write(m_pDataIndex0, dwIndexSize);
    m_File0.Close();
    delete[] m_pDataIndex0; m_pDataIndex0 = NULL;
    delete[] (char*)m_pHead1; m_pHead1 = NULL;
}

} // namespace ResFile


// End of file