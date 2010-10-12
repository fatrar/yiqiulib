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
#include <string.h>
#include <assert.h>
//template ResFile::CResFileReader<ResFile::File_Version_1_0>;

namespace ResFile
{

template class CResFileReader<File_Version_1_0>;
//template class CResFileReader<File_Version_2_0>;

IResReader* CreateResFileReader( const char* pResFilePath )
{
    if ( NULL == pResFilePath ||
         0 == pResFilePath[0] )
    {
        return NULL;
    }

    FileSystem::CFile* pResFile = new FileSystem::CFile();
    FileSystem::BOOL bRc = pResFile->OpenByRead(pResFilePath);
    if ( !bRc )
    {
        return false;
    }

    TFileHeadBase HeadBase;
    FileSystem::size_t nRead = pResFile->Read(
        &HeadBase, sizeof(TFileHeadBase));
    if ( nRead != sizeof(TFileHeadBase) )
    {
        return false;
    }

    if ( HeadBase.FormatFlag != File_Format_Flag )
    {
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
    ResMemType& t = GetMemType(pUnPackDataInfo);
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
    t = Reader_Allocate;
    return new char[nBufNeed];
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
    void* pIn, size_t nInLen,
    void* pEncryptParam )
{

}


template<DWORD Version>
void CResFileReader<Version>::LZMAUnPack(
    void* pIn, size_t nInLen, 
    void* pOut, size_t nOut )
{

}


template<DWORD Version>
void CResFileReader<Version>::ZipUnPack(
    void* pIn, size_t nInLen,
    void* pOut, size_t nOut )
{

}

template<>
int CResFileReader<File_Version_1_0>::Find(
    const char* pFileName )
{
    UHashValue FindKey = Util::HashString(pFileName);
    const FileHead::TDataIndex* DataIndex = &m_FileHead.DataIndex[0];

    // 二分法查找
    size_t nLow = 0, nHigh = m_FileHead.dwFileCount-1;
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