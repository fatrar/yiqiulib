/*H***************************************************************************
 File            : TransformUtil.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-29  
 Time            : 14:47
 Description     : 
    1. 进制转换 ex. hex->char hex ->base64
    2. 加解密工具

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _TRANSFORMUTIL_H_2010_
#define _TRANSFORMUTIL_H_2010_


#ifdef BASE_EXPORTS
    #define TRANSFORMUTIL_API API_EXPORT
#else    
    #define TRANSFORMUTIL_API API_IMPORT
    #ifndef BASE_LINK
        #define BASE_LINK
        #pragma comment(lib, "Base.lib")
        #pragma message("Automatically linking with Base.dll") 
    #endif
#endif

BEGIN_BASE_ENGINE

namespace TransformUtil
{
    TRANSFORMUTIL_API void Base64Encode(
        const BYTE* pIn, size_t nInLen,
        unsigned char*& pOut, size_t& nOutLen );

    TRANSFORMUTIL_API void Base64Decode(
        const BYTE* pIn, size_t nInLen,
        unsigned char*& pOut, size_t& nOutLen );
}

END_BASE_ENGINE


#endif  // _TRANSFORMUTIL_H_2010_


 // End of file
