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
#include "StdAfx.h"
#include "ResFileReader.h"
#include "ResFileDef.h"
#include <string.h>
namespace ResFile
{

template class CResFileReader<File_Version_1_0>;

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
     //   return new CResFileReader<File_Version_1_0>(pResFile, HeadBase);
    default:
        return NULL;
    }
}

template<>
CResFileReader<File_Version_1_0>::CResFileReader(
    FileSystem::CFile* pResFile, 
    const TFileHeadBase& HeadBase )
    : m_ResFile(pResFile)
    , m_dwHeadSize(HeadBase.dwSize)
    , m_dwFileCount(HeadBase.dwFileCount){}
 
template<>
CResFileReader<File_Version_1_0>::~CResFileReader(void)
{

}

template<>
bool CResFileReader<File_Version_1_0>::Parse()
{
    typedef TFileHead<File_Version_1_0> FileHead;
    if ( m_dwHeadSize < sizeof(FileHead) ||
         m_dwHeadSize >= Max_File_Head_1_0 ||
         m_dwFileCount == 0 ||
         m_dwHeadSize != Util::GetFileHeadSize<File_Version_1_0>(m_dwFileCount) ) 
    {
        return false;
    }
    return true;
}

template<>
ResFile::size_t CResFileReader<File_Version_1_0>::GetDataLen(
    const char* pFileName )
{
    return 0;
}

template<>
bool CResFileReader<File_Version_1_0>::GetData(
    const char* pFileName, 
    CUnPackDataInfo& UnPackDataInfo )
{
    return true;
}

template<>
void CResFileReader<File_Version_1_0>::Release(
    CUnPackDataInfo* pUnPackDataInfo )
{

}


}


// End of file