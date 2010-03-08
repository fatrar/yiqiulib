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
        //#pragma comment(lib, "DebugUtil.lib")
        //#pragma message("Automatically linking with DebugUtil.lib")
    #elif defined(_DEBUG) && defined(MEMORY_CHECK)
        #ifndef DEBUGUTIL_LINK
            #define DEBUGUTIL_LINK
            #pragma comment(lib, "DebugUtil_Debug.lib")
            #pragma message("Automatically linking with DebugUtil_Debug.lib") 
        #endif
        
    /*#elif defined(_DEBUG) && !defined(MEMORY_CHECK)
        #pragma comment(lib, "OtherUtil_debug.lib")
        #pragma message("Automatically linking with OtherUtil_debug.lib") */
    #endif
#endif


#ifdef _DEBUG
    void __cdecl DebugOut(const char* format, ...);
    void __cdecl DebugOutIndex (const char *format, ... );
    void __cdecl KAssert(const char *format, ...);
#else
    static void __cdecl DebugOut(const char* format, ...){}
    static void __cdecl DebugOutIndex(const char *format, ... ){}
    static void __cdecl KAssert(const char *format, ...){}
#endif // _DEBUG

// ctrgbg.h  1143
// for memory leak
#if defined(MEMORY_CHECK) && defined(_DEBUG)
    #pragma message("Run DebugUtil_Debug...")
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
    struct ds
    {
        ds();
        ~ds();
        void Init(const char* pProject = __FILE__ );
    };

    //#undef _CRTDBG_MAP_ALLOC
    #define new new(_NORMAL_BLOCK, __FILE__,__LINE__)
    //extern volatile ds memory_check;
    //static ds memory_check;
    extern ds memory_check;
#else
    struct ds
    {
        ds(){};
        ~ds(){};
        void Init(const char* pProject = __FILE__ ){};
    };
    static ds memory_check;
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