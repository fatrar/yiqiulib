/**H**************************************************************************
 File            : ResPacker.h
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-12  
 Time            : 16:06
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESPACKER_H_2010_10
#define _RESPACKER_H_2010_10


#include "IResPacker.h"
#include <string>
#include <list>
#include <set>
using namespace std;
#include <Windows.h>

namespace FileSystem{class CFile;} 

namespace ResFile
{

template<DWORD Version>
class CResPacker :
    public IResPacker
{
public:
    typedef TEncryptParam<Version> EncryptParam;
    typedef TCompressParam<Version> CompressParam;

    CResPacker( 
        const char* pResFlodPath, 
        eCompressAlgo cAlgo,
        const CompressParam& pcParam,
        eEncryptAlgo eAlgo,
        const EncryptParam& peParam );
    ~CResPacker(void);

    // IResPacker
public:
    virtual void AddFile(
        const char* pFileName );

    virtual void AddFile(
        const char* pFileName,
        eCompressAlgo cAlgo,
        void* pcParam,
        eEncryptAlgo eAlgo,    
        void* peParam );

    virtual void MakeFile(
        const char* pPackFilePath,
        eFileNamePos eFileNamePos);

    // Struct Define
protected:
    struct FileInfo
    {
        inline FileInfo() 
            : pRawDataBuf(NULL)
            , nRawDataSize(0)
            , nCompressDataSize(0){}

        string strFileName;
        eEncryptAlgo eAlgo;
        TEncryptParam<Version> eParam;
        eCompressAlgo cAlgo;
        TCompressParam<Version> cParam;

        // ´æÊý¾Ý
        BYTE* pRawDataBuf;
        size_t nRawDataSize;
        size_t nCompressDataSize;

        FileInfo(const FileInfo& a)
        {
            strFileName = a.strFileName;
            eAlgo = a.eAlgo;
            eParam = a.eParam;
            cAlgo = a.cAlgo;
            cParam = a.cParam;

            pRawDataBuf = a.pRawDataBuf;
            nRawDataSize = a.nRawDataSize;
            nCompressDataSize = a.nCompressDataSize;
        }
    };

    typedef list<FileInfo> FileInfoList;
    typedef TFileHead<Version> FileHead;
    template<DWORD Version> class DataIndexList;

    // Version 1.0 Define
protected:
    template<> class DataIndexList<File_Version_1_0> :
        public set<TFileHead<File_Version_1_0>::TDataIndex >{};

protected:
    static ::DWORD WINAPI DataTransform(void* p)
    {
        CResPacker* pThis = (CResPacker*)p;
        return pThis->DataTransform();
    }
    DWORD DataTransform();

    void Init();
    void Unit();
    void DoRead();
    void DoWrite(
        const char* pPackFilePath,
        eFileNamePos FileNamePos);

    void DoWriteFileName(
        FileSystem::CFile& Writer );

    DWORD GetFileNameDataLen();

    void TransformOne(FileInfo& Info);

    // Encrypt 
protected:
    typedef void (CResPacker<Version>::*EncryptFn)(
        void*, size_t, const EncryptParam&);
    void RawEncrypt(void* pIn, size_t nInLen, const EncryptParam&){}
    void XorEncrypt(void* pIn, size_t nInLen, const EncryptParam& p);
    void BlowFishEncrypt(void* pIn, size_t nInLen, const EncryptParam& p);

    // Compress
protected:
    typedef int (CResPacker<Version>::*CompressFn)(
        void*, size_t, void*, size_t&, const CompressParam&);
    int RawCompress(
        void* pIn, size_t nIn, void* pOut, size_t& nOut,
        const CompressParam& p)
    {  
        memcpy(pOut,pIn,nIn);
        nOut = nIn;
        return 0;
    }
    int ZipCompress(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut,
        const CompressParam& p){return 0;};
    int LzmaCompress(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut,
        const CompressParam& p);
private:
    eEncryptAlgo m_DefeAlgo;
    EncryptParam m_DefeParam;

    eCompressAlgo m_DefcAlgo;
    CompressParam m_DefcParam;

    string m_strResFold;

    FileInfoList m_FileInfoList;
    DataIndexList<Version> m_DataIndexList;

    //
    // Thread
    //
    HANDLE m_hTransThread;
    enum eReadEvent {
        ReadSome_Event,
        ReadFinsih_Event,
        ReadEvent_Count,
    };
    HANDLE m_hReadEvent[ReadEvent_Count];

    BYTE* m_pRawFileBuf;
    size_t m_nRawFileBufUse;

    BYTE* m_pResFileBuf;
    BYTE* m_pResFileBufNow;
    size_t m_nResFileBufRemain;

    CompressFn m_CompressFn[Compress_Count];
    EncryptFn m_EncryptFn[Encrypt_Count];
};

}




#endif  // _RESPACKER_H_2010_


// End of file

