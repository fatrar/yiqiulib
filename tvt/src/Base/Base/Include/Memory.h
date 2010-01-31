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
    #define MEMORYUTIL API_EXPORT
#else    
    #define MEMORYUTIL API_IMPORT
    #ifndef BASE_LINK
        #define BASE_LINK
        #pragma comment(lib, "Base.lib")
        #pragma message("Automatically linking with Base.dll") 
    #endif
#endif

BEGIN_BASE_ENGINE

namespace MemoryUtil
{
    API_EXPORT void Delete(void* p);
    API_EXPORT void DeleteArray(void* p);
};



END_BASE_ENGINE // namespace




#endif  // _MEMORY_H_2010_


 // End of file