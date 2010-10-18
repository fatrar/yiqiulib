/**CPP*************************************************************************
 File            : ResFileReader_1_0.cpp
 Subsystem       : 
 Function Name(s): CResFileReader<File_Version_1_0>
 Author          : YiQiu
 Date            : 2010-10-11  
 Time            : 16:45
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
//#include "StdAfx.h"
#include "ResFileReader.h"
#include "ResFileDef.h"
#include <string.h>
#include <assert.h>
#include "Common.h"

namespace ResFile
{

template class CResFileReader<File_Version_1_0>;

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

    m_DecryptFn[Raw_E_Algo] = &CResFileReader<File_Version_1_0>::RawDecrypt;
    m_DecryptFn[Xor_E_Algo] = &CResFileReader<File_Version_1_0>::XorDecrypt;
    m_DecryptFn[BlowFish_E_Algo] = &CResFileReader<File_Version_1_0>::BlowFishDecrypt;

    m_UnPackFn[Raw_C_Algo] = &CResFileReader<File_Version_1_0>::RawUnPack;
    m_UnPackFn[Zip_C_Algo] = &CResFileReader<File_Version_1_0>::ZipUnPack;
    m_UnPackFn[Lzma_C_Algo] = &CResFileReader<File_Version_1_0>::LzmaUnPack;
}

template<>
CResFileReader<File_Version_1_0>::~CResFileReader(void)
{
    delete[] ((char*)m_pFileHead);
    m_pFileHead = NULL;
    safeDelete(m_ResFile);
}

// template<>
// bool CResFileReader<File_Version_1_0>::Parse()
// {
//     //typedef TFileHead<File_Version_1_0> FileHead;
//     if ( m_FileHead.dwFileCount == 0 ||
//         m_FileHead.dwSize != Util::GetFileHeadSize<File_Version_1_0>(m_FileHead.dwFileCount) ) 
//     {
//         return false;
//     }
// 
//     size_t nHeadBase = sizeof(TFileHeadBase);
//     char* pStart = (char*)&m_FileHead;
//     char* pBuf = pStart + sizeof(TFileHeadBase);
//     m_ResFile->Read(pBuf, m_FileHead.dwSize-sizeof(TFileHeadBase));
//     return true;
// }

template<>
ResFile::size_t CResFileReader<File_Version_1_0>::GetDataLen(
    const char* pFileName )
{
    int nPos = Find(pFileName);
    if ( nPos == Invaild_Pos )
    {
        return 0;
    }

    const FileHead::TDataIndex* DataIndex = &m_pFileHead->DataIndex[0];
    DWORD dwDataPos = DataIndex[nPos].Info.dwDataOffset;

    m_ResFile->Seek(dwDataPos);
    DataHead TmpDataHead;
    m_ResFile->Read(&TmpDataHead, sizeof(DataHead)); 
    return TmpDataHead.dwRawDataLen;
}

template<>
bool CResFileReader<File_Version_1_0>::GetData(
    const char* pFileName, 
    CUnPackDataInfo& UnPackDataInfo )
{
    /**
    *@note 二分法在索引中查找
    */
    int nPos = Find(pFileName);
    if ( nPos == Invaild_Pos )
    {
        return 0;
    }

    /**
    *@note 取文件的位置及数据大小
    */
    const FileHead::TDataIndex* DataIndex = &m_pFileHead->DataIndex[0];
    DWORD dwDataOffset = DataIndex[nPos].Info.dwDataOffset;
    DWORD dwDataLen = DataIndex[nPos].Info.dwDataLen;

    /**
    *@note 移动文件指针并读取数据（包括文件头及文件数据）
    */
    m_ResFile->Seek(dwDataOffset);
    void* pBuf = GetFileReadBuf(dwDataLen);
    m_ResFile->Read(pBuf, dwDataLen);

    /**
    *@note 解密数据,解密的数据我是放在头后面
    */
    DataHead* pTmpDataHead = (DataHead*)pBuf;
    void* pEncryptBuf = (void*)(pTmpDataHead + 1);
    assert(pTmpDataHead->nEncryptAlgo < Encrypt_Count);
    DecryptFn pDecryptFn = m_DecryptFn[pTmpDataHead->nEncryptAlgo];
    (this->*pDecryptFn)(
        pEncryptBuf, pTmpDataHead->nDataEncryptLen,
        (void*)&pTmpDataHead->eParam );

    /**
    *@note 解压数据
    */
    UnPackFn pUnPackFn = m_UnPackFn[pTmpDataHead->nCompressAlgo];
    GetUnPackBuf(pTmpDataHead->dwRawDataLen, &UnPackDataInfo);
    size_t nUnpackSize = UnPackDataInfo.Length();
    (this->*pUnPackFn)(
        pEncryptBuf, dwDataLen-sizeof(DataHead),
        (void*)UnPackDataInfo.Ptr(), nUnpackSize,
        pTmpDataHead->nCompressLevel );
    UnPackDataInfo.m_pResReader = this;
    return true;
}

template<>
void CResFileReader<File_Version_1_0>::Release(
    CUnPackDataInfo* pUnPackDataInfo )
{
    ResMemType t = (ResMemType)pUnPackDataInfo->m_nMemType;
    switch (t)
    {
    case Mem_Unknown:
    case User_Allocate:
        break;
    case Reader_Static_Buf:
    case Reader_Allocate:
        delete[] (char*)pUnPackDataInfo->Ptr();
    default:
        break;
    }
}



}





// End of file