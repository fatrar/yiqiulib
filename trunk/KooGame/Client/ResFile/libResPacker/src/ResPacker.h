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

namespace FileSystem{class CFile;} 

namespace ResFile
{

template<DWORD Version> class DataIndexList;

template<> class DataIndexList<File_Version_1_0> :
    public set<TFileHead<File_Version_1_0>::TDataIndex>{};
template<> class DataIndexList<File_Version_1_1> :
    public set<TFileHead<File_Version_1_1>::TDataIndex>{};


template<DWORD Version>
class CResPacker :
    public IResPacker
{
public:
    CResPacker( 
        const char* pResFlodPath,
        eEncryptAlgo eAlgo,
        BYTE (&szKey)[8],
        eCompressAlgo cAlgo );
    ~CResPacker(void);

    // IResPacker
public:
    virtual void AddFile(
        const char* pFileName );

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
            , nPackDataSize(0){}

        string strFileName;

        // 存数据
        BYTE* pRawDataBuf;
        size_t nRawDataSize;
        size_t nPackDataSize;

        FileInfo(const FileInfo& a)
        {
            strFileName = a.strFileName;
            pRawDataBuf = a.pRawDataBuf;
            nRawDataSize = a.nRawDataSize;
            nPackDataSize = a.nPackDataSize;
        }
    };

    typedef list<FileInfo> FileInfoList;
    typedef TFileHead<Version> FileHead;

protected:
    static unsigned int WINAPI DataTransform(void* p)
    {
        return ((CResPacker*)p)->DataTransform();
    }
    unsigned int DataTransform();

    void Init();
    void Unit();
    void DoRead();
    void ShowLog();
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
        void*, size_t);
    void RawEncrypt(void* pIn, size_t nInLen){}
    void XorEncrypt(void* pIn, size_t nInLen);
    void BlowFishEncrypt(void* pIn, size_t nInLen);

    // Compress
protected:
    typedef int (CResPacker<Version>::*CompressFn)(
        void*, size_t, void*, size_t&);
    int RawCompress(
        void* pIn, size_t nIn, void* pOut, size_t& nOut)
    {  
        memcpy(pOut,pIn,nIn);
        nOut = nIn;
        return 0;
    }
    int ZipCompress(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut){return 0;};
    int LzmaCompress(
        void* pIn, size_t nIn,
        void* pOut, size_t& nOut );
private:
    eEncryptAlgo m_eAlgo;

    union
    {
        BYTE m_szKey[8];  // 最大密钥8个字节
        QWORD m_dwKey;    // 如果是XOR加密用这个
    };
    eCompressAlgo m_cAlgo;

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

