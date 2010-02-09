/*H***************************************************************************
File            : DebugUtil.h
Subsystem       : 
Function Name(s): DebugUtil

Description     : 
Author          : Yiqiu
Date            : 2009/3/26
Revision        : 

History
-------


Copyright (c) DOWSHU Electronica (China) Ltd.
***************************************************************************H*/
#ifndef _DEBUGUTIL_H_2009_3_
#define _DEBUGUTIL_H_2009_3_

#define MEMORY_CHECK

#ifndef DEBUGUTIL_EXPORTS
    #if !defined(_DEBUG)
        #pragma comment(lib, "DebugUtil.lib")
        #pragma message("Automatically linking with DebugUtil.lib")
    #elif defined(_DEBUG) && defined(MEMORY_CHECK)
        #pragma comment(lib, "DebugUtil_Debug.lib")
        #pragma message("Automatically linking with DebugUtil_Debug.lib") 
    /*#elif defined(_DEBUG) && !defined(MEMORY_CHECK)
        #pragma comment(lib, "OtherUtil_debug.lib")
        #pragma message("Automatically linking with OtherUtil_debug.lib") */
    #endif
#endif

#include <stdio.h>

#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
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


/*==========================================
Debug utility.
==========================================*/
void __cdecl DebugOut(const DEBUG_CHAR* format, ...);
void __cdecl DebugOutIndex ( const DEBUG_CHAR *format, ... );
void __cdecl KAssert(const DEBUG_CHAR *format, ...);

// for memory leak
#if defined(MEMORY_CHECK) && defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
    struct ds
    {
        ds();
        ~ds();
        void Init();
    };

    #define new new(_NORMAL_BLOCK, __FILE__,__LINE__)
    extern ds memory_check;
#endif


//#ifdef WIN32
//#   ifdef _DEBUG
//#      define MEMORY_CHECK ds memory_check
//#   endif //_DEBUG
//#   ifndef _DEBUG
//#      define MEMORY_CHECK int memory_check
//#   endif // _DEBUG
//#else
//#   define MEMORY_CHECK int memory_check
//#endif

        // NEW

//#if defined(_DEBUG)&&defined(_CRTDBG_MAP_ALLOC)
//#define new new(_NORMAL_BLOCK, __FILE__,__LINE__)
//#else
//        //#define new new
//#endif

#endif