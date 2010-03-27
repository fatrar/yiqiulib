/*CPP*************************************************************************
File            : MemCheck.cpp
Subsystem       : 
Function Name(s): 

Description     : 
Author          : Yiqiu
Date            : 2009/3/26
Revision        : 

History
-------


Copyright (c) DOWSHU Electronica (China) Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "MemCheck.h"
#include "Trace.h"
#pragma init_seg(lib)


#if defined(MEMORY_CHECK) && defined(_DEBUG)

char g_szProjectName[MAX_PATH] = {0};

ds::ds()
{
    HMODULE hModule = NULL;
    BOOL bRc = GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        ( LPCSTR)this, &hModule );
    if ( bRc )
    {
        GetModuleFileName(hModule, g_szProjectName, MAX_PATH);
        FreeLibrary(hModule);
        DebugOut(
            "\n*******************Memory Check %s!\n\n",
            g_szProjectName );
    }
    else
    {
        OutputDebugString("\n*******************GetModuleHandleEx Failed!\n\n");
    }

    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );
    /*
    * Set the debug-heap flag to keep freed blocks in the
    * heap's linked list - This will allow us to catch any
    * inadvertent use of freed memory
    */
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
}

ds::~ds ()
{
    OutputDebugString(s_strLine);
    OutputDebugString(" *    End of Application.                            * \n");
    DebugOut(" *    Project %s Begin memory leak check.     * \n", g_szProjectName);
    OutputDebugString(s_strLine);
}

void ds::Init(const char* pProject)
{
    strcpy_s(g_szProjectName, pProject);
}

//volatile ds memory_check;
ds memory_check;


#endif




// End of file


