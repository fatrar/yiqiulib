/**H**************************************************************************
 File            : ResFileReader.h
 Subsystem       : 
 Function Name(s): CResFileReader
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 14:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESFILEREADER_H_2010_10
#define _RESFILEREADER_H_2010_10

#include "ResFileDef.h"
#include "IResReader.h"
#include <FileSystem.h>

namespace ResFile
{

template<DWORD Version> 
class CResFileReader:
    public IResReader
{
    typedef void (CResFileReader<Version>::*DecryptFn)(void*, size_t, void*);
    typedef void (CResFileReader<Version>::*UnPackFn)(void*, size_t, void*, size_t);
public:
    CResFileReader(
        FileSystem::CFile* pResFile, 
        const TFileHeadBase& HeadBase ){};
    virtual ~CResFileReader(void){};

    // IResReader
public:
    virtual bool Parse(){return false;}
    virtual size_t GetDataLen(const char* pFileName){return 0;};
    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo){return false;};

    virtual void Release(CUnPackDataInfo* pUnPackDataInfo){};

protected:
    int Find(const char* pFileName);//{return -1;};
    void* GetFileReadBuf(size_t nBufNeed);
    void* GetUnPackBuf(size_t nBufNeed, CUnPackDataInfo* pUnPackDataInfo);

    // Decrypt 
protected:
    void RawDecrypt(void* pIn, size_t nIn,void*){}
    void XorDecrypt(void* pIn, size_t nIn,void*);
    void BlowFishDecrypt(void* pIn, size_t nIn,void* pEncryptParam);

    // UnPack
protected:
    void RawUnPack(void* pIn, size_t nIn, void* pOut, size_t nOut){}
    void LZMAUnPack(void* pIn, size_t nIn, void* pOut, size_t nOut);
    void ZipUnPack(void* pIn, size_t nIn, void* pOut, size_t nOut);

    typedef TFileHead<Version> FileHead;
    typedef TDataInfo<Version> DataInfo;
    typedef TDataHead<Version> DataHead;

protected:
    FileHead m_FileHead;
    DecryptFn m_DecryptFn[Encrypt_Count];
    UnPackFn  m_UnPackFn[Compress_Count];
    //DWORD m_dwHeadSize, m_dwFileCount;
    FileSystem::CFile* m_ResFile;
};

class CMemResReader : 
    public ResFile::IResReader
{
public:
    CMemResReader();
    virtual ~CMemResReader();
};




}


#endif  // _RECFILEREADER_H_2010_


// End of file