/*H***************************************************************************
File            : MemCheck.h
Subsystem       : 
Function Name(s): 

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


#endif