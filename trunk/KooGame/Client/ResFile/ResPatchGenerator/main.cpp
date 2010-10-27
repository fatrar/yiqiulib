/*CPP*************************************************************************
File            : main.cpp
Subsystem       : 
Function Name(s): 

Description     : 
Author          : Yiqiu
Date            : 2010/10/20
Revision        : 

History
-------


Copyright (c) Shenzhen Sunline Tech Co.,Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "ResPatchGenerator.h"

#define _Command_Help_Note "\nUse command like: \"ResPatchGenerator.exe ?\" can Get help!"
#define _Command_Help "xx is your set value. '[]' is not must set.\n"\
    "'OldFile=xx' xx is a Path.\n"\
    "'NewFile=xx' xx is a Path.\n"\
    "'PatchFile=xx'  xx is a Path.\n"\


int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc == 2 && argv[1][0] == '?' )
    {
        cout << _Command_Help << endl;
        return 0;
    }

    using namespace ICommand;
    ICmdParser2* pCmdParser2 = CreateCmdParser2();
    for ( int i = 1; i< argc; ++i )
    {
        pCmdParser2->AddCmdString(argv[i]);
        cout << "Add Command :" << argv[i] << endl;
    }

    try
    {
        pCmdParser2->Parse();
        char* pOldFile=NULL, *pNewFile=NULL, *pPatchFile=NULL;
        if ( !pCmdParser2->GetValue("OldFile", T_String, (void*&)pOldFile)) 
        {
            throw "Must Set Old File";
        }

        if ( !pCmdParser2->GetValue("NewFile", T_String, (void*&)pNewFile)) 
        {
            throw "Must Set New File";
        }

        if ( !pCmdParser2->GetValue("PatchFile", T_String, (void*&)pPatchFile)) 
        {
            throw "Must Set Patch File";
        }

        ResFile::ResPatchGenerator::Generate(pOldFile,pNewFile,pPatchFile);
    }
    catch (const char* pErrInfo)
    {
        cout << pErrInfo << _Command_Help_Note << endl;
    }

    pCmdParser2->Release();
    system("pause");
    return 0;
}