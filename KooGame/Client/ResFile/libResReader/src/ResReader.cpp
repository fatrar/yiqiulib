/**CPP*************************************************************************
 File            : ResReader.cpp
 Subsystem       : 
 Function Name(s): CResFileReader,CMemResReader
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 14:32
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
//#include "StdAfx.h"
#include "ResReader.h"
#include <string.h>
#include <stdio.h>
#include "Common.h"
#include "FileSystem.h"
#include "ResFileUtil.h"
#include "LzmaUtil.h"
//template ResFile::CResFileReader<ResFile::File_Version_1_0>;


namespace ResFile
{

template class CResFileReader<File_Version_1_0>;
//template class CResFileReader<File_Version_1_1>;

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
    TFileHeadBase HeadBase;
    FileSystem::CFile File1;
    ResCodeDef Code = Util::GetFileBaseHead(pResFilePath, HeadBase, File1);
    if ( Code != No_Error ||
         HeadBase.dwFileCount == 0)
    {
        return false;
    }

    switch (HeadBase.Version)
    {
    case File_Version_1_0:
        return true;
    case File_Version_1_1:
        break;
    default:
        return false;
    }

    typedef TFileHead<File_Version_1_0> FileHead0;
    typedef TFileHead<File_Version_1_1> FileHead1;
    typedef FileHead0::TDataIndex DataIndex0;
    typedef FileHead1::TDataIndex DataIndex1;

    FileHead1* pHead1 = (FileHead1*)new char[HeadBase.dwSize];
    memcpy(pHead1, &HeadBase, sizeof(HeadBase));
    char* pBuf = (char*)pHead1 + sizeof(TFileHeadBase);
    File1.Read(pBuf, HeadBase.dwSize-sizeof(TFileHeadBase));

    char szResBkPath[260] = {0};
    sprintf(szResBkPath, "%s%s", pResFilePath, ".bk");
    FileSystem::CFile File0;
    File0.OpenByWrite(szResBkPath);
    if ( !File0.IsOpen() )
    {
        delete[] (char*)pHead1;
        return false;
    }

    size_t nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
        File0, pHead1->dwRealFileCount,
        false, pHead1->eAlgo, pHead1->szKey);
    {
        // �����������ʱ����
        DWORD dwRawDataMem[TDataMemInfo::Max_Num] = {0};
        DWORD (&dwCompressDataMem)[TDataMemInfo::Max_Num] = dwRawDataMem;
        File0.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
        File0.Write((void*)dwCompressDataMem, sizeof(dwRawDataMem));
    }

    /**
    *@note ��Ԥ��һ��д�����Ŀռ�
    */
    const DWORD dwIndexCount0 = pHead1->dwRealFileCount;
    DataIndex0* pDataIndex0 = new DataIndex0[dwIndexCount0];
    size_t nIndexPos = 0;
    DWORD dwIndexOffset = dwIndexCount0*sizeof(DataIndex0);
    File0.Seek(dwIndexOffset, FileSystem::CFile::current);
    size_t nFilePosNow = nHeadSize;

    bool bRc = true;
    /**
    *@note ѭ����ÿ�����ѹ�������浽�µ��ļ���
    */
    size_t nFileBufSize = pHead1->dwMaxVolumeSize; 
    BYTE* pFileBuf = new BYTE[nFileBufSize];
    size_t nRawFileBufSize = pHead1->dwMaxVolumeSize;
    BYTE* pRawFileBuf = new BYTE[nFileBufSize];
    for (DWORD c = 0; c< pHead1->dwFileCount; ++c)
    {
        DataIndex1& Index1 = pHead1->DataIndex[c];

        /**
        *@note ���þ����ݴ��ļ��ж���
        */
        if ( nFileBufSize < Index1.dwLen )
        {
            delete[] pFileBuf;
            nFileBufSize = Index1.dwLen;
            pFileBuf = new BYTE[nFileBufSize];
        }

        File1.Seek(Index1.dwOffset);
        File1.Read(pFileBuf, Index1.dwLen);

        size_t nRawDataSize = Index1.dwRawVolumeDataLen;
        if ( nRawFileBufSize < nRawDataSize )
        {
            delete[] pRawFileBuf;
            nRawFileBufSize = nRawDataSize;
            pRawFileBuf = new BYTE[nRawFileBufSize];
        }

        /**
        *@note ������Ӧ�� ����[]�� ��ʱĬ��xor
        */
        //QWORD* pStart = (QWORD*)pFileBuf;
        //for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
        //{
        //    *pStart ^= pHead1->dwKey;
        //    ++pStart;
        //}

        /**
        *@note ��ѹ�þ�
        */
        int nRc = LzmaUtil::LzmaUncompress(
            pRawFileBuf, &nRawDataSize,
            pFileBuf, nFileBufSize,
            Compress_High);
            //pHead1->nCompressLevel);
        if ( nRc != 0 )
        {
            bRc = false;
            break;
        }

        /**
        *@note �����þ����ݣ�Ȼ��д�����ļ���������������
        */
        typedef TDataHead<File_Version_1_0> DataHead0;
        typedef TDataHead<File_Version_1_1> DataHead1;
        DataHead1* pDataHead1 = NULL;
        BYTE* pData1 = NULL;
        size_t nDataOffSetNow = 0;
        while (nDataOffSetNow != nRawDataSize)
        {
            pDataHead1 = (DataHead1*)(pRawFileBuf+nDataOffSetNow);
            pData1 = (BYTE*)(pDataHead1 + 1);
    
            /**
            *@note д����ͷ�����ݣ����ò���ѹ���ͼ���
            */
            DataHead0 TmpDataHead0;
            TmpDataHead0.dwRawDataLen = pDataHead1->dwRawDataLen;
            TmpDataHead0.nCompressAlgo = Raw_C_Algo;
            TmpDataHead0.nCompressLevel = Compress_High;
            TmpDataHead0.nIsDecrypt = 0;
            File0.Write(&TmpDataHead0, sizeof(DataHead0));
            File0.Write(pData1, pDataHead1->dwRawDataLen);

            /**
            *@note ��������Ϣ�Ž���������
            */
            DataIndex0& TmpDataIndex0 = pDataIndex0[nIndexPos++];
            TmpDataIndex0.HashValue = pDataHead1->HashValue;
            size_t nDataSize0 = pDataHead1->dwRawDataLen + sizeof(DataHead0);
            TmpDataIndex0.dwLen = nDataSize0;
            TmpDataIndex0.dwOffset = nFilePosNow;

            /**
            *@note �����ļ���ǰλ�ú�����λ��
            */
            nFilePosNow += nDataSize0;
            size_t nDataSize1 = pDataHead1->dwRawDataLen + sizeof(DataHead1);
            nDataOffSetNow += nDataSize1;
        }
    }
    File1.Close();

    delete[] (char*)pHead1;
    delete[] pFileBuf;
    delete[] pRawFileBuf;

    if ( !bRc )
    {
        delete[] pDataIndex0;
        return false;
    }

    /**
    *@note �����������ݣ�ʹ��ð������
    */
    for (int i=dwIndexCount0-1; i>0; --i)
    {
        for (int j=0; j<i; ++j)
        {
            if ( pDataIndex0[j] > pDataIndex0[j+1] )
            {
                DataIndex0 Tmp = pDataIndex0[j];
                pDataIndex0[j] = pDataIndex0[j+1];
                pDataIndex0[j+1] = Tmp;
            }
        }
    }  

    /**
    *@note ������д��Ԥ���������ļ�������
    */
    File0.Seek(nHeadSize-dwIndexOffset);
    File0.Write(pDataIndex0, dwIndexOffset);
    File0.Close();
    delete[] pDataIndex0;

    bRc = !!FileSystem::CFile::Remove(pResFilePath);
    if ( !bRc )
    {
        return false;
    }

    return !!FileSystem::CFile::Rename(
        szResBkPath, pResFilePath);
}

IResReader* CreateResFileReader( const char* pResFilePath )
{
    if ( !isValidString(pResFilePath) )
    {
        return NULL;
    }

    FileSystem::CFile* pResFile = new FileSystem::CFile();
    FileSystem::BOOL bRc = pResFile->OpenByRead(pResFilePath);
    if ( !bRc )
    {
        delete pResFile;
        return false;
    }

    TFileHeadBase HeadBase;
    FileSystem::size_t nRead = pResFile->Read(
        &HeadBase, sizeof(TFileHeadBase));
    if ( nRead != sizeof(TFileHeadBase) )
    {
        delete pResFile;
        return false;
    }

    if ( HeadBase.FormatFlag != Res_File_Format_Flag )
    {
        delete pResFile;
        return false;
    }

    switch (HeadBase.Version)
    {
    case File_Version_1_0:
        return new CResFileReader<File_Version_1_0>(pResFile, HeadBase);
    default:
        return NULL;
    }
}

IResReader* CreateMemResReader(
    BYTE* pData, size_t nSize,
    bool bRef, bool bDelete )
{
    if ( pData == NULL )
    {
        return NULL;
    }
    return new CMemResReader<File_Version_1_0>(pData, nSize, bRef, bDelete);
}

void DestroyResReader( IResReader*& pResReader )
{
    delete pResReader;
    pResReader = NULL;
}

/**
*@ { ******************** CResFileReader *****************
*/
template<>
CResFileReader<File_Version_1_0>::CResFileReader(
    FileSystem::CFile* pResFile, 
    const TFileHeadBase& HeadBase )
    : m_ResFile(pResFile)
{
    if ( HeadBase.dwFileCount == 0 ||
         HeadBase.dwSize != Util::GetFileHeadSize<File_Version_1_0>(HeadBase.dwFileCount) ) 
    {
        return;
    }

    char* pHead = new char[HeadBase.dwSize];
    memcpy(pHead, &HeadBase, sizeof(HeadBase));
    char* pBuf = pHead + sizeof(TFileHeadBase);
    m_ResFile->Read(pBuf, HeadBase.dwSize-sizeof(TFileHeadBase));
    m_pFileHead = (FileHead*)pHead;
}

template<>
CResFileReader<File_Version_1_0>::~CResFileReader(void)
{
    delete[] ((char*)m_pFileHead);
    m_pFileHead = NULL;
    safeDelete(m_ResFile);
}

template<>
void CResFileReader<File_Version_1_0>::GetDataHead(
    DWORD dwOffset, DataHead& Head)
{
    m_ResFile->Seek(dwOffset);
    m_ResFile->Read(&Head, sizeof(DataHead));
}

template<>
void CResFileReader<File_Version_1_0>::ReadData(
    DWORD dwOffset, void*& pBuf,
    size_t nRead, bool& bisRef)
{
    bisRef = FALSE;
    pBuf = GetFileReadBuf(nRead);
    m_ResFile->Seek(dwOffset);
    m_ResFile->Read(pBuf, nRead);
}
/** CResFileReader
*@ }
*/

/**
*@ { ******************** CMemResReader *****************
*/
template<>
CMemResReader<File_Version_1_0>::CMemResReader(
    BYTE* pData, size_t nSize, 
    bool bRef, bool bDelete )
    : m_nSize(nSize)
{
    if ( bRef )
    {
        m_pData = pData;
        m_bDelete = bDelete;
    }
    else
    {
        m_pData = new BYTE[nSize];
        memcpy(m_pData, pData, nSize);
        m_bDelete = true;
    }
    m_pFileHead = (FileHead*)m_pData;
}

template<>
CMemResReader<File_Version_1_0>::~CMemResReader()
{
    if ( m_bDelete )
    {
        delete[] m_pData;
    }
}

template<>
void CMemResReader<File_Version_1_0>::GetDataHead(
    DWORD dwOffset, DataHead& Head )
{
    BYTE* pRead = m_pData + dwOffset;
    memcpy(&Head, pRead, sizeof(DataHead));
}

template<>
void CMemResReader<File_Version_1_0>::ReadData(
    DWORD dwOffset, void*& pBuf,
    size_t nRead, bool& bisRef )
{
    pBuf = m_pData + dwOffset;
    bisRef = true;
}
/** CMemResReader
*@ }
*/

}




// End of file