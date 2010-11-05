/**CPP*************************************************************************
 File            : ResReaderBase.cpp
 Subsystem       : 
 Function Name(s): CResReaderBase
 Author          : YiQiu
 Date            : 2010-10-19  
 Time            : 11:19
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "ResReaderBase.h"
#include "Common.h"
#include "HashHelper.h"
#include <assert.h>
#include "LzmaUtil.h"

#ifdef _USE_IRESREADEREX_
#include <process.h>
#include <Windows.h>
#endif


namespace ResFile
{

template class CResReaderBase<File_Version_1_0>;

template<DWORD Version>
int CResReaderBase<Version>::GetDataIndex(
    const char* pFileName,
    QWORD& qwHashValue )
{
    qwHashValue = OCI::HashStringEx(pFileName);
    return Find(qwHashValue);
}

template<DWORD Version>
void CResReaderBase<Version>::LzmaUnPack(
    void* pIn, size_t nIn,
    void* pOut, size_t& nOut, size_t nLevel)
{
    LzmaUtil::LzmaUncompress(
        (BYTE*)pOut, &nOut, (const BYTE*)pIn, nIn, nLevel);
}

template<>
ResFile::size_t CResReaderBase<File_Version_1_0>::GetDataLen(
    const char* pFileName,
    size_t& nPos )
{
    nPos = Find(pFileName);
    if ( nPos == Invaild_Pos )
    {
        return 0;
    }

    const FileHead::TDataIndex& DataIndex = m_pFileHead->DataIndex[nPos];
    DWORD dwDataPos = DataIndex.dwOffset;

    DataHead Head;
    GetDataHead(dwDataPos, Head); 
    return Head.dwRawDataLen;
}

template<>
bool CResReaderBase<File_Version_1_0>::GetData(
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
bool CResReaderBase<File_Version_1_0>::GetData(
    size_t nPos,
    CUnPackDataInfo& UnPackDataInfo )
{
    /**
    *@note 取文件的位置及数据大小
    */
    const FileHead::TDataIndex& DataIndex = m_pFileHead->DataIndex[nPos];
    DWORD dwDataOffset = DataIndex.dwOffset;
    DWORD dwDataLen = DataIndex.dwLen;

    /**
    *@note 移动文件指针并读取数据（包括文件头及文件数据）
    */
    void* pBuf = NULL;
    bool bisRef;
    ReadData(dwDataOffset, pBuf, dwDataLen, bisRef);

    /**
    *@note 解密数据,解密的数据我是放在头后面
    */
    DataHead* pTmpDataHead = (DataHead*)pBuf;
    void* pEncryptBuf = (void*)(pTmpDataHead + 1);
    if ( 0 == pTmpDataHead->nIsDecrypt )
    {
        DecryptFn pDecryptFn = m_DecryptFn[m_pFileHead->eAlgo];
        (this->*pDecryptFn)(
            pEncryptBuf, dwDataLen-sizeof(DataHead) );
        if ( bisRef )
        {
            // 这个地方有问题，当数据被解密，会被再解密
            pTmpDataHead->nIsDecrypt = 1;
        }
    } 

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

    ReleaseReadBuf(pBuf);
    return true;
}

template<>
int CResReaderBase<File_Version_1_0>::Find(
    const char* pFileName )
{
    QWORD FindKey = OCI::HashStringEx(pFileName);
    return Find(FindKey);
}

template<>
int CResReaderBase<File_Version_1_0>::Find(
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

template<DWORD Version>
void* CResReaderBase<Version>::GetUnPackBuf(
    size_t nBufNeed,
    CUnPackDataInfo* pUnPackDataInfo)
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
void CResReaderBase<Version>::Release(
    CUnPackDataInfo* pUnPackDataInfo)
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
void ResFile::CResReaderBase<Version>::XorDecrypt(
    void* pIn, size_t nIn )
{
    assert(nIn == 32);
    QWORD pKey = m_pFileHead->dwKey;
    QWORD* pStart = (QWORD*)pIn;  
    for ( int i = 0 ; i < 4; ++i )  // 4 = 32 / sizeof(QWORD)
    {
        *pStart ^= pKey;
        ++pStart;
    }
}

#ifdef _USE_IRESREADEREX_
template<>
size_t CResReaderBase<File_Version_1_0>::Count()
{
    return m_pFileHead->dwFileCount;
}

template<>
bool CResReaderBase<File_Version_1_0>::GetSomeInfo(
    size_t i, CResIterator& iter )
{
    if ( i >= m_pFileHead->dwFileCount )
    {
        return false;
    }

    const FileHead::TDataIndex& DataIndex = 
        m_pFileHead->DataIndex[i];
    iter.m_nIndex = i;
    iter.m_HashValue = DataIndex.HashValue;
    iter.m_dwPackDataLen = DataIndex.Info.dwDataLen;
    iter.m_dwDataOffset = DataIndex.Info.dwDataOffset;
    return true;
}

template<>
bool CResReaderBase<File_Version_1_0>::StartGetAllInfo(
    IInfoReadCallback* p)
{
    m_pInfoReadCallback = p;
    m_bEndThread = false;
    m_hThead = _beginthreadex(
        NULL, 0 , ReadInfoThread, this, 0, NULL);
    return 0;
}

template<>
bool CResReaderBase<File_Version_1_0>::EndGetAllInfo()
{
    m_bEndThread = true;
    WaitForSingleObject((HANDLE)m_hThead, INFINITE);
    CloseHandle((HANDLE)m_hThead);
    return true;
}

template<>
unsigned int CResReaderBase<File_Version_1_0>::ReadInfoThread()
{
    DataHead Head;
    for ( size_t i = 0; 
          i < m_pFileHead->dwFileCount && !m_bEndThread;
          ++i )
    {
        const FileHead::TDataIndex& DataIndex = m_pFileHead->DataIndex[i];
        DWORD dwDataPos = DataIndex.Info.dwDataOffset;
        GetDataHead(dwDataPos, Head); 
        m_pInfoReadCallback->OnInfoRead(
            i, 
            (eEncryptAlgo)Head.nEncryptAlgo,
            (eCompressAlgo)Head.nCompressAlgo,
            (eCompressParam)Head.nCompressLevel,
            Head.dwRawDataLen );
    }
    return 0;
}

#endif

}



// End of file