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

template<>
int CResFileReader<File_Version_1_0>::Find(
    const char* pFileName )
{
    UHashValue FindKey = OCI::HashStringEx(pFileName);
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

}




// End of file