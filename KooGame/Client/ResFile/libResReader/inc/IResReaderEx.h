/*H***************************************************************************
File            : IResReaderEx.h
Subsystem       : 
Function Name(s): CIResReaderEx

Description     : 
Author          : Yiqiu
Date            : 2010/10/31
Revision        : 

History
-------


Copyright (c) Shenzhen KooGame Co.,Ltd.
***************************************************************************H*/
#ifndef _IRESREADEREX_H_2010_10
#define _IRESREADEREX_H_2010_10
#include "IResReader.h"

#ifdef _USE_IRESREADEREX_


namespace ResFile
{

class CResIterator
{
public:
    CResIterator()
    {
        memset(this, 0, sizeof(CResIterator));
        m_eAlgo = Encrypt_Count;
        m_cAlgo = Compress_Count;
        m_cParam = CompressParam_Count;
    }

public:
    DWORD m_nIndex;
    UHashValue m_HashValue;
    DWORD m_dwPackDataLen;
    DWORD m_dwDataOffset;
    const char* m_pFileName;

    DWORD m_dwRawDataLen;
    eEncryptAlgo m_eAlgo;
    eCompressAlgo m_cAlgo;
    eCompressParam m_cParam;
};

struct IInfoReadCallback
{
    virtual void OnInfoRead(
        DWORD nIndex,
        eEncryptAlgo eAlgo,
        eCompressAlgo cAlgo,
        eCompressParam cParam,
        DWORD dwRawDataLen ) = 0;
};

struct IResReaderEx :
    IResReader
{
    virtual size_t Count() = 0;
    virtual bool GetSomeInfo(size_t i, CResIterator& iter) = 0;
    virtual bool StartGetAllInfo(IInfoReadCallback* p) = 0;
    virtual bool EndGetAllInfo() = 0;
};

inline IResReaderEx* CreateResFileReaderEx(const char* pResFilePath)
{
    return (IResReaderEx*)CreateResFileReader(pResFilePath);
}

/**
*@note 从内存数据创建资源包读取对象
*@param	pData 压缩包数据  
*@param nSize 数据长度  
*@param bRef 是否直接引用数据 
*@param bDelete 如果是引用，是否在析构delete pData
*/
inline IResReaderEx* CreateMemResReaderEx(
    BYTE* pData, size_t nSize,
    bool bRef, bool bDelete )
{
    return (IResReaderEx*)CreateMemResReader(
        pData, nSize, bRef, bDelete);
}

/**
*@note 干掉对象且会将pResReader赋空
*/
inline void DestroyResReaderEx(IResReaderEx*& pResReader)
{
    return DestroyResReader((IResReader*&)pResReader);
}

#endif


}






#endif