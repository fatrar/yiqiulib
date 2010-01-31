/*H***************************************************************************
 File            : Base64.cpp
 Subsystem       : 
 Function Name(s): BaseUtil
 Author          : YiQiu
 Date            : 2010-1-29  
 Time            : 14:50
 Description     : 
 从base拷贝过来，不同是Base64Decode的内存模型是调用者申请好
 目地是为了减少基础模块之间的依赖，
 在我看来tinyxml和Base是最底层的模块。
 本来想实现一次内存拷贝就完成去数据的，但是Base算法要改成那样，代码会比较恶心。
 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"

/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 64->hex table
static BYTE dtable[256];
static bool bdtable = false;

void InitBase64DecodeTable()
{
    memset(dtable, 0x80, 256);
    for (size_t i = 0; i < sizeof(cb64) - 1; i++)
        dtable[cb64[i]] = (BYTE) i;
    dtable['='] = 0;

    bdtable = true;
}

void Base64Encode(
    const BYTE* pIn, size_t nInLen,
    unsigned char*& pOut, size_t& nOutLen )
{
    size_t nMod = nInLen%3;
    size_t nGroup = nInLen/3;
    if ( nMod == 0)
    {
        nOutLen = nGroup*4;
    }
    else
    {
        nOutLen = (nGroup+1)*4;
    }

    pOut= new BYTE[nOutLen];

    const BYTE* pInTmp = pIn;
    BYTE* pOutTmp = pOut;
    for ( size_t i=0; 
          i<nGroup;
          ++i, pInTmp+=3, pOutTmp+=4 )
    {
        pOutTmp[0] = cb64[ pInTmp[0] >> 2 ];
        pOutTmp[1] = cb64[ ((pInTmp[0] & 0x03) << 4) | ((pInTmp[1] & 0xf0) >> 4) ];
        pOutTmp[2] = cb64[ ((pInTmp[1] & 0x0f) << 2) | ((pInTmp[2] & 0xc0) >> 6) ];
        pOutTmp[3] = cb64[ pInTmp[2] & 0x3f ];
    }

    if ( nMod == 1)
    {
        pOutTmp[0] = cb64[ pInTmp[0] >> 2 ];
        pOutTmp[1] = cb64[ ((pInTmp[0] & 0x03) << 4) ];
        pOutTmp[2] = (BYTE)'=';
        pOutTmp[3] = (BYTE)'=';
    }
    else if ( nMod == 2 )
    {
        pOutTmp[0] = cb64[ pInTmp[0] >> 2 ];
        pOutTmp[1] = cb64[ ((pInTmp[0] & 0x03) << 4) | ((pInTmp[1] & 0xf0) >> 4) ];
        pOutTmp[2] = cb64[ ((pInTmp[1] & 0x0f) << 2) ];
        pOutTmp[3] = (BYTE)'=';
    }
}

/**
// 参考 git://android.git.kernel.org/platform/external/wpa_supplicant.git  > base64.c
*/
void Base64Decode(
    const BYTE* pIn, size_t nInLen,
    unsigned char*& pOut, size_t& nOutLen )
{
    if ( !bdtable )
    {
        InitBase64DecodeTable();
    }

    unsigned char *pos, in[4], block[4], tmp;
    size_t i, count, olen;

    count = 0;
    for (i = 0; i < nInLen; i++)
    {
        if (dtable[pIn[i]] != 0x80)
            count++;
    }
    if (count % 4)
    {
        pOut = NULL;
        nOutLen = 0;
        return;
    }

    olen = count / 4 * 3;
    pos = pOut = new BYTE[olen];

    count = 0;
    for (i = 0; i < nInLen; i++)
    {
        tmp = dtable[pIn[i]];
        if (tmp == 0x80)
            continue;

        in[count] = pIn[i];
        block[count] = tmp;
        count++;
        if (count == 4)
        {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
        }
    }

    if (pos > pOut) 
    {
        if (in[2] == '=')
            pos -= 2;
        else if (in[3] == '=')
            pos--;
    }

    nOutLen = pos - pOut;
}



// End of file