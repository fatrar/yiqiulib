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
#include "Common.h"
#include "FileSystem.h"
#include "ResFileUtil.h"
//template ResFile::CResFileReader<ResFile::File_Version_1_0>;

namespace ResFile
{

template class CResFileReader<File_Version_1_0>;
template class CResFileReader<File_Version_1_1>;

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
    FileSystem::CFile File;
    ResCodeDef Code = Util::GetFileBaseHead(pResFilePath, HeadBase, File);
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

    FileHead1* pHead1 = (FileHead1*)new char[HeadBase.dwSize];
    memcpy(pHead1, &HeadBase, sizeof(HeadBase));
    char* pBuf = (char*)pHead1 + sizeof(TFileHeadBase);
    File.Read(pBuf, HeadBase.dwSize-sizeof(TFileHeadBase));

    return true;
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