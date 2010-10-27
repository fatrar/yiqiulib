/*H***************************************************************************
File            : ResPatchGenerator.h
Subsystem       : 
Function Name(s): CResPatchGenerator

Description     : 
Author          : Yiqiu
Date            : 2010/10/20
Revision        : 

History
-------


Copyright (c) Shenzhen KooGame Co.,Ltd.
***************************************************************************H*/
#ifndef _RESPATCHGENERATOR_H_2010_10
#define _RESPATCHGENERATOR_H_2010_10


namespace ResFile
{


namespace ResPatchGenerator
{
void Generate(
    const char* pOld,
    const char* pNew, 
    const char* pPatchPath);
};

}



#endif