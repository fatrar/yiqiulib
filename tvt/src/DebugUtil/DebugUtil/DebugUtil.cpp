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



/*==========================================
Debug utility.
==========================================*/

void DebugOut(const DEBUG_CHAR* format, ...)
{
#ifdef _DEBUG
    const unsigned int bufsize = 10 * 1024 + 1;
    DEBUG_CHAR *buf = new DEBUG_CHAR[bufsize];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);
#ifdef _MSC_VER
    OutputDebugString(buf);
#endif // _MSC_VER

#ifdef __APPLE__
    __printf ( "%s", buf ) ;
#endif // __APPLE__
    __fprintf ( stdout, DEBUG_T("%s"), buf ) ;
    delete[] buf;
#endif // _USERDEBUG
}

void DebugOutIndex ( const DEBUG_CHAR *format, ... )
{
#ifdef _DEBUG
    __fprintf ( stdout, DEBUG_T("=======================================================\n") ) ;
    const unsigned int bufsize = 10 * 1024 + 1;
    DEBUG_CHAR *buf = new DEBUG_CHAR[bufsize];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);
#ifdef _MSC_VER
    OutputDebugString(DEBUG_T("=======================================================\n"));
    OutputDebugString(buf);
    OutputDebugString(DEBUG_T("=======================================================\n"));
#endif // _MSC_VER

#ifdef __APPLE__
    __printf ( "%s", buf ) ;
#endif // __APPLE__
    __fprintf ( stdout, DEBUG_T("%s"), buf ) ;
    delete[] buf;
    __fprintf ( stdout, DEBUG_T("=======================================================\n") ) ;
#endif // _USERDEBUG
}


void KAssert(const DEBUG_CHAR *format, ...)
{
#ifdef _DEBUG
    const unsigned int bufsize = 10 * 1024 + 1;
    DEBUG_CHAR *buf = new DEBUG_CHAR[bufsize];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);

#ifdef _MSC_VER
    OutputDebugString(buf);
#endif // _MSC_VER

#ifdef __APPLE__
    __printf ( "%s", buf ) ;
#endif // __APPLE__

    __fprintf ( stderr, DEBUG_T("%s"), buf ) ;
    delete[] buf;
    exit ( EXIT_FAILURE ) ;
#endif // _USERDEBUG
}


#if defined(MEMORY_CHECK) && defined(_DEBUG)
ds::ds()
{
    int tmpDbgFlag;

    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );
    /*
    * Set the debug-heap flag to keep freed blocks in the
    * heap's linked list - This will allow us to catch any
    * inadvertent use of freed memory
    */
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
}

ds::~ds ()
{
    DebugOut ( DEBUG_T(" *============================================* \n") ) ;
    DebugOut ( DEBUG_T(" *    End of Application.                     * \n") ) ;
    DebugOut ( DEBUG_T(" *    Begin memory leak check.                * \n") ) ;
    DebugOut ( DEBUG_T(" *============================================* \n") ) ;
}

void ds::Init()
{
}

ds memory_check;
#endif




// End of file


