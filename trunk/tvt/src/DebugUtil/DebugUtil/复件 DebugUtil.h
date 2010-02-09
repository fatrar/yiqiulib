/*H***************************************************************************
File            : DebugUtil.h
Subsystem       : 
Function Name(s): DebugUtil

Description     : 
Author          : Yiqiu
Date            : 2008/12/08
Revision        : 

History
-------


Copyright (c) DOWSHU Electronica (China) Ltd.
***************************************************************************H*/
#ifndef _DebugUtil_h_2008_12
#define _DebugUtil_h_2008_12


#include <stdio.h>

#ifdef WIN32
#   include <windows.h>
#    include <tchar.h>
#endif // WIN32

#ifndef EXIT_FAILURE
    #define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#ifdef _UNICODE
    typedef wchar_t DEBUG_CHAR ;
    #define __vsprintf wvsprintf
    #define __printf wprintf 
    #define __fprintf fwprintf
    #define DEBUG_T(x) _T(x)
#else
    typedef char DEBUG_CHAR ;
    #define __vsprintf  vsprintf
    #define __printf printf 
    #define __fprintf fprintf
    #define DEBUG_T(x) x
#endif // UNICODE

// NEW
#if defined(_DEBUG)&&defined(_CRTDBG_MAP_ALLOC)&&!defined(NEW)
    #define NEW ::new(_NORMAL_BLOCK, __FILE__,__LINE__)
#else
    #define NEW new
#endif

/*==========================================
Debug utility.
==========================================*/
inline void DebugOut(int opt, const DEBUG_CHAR *format, ...)
{
#   ifdef _USERDEBUG
    if ( opt ) {
        const unsigned int bufsize = 10 * 1024 + 1;
        DEBUG_CHAR *buf = NEW DEBUG_CHAR[bufsize];
        va_list args;
        va_start(args, format);
        __vsprintf(buf, format, args);
        va_end(args);
#   ifdef _MSC_VER
        OutputDebugString(buf);
#   endif // _MSC_VER
#   ifdef __APPLE__
        __printf ( "%s", buf ) ;
#   endif // __APPLE__
        __fprintf ( stdout, DEBUG_T("%s"), buf ) ;
        delete[] buf;
    }
#   endif // _USERDEBUG
}

inline void DebugOut(const DEBUG_CHAR* format, ...)
{
#   ifdef _USERDEBUG
    const unsigned int bufsize = 10 * 1024 + 1;
    DEBUG_CHAR *buf = NEW DEBUG_CHAR[bufsize];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);
#   ifdef _MSC_VER
    OutputDebugString(buf);
#   endif // _MSC_VER
#   ifdef __APPLE__
    __printf ( "%s", buf ) ;
#   endif // __APPLE__
    __fprintf ( stdout, DEBUG_T("%s"), buf ) ;
    delete[] buf;
#   endif // _USERDEBUG
}

inline void DebugOutIndex ( const DEBUG_CHAR *format, ... )
{
#   ifdef _USERDEBUG
    __fprintf ( stdout, DEBUG_T("=======================================================\n") ) ;
    const unsigned int bufsize = 10 * 1024 + 1;
    DEBUG_CHAR *buf = NEW DEBUG_CHAR[bufsize];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);
#   ifdef _MSC_VER
    OutputDebugString(DEBUG_T("=======================================================\n"));
    OutputDebugString(buf);
    OutputDebugString(DEBUG_T("=======================================================\n"));
#   endif // _MSC_VER
#   ifdef __APPLE__
    __printf ( "%s", buf ) ;
#   endif // __APPLE__
    __fprintf ( stdout, DEBUG_T("%s"), buf ) ;
    delete[] buf;
    __fprintf ( stdout, DEBUG_T("=======================================================\n") ) ;
#   endif // _USERDEBUG
}


inline void KAssert(int opt, const DEBUG_CHAR *format, ...)
{
#   ifdef _USERDEBUG
    if ( opt ) {
        const unsigned int bufsize = 10 * 1024 + 1;
        DEBUG_CHAR *buf = NEW DEBUG_CHAR[bufsize];
        va_list args;
        va_start(args, format);
        __vsprintf(buf, format, args);
        va_end(args);
#   ifdef _MSC_VER
        OutputDebugString(buf);
#   endif // _MSC_VER
#   ifdef __APPLE__
        __printf ( "%s", buf ) ;
#   endif // __APPLE__
        __fprintf ( stderr, DEBUG_T("%s"), buf ) ;
        delete[] buf;
        exit ( EXIT_FAILURE ) ;
    }
#   endif // _USERDEBUG
}

// for memory leak
#ifdef _DEBUG
#   include "DebugUtil.h"
#   ifdef WIN32
#       include <new>
#       include <memory>
#       define _CRTDBG_MAP_ALLOC
#       include <cstdlib>
#       include <stdlib.h>
#       include <crtdbg.h>
struct ds {
    ds () {
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
    ~ds () {
        DebugOut ( DEBUG_T(" *============================================* \n") ) ;
        DebugOut ( DEBUG_T(" *    End of Application.                     * \n") ) ;
        DebugOut ( DEBUG_T(" *    Begin memory leak check.                * \n") ) ;
        DebugOut ( DEBUG_T(" *============================================* \n") ) ;
    }
} ;
#   endif
#endif


#ifdef WIN32
#   ifdef _DEBUG
#      define MEMORY_CHECK ds memory_check
#   endif //_DEBUG
#   ifndef _DEBUG
#      define MEMORY_CHECK int memory_check
#   endif // _DEBUG
#else
#   define MEMORY_CHECK int memory_check
#endif


#endif // DebugUtil_h_2008_12







// End of file



