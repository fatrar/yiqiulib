/**H**************************************************************************
 File            : Trace.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-3-22  
 Time            : 13:47
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _TRACE_H_2010_3
#define _TRACE_H_2010_3
#pragma once



#ifndef DEBUGUTIL_EXPORTS
    #if !defined(_DEBUG)
    //#pragma comment(lib, "DebugUtil.lib")
    //#pragma message("Automatically linking with DebugUtil.lib")
    #elif defined(_DEBUG) /*&& defined(MEMORY_CHECK)*/
        #ifndef DEBUGUTIL_LINK
        #define DEBUGUTIL_LINK
            #pragma comment(lib, "DebugUtil_Debug.lib")
            #pragma message("Automatically linking with DebugUtil_Debug.lib") 
        #endif
    #endif
#endif



#ifdef _DEBUG
    void __cdecl DebugOut(const char* format, ...);
    void __cdecl DebugOutIndex (const char *format, ... );
    void __cdecl KAssert(const char *format, ...);
#else
    #define DebugOut __noop
    #define  DebugOutIndex __noop
    #define KAssert __noop
#endif // _DEBUG







#endif  // _TRACE_H_2010_


// End of file




