// LzmaTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

SRes MyLzmaEncode(Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen,
                const CLzmaEncProps *props, Byte *propsEncoded, SizeT *propsSize, int writeEndMark,
                ICompressProgress *progress, ISzAlloc *alloc, ISzAlloc *allocBig)
{
    CLzmaEncHandle *p = (CLzmaEncHandle *)LzmaEnc_Create(alloc);
    SRes res;
    if (p == 0)
        return SZ_ERROR_MEM;

    res = LzmaEnc_SetProps(p, props);
    if (res == SZ_OK)
    {
        res = LzmaEnc_WriteProperties(p, propsEncoded, propsSize);
        if (res == SZ_OK)
            res = LzmaEnc_MemEncode(p, dest, destLen, src, srcLen,
            writeEndMark, progress, alloc, allocBig);
    }

    LzmaEnc_Destroy(p, alloc, allocBig);
    return res;
}


const char g_TestData[] = "0123456789";

void*SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
void SzFree(void *p, void *address) { p = p; free(address); }


ISzAlloc g_Alloc = { SzAlloc, SzFree };

int _tmain(int argc, _TCHAR* argv[])
{
    char* pTest = new char[1001];
    char* pTmp = pTest;
    for (int i =0; i<1; ++i)
    {
        strcpy(pTmp, g_TestData);
        pTmp += sizeof(g_TestData);
    }
    char szBuf[3000] = {0};
    size_t nDest = 3000;
   
    BYTE szProps[5] = {0};
    size_t outPropsSize = 5;
    int nRc = LzmaCompress(
        (BYTE*)szBuf, &nDest, (BYTE*)pTest, 1, 
        szProps, &outPropsSize, 5, 1<<24, 3,0,2,32,1);


    char szUnPackDest[3000] = {0};
    size_t nUnPackDest = 3000;
    LzmaUncompress((BYTE*)szUnPackDest, &nUnPackDest, (BYTE*)szBuf, &nDest, szProps, outPropsSize);
    //CLzmaEncProps props;
    //LzmaEncProps_Init(&props);
    //props.level = 5;
    //props.dictSize = 1;
    //props.lc = 3;
    //props.lp = 0;
    //props.pb = 2;
    //props.fb = 32;
    //props.numThreads = 1;

    
    //MyLzmaEncode(
    //   (BYTE*)szBuf, &nDest, (BYTE*)g_TestData, sizeof(g_TestData), 
    //   &props, &outPropsSize, NULL, 0, NULL, &g_Alloc, NULL);
	return 0;
}

