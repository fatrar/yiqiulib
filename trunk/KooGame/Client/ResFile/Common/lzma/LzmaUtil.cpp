/**CPP*************************************************************************
 File            : lzmaUtil.cpp
 Subsystem       : 
 Function Name(s): lzmaUtil
 Author          : YiQiu
 Date            : 2010-10-15  
 Time            : 9:52
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "LzmaEnc.h"
#include "LzmaDec.h"
#include "LzmaUtil.h"
#include <assert.h>

namespace LzmaUtil
{

class CLzmaPropsDic
{
public:
    //static CLzmaPropsDic s_LzmaPropsDic;
public:
    static void GetLzmaProps(
        unsigned int nLevel, 
        unsigned char (&szprops)[LZMA_PROPS_SIZE] )
    {
        if ( nLevel > Lzma_Level_Max )
        {
            nLevel = Lzma_Normal;
        }

        LzmaProps& Now = s_Props[nLevel];
        szprops[0] = (Byte)((Now.pb * 5 + Now.lp) * 9 + Now.lc);
        for (int i = 0; i < 4; i++)
            szprops[1 + i] = (Byte)(Now.dictSize >> (8 * i));
    }

   static void InitLzmaProps(
        unsigned int nLevel, 
        CLzmaEncProps& props )
    {
        if ( nLevel > Lzma_Level_Max )
        {
            nLevel = Lzma_Normal;
        }
        LzmaEncProps_Init(&props);
        LzmaProps& Now = s_Props[nLevel];
        props.level = Now.level;
        props.dictSize = Now.dictSize;
        props.lc = Now.lc;
        props.lp = Now.lp;
        props.pb = Now.pb;
        props.fb = Now.fb;
        props.numThreads = 1;
    }  
protected:
    static struct LzmaProps
    {
        int level;       // [0,9] def=5 越大压缩效果就好，速度就越慢
        int dictSize;    // [4K-128MB] def=16M(1<<24)
        int lc;          // [0-8] def=3 big file use 4
        int lp;          // 意思指原始数据的位宽处理，如果是纯二进制当然用0，
                         // 如果是一个DWORD为单位用2^lp default 0
        int pb;          // [0-4] def=2 意思同上不过是压缩数据的
        int fb;          // [5-273] def=32, 越大压缩效果就好，速度就越慢
    } s_Props[Lzma_Level_Max] ;
};

CLzmaPropsDic::LzmaProps CLzmaPropsDic::s_Props[] = 
{
    {0, 1<<12, 3, 0, 2, 5},  // top fast
    {5, 1<<24, 3, 0, 2, 32}, // normal
    {9, 1<<27, 3, 0, 2, 273} // top better compression
};

struct CLzmaAlloc
{
    static void *SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
    static void SzFree(void *p, void *address) { p = p; free(address); }
    static ISzAlloc g_Alloc;
};

ISzAlloc CLzmaAlloc::g_Alloc = { CLzmaAlloc::SzAlloc, CLzmaAlloc::SzFree };

#if defined(_USE_LZMA_COMPRESS_) || defined(_USE_LZMA_ALL)
int LzmaCompress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t srcLen, 
    unsigned int nLevel )
{
    CLzmaEncProps props;
    CLzmaPropsDic::InitLzmaProps(nLevel, props);
    Byte outProps[LZMA_PROPS_SIZE] = {0};
    size_t outPropsSize = LZMA_PROPS_SIZE;
    return LzmaEncode(
        dest, destLen, src, srcLen,
        &props, outProps, &outPropsSize,
        0, NULL, &CLzmaAlloc::g_Alloc, &CLzmaAlloc::g_Alloc);
}
#endif

#if defined(_USE_LZMA_UNCOMPRESS_) || defined(_USE_LZMA_ALL)
int LzmaUncompress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t srcLen,
    unsigned int nLevel )
{
    ELzmaStatus status;
    Byte props[LZMA_PROPS_SIZE] = {0};
    CLzmaPropsDic::GetLzmaProps(nLevel, props);
    return LzmaDecode(
        dest, destLen, src, &srcLen,
        props, LZMA_PROPS_SIZE,
        LZMA_FINISH_ANY, &status, &CLzmaAlloc::g_Alloc);
}

#endif
/*
int Lzma2Compress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t srcLen,
    unsigned int nlevel )
{

}

int Lzma2Uncompress(
    unsigned char *dest, size_t *destLen,
    const unsigned char *src, size_t *srcLen, 
    unsigned int nlevel )
{

}
*/
}

#if defined(_USE_LZMA_UNCOMPRESS_) || defined(_USE_LZMA_ALL)
namespace ResFile
{

int Decode7z(
    void* oData, size_t* oSize, 
    const void* zData, size_t* zSize,
    const BYTE* props)
{
    ELzmaStatus status;
    return LzmaDecode(
        (BYTE*)oData, oSize, (BYTE*)zData, zSize,
        props, LZMA_PROPS_SIZE,
        LZMA_FINISH_ANY, &status, &LzmaUtil::CLzmaAlloc::g_Alloc);
}

int Decode7z(
    void* oData, size_t* oSize, 
    const void* zData, size_t* zSize,
    unsigned int nLevel )
{
    ELzmaStatus status;
    Byte props[LZMA_PROPS_SIZE] = {0};
    LzmaUtil::CLzmaPropsDic::GetLzmaProps(nLevel, props);
    return LzmaDecode(
        (BYTE*)oData, oSize, (BYTE*)zData, zSize,
        props, LZMA_PROPS_SIZE,
        LZMA_FINISH_ANY, &status, &LzmaUtil::CLzmaAlloc::g_Alloc);
    return 0;
}
}
#endif
// End of file