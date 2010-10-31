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
*@note ���ڴ����ݴ�����Դ����ȡ����
*@param	pData ѹ��������  
*@param nSize ���ݳ���  
*@param bRef �Ƿ�ֱ���������� 
*@param bDelete ��������ã��Ƿ�������delete pData
*/
inline IResReaderEx* CreateMemResReaderEx(
    BYTE* pData, size_t nSize,
    bool bRef, bool bDelete )
{
    return (IResReaderEx*)CreateMemResReader(
        pData, nSize, bRef, bDelete);
}

/**
*@note �ɵ������һὫpResReader����
*/
inline void DestroyResReaderEx(IResReaderEx*& pResReader)
{
    return DestroyResReader((IResReader*&)pResReader);
}

#endif


}






#endif