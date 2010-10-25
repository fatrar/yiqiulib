/*CPP*************************************************************************
File            : ResPatchGenerator.cpp
Subsystem       : 
Function Name(s): CResPatchGenerator

Description     : 
Author          : Yiqiu
Date            : 2010/10/20
Revision        : 

History
-------


Copyright (c) Shenzhen KooGame Tech Co.,Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "ResPatchGenerator.h"


namespace ResFile
{


CResPatchGenerator::CResPatchGenerator(void)
{
}

CResPatchGenerator::~CResPatchGenerator(void)
{
}

bool CResPatchGenerator::Generate(
    const char* pPatchPath )
{
    if ( !isValidString(pPatchPath) )
    {
        return false;
    }

    FileSystem::CFile OldFile, NewFile;
    if ( !OldFile.OpenByRead(m_strOldPath.c_str()) ||
         !NewFile.OpenByRead(m_strNewPath.c_str()) )
    {
        return false;
    }

    typedef TFileHead<File_Version_1_0> FileHead;

    TFileHeadBase HeadBase;
    FileSystem::size_t nRead = OldFile.Read(
        &HeadBase, sizeof(TFileHeadBase));
    if ( nRead != sizeof(TFileHeadBase) )
    {
        return false;
    }

}

}