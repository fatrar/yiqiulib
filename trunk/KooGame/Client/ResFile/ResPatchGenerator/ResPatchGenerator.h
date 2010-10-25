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


class CResPatchGenerator
{
public:
    CResPatchGenerator(
        const char* pOld,
        const char* pNew );
    ~CResPatchGenerator(void);

public:
    bool Generate(const char* pPatchPath);

protected:
     GetDataIndex();

protected:
    string m_strOldPath;
    string m_strNewPath;

    //ifstream m_Reader;
};

}



#endif