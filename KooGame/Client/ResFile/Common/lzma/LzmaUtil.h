/**H**************************************************************************
 File            : LzmaUtil.h
 Subsystem       : 
 Function Name(s): LzmaUtil
 Author          : YiQiu
 Date            : 2010-10-15  
 Time            : 9:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _LZMAUTIL_H_2010_10
#define _LZMAUTIL_H_2010_10
#include "Common.h"


namespace LzmaUtil
{

enum 
{
    Lzma_Top_Fast = 0,
    Lzma_Normal,   //default
    Lzma_Best_Compression,
    Lzma_Level_Max,
};

//#if defined(_USE_LZMA_COMPRESS_) || defined(_USE_LZMA_ALL)
int LzmaCompress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t srcLen,
    unsigned int nLevel );
//#endif

//#if defined(_USE_LZMA_UNCOMPRESS_) || defined(_USE_LZMA_ALL)
int  LzmaUncompress(
    unsigned char *dest, size_t *destLen, 
    const unsigned char *src, size_t srcLen,
    unsigned int nLevel );
//#endif

CLASS_EXPORT int Decode7z(
    void* oData, unsigned int* oSize, 
    const void* zData, unsigned int* zSize,
    const unsigned char* props);

CLASS_EXPORT int Decode7z(
    void* oData, unsigned int* oSize, 
    const void* zData, unsigned int* zSize,
    unsigned int nLevel );
/*
int Lzma2Compress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t srcLen,
    unsigned int nlevel );

int  Lzma2Uncompress(
    unsigned char *dest, size_t *destLen, 
    const unsigned char *src, size_t *srcLen,
    unsigned int nlevel );
*/
};





#endif  // _LZMAUTIL_H_2010_


 // End of file



