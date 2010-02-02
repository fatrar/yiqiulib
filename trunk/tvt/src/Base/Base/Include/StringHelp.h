/*H***************************************************************************
 File            : StringHelp.h
 Subsystem       : 
 Function Name(s): StringHelp
 Author          : YiQiu
 Date            : 2010-2-1  
 Time            : 9:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _STRINGHELP_H_2010_2_
#define _STRINGHELP_H_2010_2_

#include "Common.h"

#ifdef BASE_EXPORTS
    #define STRINGHELP_API API_EXPORT
    #define STRINGHELP_CLASS CLASS_EXPORT
#else    
    #define STRINGHELP_API API_IMPORT
    #define STRINGHELP_CLASS CLASS_IMPORT
    #ifndef BASE_LINK
        #define BASE_LINK
        #pragma comment(lib, "Base.lib")
        #pragma message("Automatically linking with Base.dll") 
    #endif
#endif


BEGIN_BASE_ENGINE

namespace StringHelp
{

};






END_BASE_ENGINE // namespace


#include "ParseString.h"



#endif  // _STRINGUTIL_H_2010_


// End of file




