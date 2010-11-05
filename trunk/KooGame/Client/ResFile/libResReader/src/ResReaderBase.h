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

#ifdef _USE_IRESREADEREX_
    #include "IResReaderEx.h"
    #define IReaderBase IResReaderEx
#else
    #include "IResReader.h"
    #define IReaderBase IResReader
#endif

namespace ResFile
{

template<DWORD Version>
class CResReaderBase:
    public IReaderBase
{
protected:
    typedef TFileHead<Version> FileHead;
    typedef TDataHead<Version> DataHead;

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

#ifdef _USE_IRESREADEREX_
    // IResReaderEx
public:
    virtual size_t Count();
    virtual bool GetSomeInfo(size_t i, CResIterator& iter);
    virtual bool StartGetAllInfo(IInfoReadCallback* p);
    virtual bool EndGetAllInfo();
#endif

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
        void*, size_t);
    void RawDecrypt(void* pIn, size_t nIn){}
    void XorDecrypt(void* pIn, size_t nIn);
    void BlowFishDecrypt(void* pIn, size_t nIn){};

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


#ifdef _USE_IRESREADEREX_
    unsigned int m_hThead;
    IInfoReadCallback* m_pInfoReadCallback;
    bool m_bEndThread;
    unsigned int ReadInfoThread();
    static  unsigned int __stdcall ReadInfoThread(void* p)
    {
        return ((CResReaderBase*)p)->ReadInfoThread();
    }
    
#endif
};


} // namespace ResFile

#endif  // _RESREADERBASE_H_2010_


 // End of file




