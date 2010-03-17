/*H***************************************************************************
 File            : PicTransformUtil.h
 Subsystem       : 
 Function Name(s): CPicTransformUtil
 Author          : YiQiu
 Date            : 2010-2-27  
 Time            : 16:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _PICTRANSFORMUTIL_H_2010_2
#define _PICTRANSFORMUTIL_H_2010_2


#include "..\..\Base\Include\Common.h"

#ifdef PLAYER_EXPORTS
    #define PICTRANSFORMUTIL_API   API_EXPORT
    #define PICTRANSFORMUTIL_CLASS CLASS_EXPORT
#else    
    #define PICTRANSFORMUTIL_API API_IMPORT
    #define PICTRANSFORMUTIL_CLASS CLASS_IMPORT
    #ifndef PLAYER_LINK
        #define PLAYER_LINK
        #pragma comment(lib, "Player.lib")
        #pragma message("Automatically linking with Player.dll") 
    #endif
#endif


BEGIN_BASE_ENGINE

class PICTRANSFORMUTIL_CLASS CPicTransformUtil
{
public:
    CPicTransformUtil(void);
    ~CPicTransformUtil(void);

    static void YUV422Pack(
        void *pDst, const void *pSrc,
        unsigned int nWidth, unsigned int nHeight, 
        unsigned int nPitch);

    static void YUV420_YUV422Pack(
        void *pDst, const void *pSrc,
        unsigned int nWidth, unsigned int nHeight,
        unsigned int nPitch, INT src420Subtype);

};


END_BASE_ENGINE



#endif  // _PICTRANSFORMUTIL_H_2010_


// End of file



