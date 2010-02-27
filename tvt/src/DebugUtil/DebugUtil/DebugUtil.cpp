/*CPP*************************************************************************
File            : DebugUtil.cpp
Subsystem       : 
Function Name(s): CDebugUtil

Description     : 
Author          : Yiqiu
Date            : 2009/3/26
Revision        : 

History
-------


Copyright (c) DOWSHU Electronica (China) Ltd.
*************************************************************************CPP*/
#include "DebugUtil.h"


#ifdef _DEBUG


#pragma warning(disable: 4996)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <stdio.h>   
#include <windows.h>
#include <tchar.h>


#define  Max_Buf_Size  1024
static const char s_strLine[] = "=======================================================\n";



void DebugOut(const char* format, ...)
{
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(szbuf);
    fprintf(stdout, "%s", szbuf) ;
}

void DebugOutIndex ( const char *format, ... )
{
    fprintf(stdout,  s_strLine) ;
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(s_strLine);
    OutputDebugString(szbuf);
    OutputDebugString(s_strLine);

    fprintf(stdout, "%s", szbuf ) ;
    fprintf(stdout, s_strLine) ;
}


void KAssert(const char *format, ...)
{
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(szbuf);

    fprintf (stderr, "%s", szbuf) ;
    exit ( EXIT_FAILURE ) ;
}

#endif // _DEBUG


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


