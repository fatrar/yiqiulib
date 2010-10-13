/**CPP*************************************************************************
 File            : ResPacker.cpp
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-13  
 Time            : 10:47
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "..\inc\ResPacker.h"


namespace ResFile
{

CResPacker::CResPacker(void)
{
}

CResPacker::~CResPacker(void)
{
}

void CResPacker::SetDefaultEncryptParam(
    EncryptAlgo eAlgo,
    void* peParam /*= NULL */ )
{

}

void CResPacker::SetDefaultCompressParam(
    CompressAlgo cAlgo, 
    void* pcParam /*= NULL*/ )
{

}

void CResPacker::SetCurrentPath(
    const char* pPath )
{

}

void CResPacker::AddFile(
    const char* pFileName )
{

}

void CResPacker::AddFile(
    const char* pFileName,
    CompressAlgo cAlgo, void* pcParam,
    EncryptAlgo eAlgo, void* peParam )
{

}

bool CResPacker::MakeFile(
    const char* pPackFilePath,
    FileNamePos eFileNamePos )
{

}

}



//namespace GG
//{
//    typedef unsigned int size_t; 
//
//    typedef unsigned char size_t; 
//}
//
//class JJ
//{
//
//};






// End of file