/**H**************************************************************************
 File            : ResReaderBase.h
 Subsystem       : 
 Function Name(s): CResReaderBase
 Author          : YiQiu
 Date            : 2010-10-19  
 Time            : 10:58
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESREADERBASE_H_2010_10
#define _RESREADERBASE_H_2010_10
#include <string.h>
#include "ResFileDef.h"
#include "IResReader.h"

namespace ResFile
{

template<DWORD Version>
class CResReaderBase:
    public IResReader
{
protected:
    typedef TFileHead<Version> FileHead;
    typedef TDataInfo<Version> DataInfo;
    typedef TDataHead<Version> DataHead;
    typedef TEncryptParam<Version> EncryptParam;

    // 上层需要实现的
public:
    virtual void GetDataHead(DWORD dwOffset, DataHead& Head) = 0;
    virtual void ReadData(
        DWORD dwOffset, void*& pBuf, size_t nRead, bool& bisRef)=0;
    virtual void ReleaseReadBuf(void*& p) = 0;
    //virtual void* GetFileReadBuf(size_t nBufNeed) = 0;
    // IResReader
public:
    virtual size_t GetDataLen(
        const char* pFileName,
        size_t& nPos );

    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo);

    virtual int GetDataIndex(
        const char* pFileName,
        QWORD& qwHashValue );

    virtual bool GetData(
        size_t nPos,
        CUnPackDataInfo& UnPackDataInfo );

protected:
    CResReaderBase()
        : m_pFileHead(NULL)
    {
        m_DecryptFn[Raw_E_Algo] = &CResReaderBase::RawDecrypt;
        m_DecryptFn[Xor_E_Algo] = &CResReaderBase::XorDecrypt;
        m_DecryptFn[BlowFish_E_Algo] = &CResReaderBase::BlowFishDecrypt;

        m_UnPackFn[Raw_C_Algo] = &CResReaderBase::RawUnPack;
        m_UnPackFn[Zip_C_Algo] = &CResReaderBase::ZipUnPack;
        m_UnPackFn[Lzma_C_Algo] = &CResReaderBase::LzmaUnPack;
    }

    // Decrypt function
protected:
    typedef void (CResReaderBase<Version>::*DecryptFn)(
        void*, size_t, const EncryptParam&);
    void RawDecrypt(void* pIn, size_t nIn,const EncryptParam&){}
    void XorDecrypt(void* pIn, size_t nIn,const EncryptParam& p);
    void BlowFishDecrypt(void* pIn, size_t nIn,const EncryptParam& p){};

    // UnPack function
protected:
    typedef void (CResReaderBase<Version>::*UnPackFn)(
        void*, size_t, void*, size_t&, size_t);
    void RawUnPack(void* pIn, size_t nIn, void* pOut, size_t& nOut,size_t)
    {
        memcpy(pOut, pIn, nIn);
    }
    void ZipUnPack(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut, size_t nLevel){}
    void LzmaUnPack(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut, size_t nLevel);

    //  unpack memory manage
protected:
    void* GetUnPackBuf(
        size_t nBufNeed,
        CUnPackDataInfo* pUnPackDataInfo);

    virtual void Release(CUnPackDataInfo* pUnPackDataInfo);

    // Util
protected:
    int Find(const char* pFileName);
    int Find(const QWORD& qwHashValue);

protected:
    FileHead* m_pFileHead;
    DecryptFn m_DecryptFn[Encrypt_Count];
    UnPackFn  m_UnPackFn[Compress_Count];
};


} // namespace ResFile

#endif  // _RESREADERBASE_H_2010_


 // End of file




