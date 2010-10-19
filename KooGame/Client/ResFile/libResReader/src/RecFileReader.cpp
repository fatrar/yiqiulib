/**CPP*************************************************************************
 File            : ResFileReader.cpp
 Subsystem       : 
 Function Name(s): CResFileReader
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
#include "ResFileReader.h"
#include "HashHelper.h"
#include <string.h>
#include <assert.h>
#include "LzmaUtil.h"
#include "Common.h"
//template ResFile::CResFileReader<ResFile::File_Version_1_0>;

namespace ResFile
{

template class CResFileReader<File_Version_1_0>;
//template class CResFileReader<File_Version_2_0>;

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

    if ( HeadBase.FormatFlag != File_Format_Flag )
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

void DestroyResReader( IResReader*& pResReader )
{
    delete pResReader;
    pResReader = NULL;
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

template<DWORD Version>
void* CResFileReader<Version>::GetUnPackBuf(
    size_t nBufNeed,
    CUnPackDataInfo* pUnPackDataInfo )
{
    ResMemType t = (ResMemType)pUnPackDataInfo->m_nMemType;
    switch (t)
    {
    case User_Allocate:
    	return (void*)pUnPackDataInfo->Ptr();
    case Reader_Static_Buf:
    	break;
    case Reader_Allocate:
    default:
    	break;
    }
    pUnPackDataInfo->m_nLen = nBufNeed;
    pUnPackDataInfo->m_nMemType = Reader_Allocate;
    return  pUnPackDataInfo->m_pData = new BYTE[nBufNeed];
}


template<DWORD Version>
void* CResFileReader<Version>::GetFileReadBuf( 
    size_t nBufNeed )
{
    return new char[nBufNeed];
}

template<DWORD Version>
void CResFileReader<Version>::XorDecrypt(
    void* pIn, size_t nInLen,
    void* pEncryptParam )
{

}

template<DWORD Version>
void CResFileReader<Version>::BlowFishDecrypt(
    void* pIn, size_t nIn,
    void* pEncryptParam )
{

}



template<DWORD Version>
void CResFileReader<Version>::LzmaUnPack(
    void* pIn, size_t nIn, 
    void* pOut, size_t& nOut, size_t nLevel )
{
    LzmaUtil::LzmaUncompress(
        (BYTE*)pOut, &nOut, (const BYTE*)pIn, nIn, nLevel);
}


template<DWORD Version>
void CResFileReader<Version>::ZipUnPack(
    void* pIn, size_t nIn,
    void* pOut, size_t& nOut, size_t nLevel )
{

}

/**
*@ { ******************** File_Version_1_0 *****************
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

template<>
ResFile::size_t CResFileReader<File_Version_1_0>::GetDataLen(
    const char* pFileName,
    size_t& nPos )
{
    nPos = Find(pFileName);
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
    
    return GetData(nPos, UnPackDataInfo);
}

template<>
virtual int CResFileReader<File_Version_1_0>::GetDataIndex(
    const char* pFileName,
    QWORD& qwHashValue )
{
    qwHashValue = OCI::HashStringEx(pFileName);
    return Find(qwHashValue);
}

template<>
virtual bool CResFileReader<File_Version_1_0>::GetData(
    size_t nPos,
    CUnPackDataInfo& UnPackDataInfo )
{
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
int CResFileReader<File_Version_1_0>::Find(
    const char* pFileName )
{
    QWORD FindKey = OCI::HashStringEx(pFileName);
    return Find(FindKey);
}

template<>
int CResFileReader<File_Version_1_0>::Find(
    const QWORD& qwHashValue )
{
    UHashValue FindKey = qwHashValue;
    const FileHead::TDataIndex* DataIndex = &m_pFileHead->DataIndex[0];

    // 二分法查找
    size_t nLow = 0, nHigh = m_pFileHead->dwFileCount-1;
    size_t nMid;
    while (nLow <= nHigh)
    {
        nMid = (nLow + nHigh)>>1;// /2;
        const UHashValue& TempKey = DataIndex[nMid].HashValue;
        if ( TempKey == FindKey )
        {
            return nMid;
        }
        else if ( TempKey > FindKey )
        {
            nHigh = nMid - 1;
        }
        else
        {
            nLow = nMid + 1;
        }
    }
    return -1;
}
/** File_Version_1_0
*@ }
*/
}




// End of file