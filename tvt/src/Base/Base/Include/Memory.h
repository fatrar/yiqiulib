/*H***************************************************************************
 File            : Memory.h
 Subsystem       : 
 Function Name(s): CMemory
 Author          : YiQiu
 Date            : 2010-1-30  
 Time            : 15:53
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _MEMORY_H_2010_
#define _MEMORY_H_2010_

#include "Common.h"

#ifdef BASE_EXPORTS
    #define MEMORYUTIL_API API_EXPORT
#else    
    #define MEMORYUTIL_API API_IMPORT
    #ifndef BASE_LINK
        #define BASE_LINK
        #pragma comment(lib, "Base.lib")
        #pragma message("Automatically linking with Base.dll") 
    #endif
#endif

BEGIN_BASE_ENGINE

namespace MemoryUtil
{
    MEMORYUTIL_API void Delete(void* p);
    MEMORYUTIL_API void DeleteArray(void* p);
};



END_BASE_ENGINE // namespace




#endif  // _MEMORY_H_2010_


 // End of file
