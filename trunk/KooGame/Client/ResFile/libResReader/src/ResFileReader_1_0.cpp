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

namespace ResFile
{

template<>
CResFileReader<File_Version_1_0>::CResFileReader(
    FileSystem::CFile* pResFile, 
    const TFileHeadBase& HeadBase )
    : m_ResFile(pResFile)
{
    memcpy(&m_FileHead, &HeadBase, sizeof(HeadBase));
    m_DecryptFn[Raw_E_Algo] = &CResFileReader<File_Version_1_0>::RawDecrypt;
    m_DecryptFn[Xor_E_Algo] = &CResFileReader<File_Version_1_0>::XorDecrypt;
    m_DecryptFn[BlowFish_E_Algo] = &CResFileReader<File_Version_1_0>::BlowFishDecrypt;

    m_UnPackFn[Raw_C_Algo] = &CResFileReader<File_Version_1_0>::RawUnPack;
    m_UnPackFn[LZMA_C_Algo] = &CResFileReader<File_Version_1_0>::LZMAUnPack;
    m_UnPackFn[Zip_C_Algo] = &CResFileReader<File_Version_1_0>::ZipUnPack;
}

template<>
CResFileReader<File_Version_1_0>::~CResFileReader(void)
{
    delete m_ResFile;
    m_ResFile = NULL;
}

template<>
bool CResFileReader<File_Version_1_0>::Parse()
{
    //typedef TFileHead<File_Version_1_0> FileHead;
    if ( m_FileHead.dwSize < sizeof(FileHead) ||
        m_FileHead.dwSize >= Max_File_Head_1_0 ||
        m_FileHead.dwFileCount == 0 ||
        m_FileHead.dwSize != Util::GetFileHeadSize<File_Version_1_0>(m_FileHead.dwFileCount) ) 
    {
        return false;
    }
    return true;
}

template<>
ResFile::size_t CResFileReader<File_Version_1_0>::GetDataLen(
    const char* pFileName )
{
    int nPos = Find(pFileName);
    if ( nPos == Invaild_Pos )
    {
        return 0;
    }

    const FileHead::TDataIndex* DataIndex = &m_FileHead.DataIndex[0];
    DWORD dwDataPos = DataIndex[nPos].DataHead.dwDataPos;

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
    int nPos = Find(pFileName);
    if ( nPos == Invaild_Pos )
    {
        return 0;
    }

    const FileHead::TDataIndex* DataIndex = &m_FileHead.DataIndex[0];
    DWORD dwDataPos = DataIndex[nPos].DataHead.dwDataPos;
    DWORD dwDataLen = DataIndex[nPos].DataHead.dwDataLen;

    m_ResFile->Seek(dwDataPos);

    void* pBuf = GetFileReadBuf(dwDataLen);
    m_ResFile->Read(pBuf, dwDataLen);

    DataHead* pTmpDataHead = (DataHead*)pBuf;
    void* pEncryptBuf = (void*)(pTmpDataHead + 1);
    assert(pTmpDataHead->nEncryptAlgo >= Encrypt_Count);
    DecryptFn pDecryptFn = m_DecryptFn[pTmpDataHead->nEncryptAlgo];
    (this->*pDecryptFn)(
        pEncryptBuf, pTmpDataHead->nDataEncryptLen,
        (void*)pTmpDataHead->cEncryptParam );

    UnPackFn pUnPackFn = m_UnPackFn[pTmpDataHead->nCompressAlgo];
    GetUnPackBuf(pTmpDataHead->dwRawDataLen, &UnPackDataInfo);
    (this->*pUnPackFn)(
        pEncryptBuf, dwDataLen-sizeof(DataHead),
        (void*)UnPackDataInfo.Ptr(), UnPackDataInfo.Length() );
    return true;
}

template<>
void CResFileReader<File_Version_1_0>::Release(
    CUnPackDataInfo* pUnPackDataInfo )
{
    ResMemType t = GetMemType(pUnPackDataInfo);
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