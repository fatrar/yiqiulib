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
    public list<TFileHead<File_Version_1_1>::TDataIndex>{};

template<> class DataIndexList<File_Version_1_2> :
    public set<TFileHead<File_Version_1_0>::TDataIndex>{};
template<> class DataIndexList<File_Version_1_3> :
    public set<TFileHead<File_Version_1_0>::TDataIndex>{};

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

    // IResPacker
public:
    virtual void AddFile(
        const char* pFileName );

    virtual void MakeFile(
        const char* pPackFilePath,
        size_t nVolumeSize,
        bool bIsExistFileName );

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

        //FileInfo(const FileInfo& a)
        //{
        //    strFileName = a.strFileName;
        //    pRawDataBuf = a.pRawDataBuf;
        //    nRawDataSize = a.nRawDataSize;
        //    nPackDataSize = a.nPackDataSize;
        //}
    };

    typedef list<FileInfo> FileInfoList;
    typedef TFileHead<Version> FileHead;
    typedef TDataHead<Version> DataHead;

protected:
    static unsigned int WINAPI DataTransform(void* p)
    {
        return ((CResPacker*)p)->DataTransform();
    }
    unsigned int DataTransform();

    void Init();
    void InitFileName();
    void Unit();
    void DoRead();
    void ShowLog();
    void DoWrite(const char* pPackFilePath);

    void TransformOne(FileInfo& Info);
    inline void MakePath(
        const string& strFileName,
        string& strFilePath);

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

protected:
    enum
    {
        Raw_File_Buf = 256*1024*1024,  // 64MB
        Res_File_Buf = 32*1024*1024,  // 32MB

        File_Read_Flag = 1,

        Invaild_Pointer = -1, // 0xffffffff
    };

private:
    eEncryptAlgo m_eAlgo;

    union
    {
        BYTE m_szKey[8];  // 最大密钥8个字节
        QWORD m_qwKey;    // 如果是XOR加密用这个
    };
    eCompressAlgo m_cAlgo;

    string m_strResFold;

    FileInfoList m_FileInfoList;
    DataIndexList<Version> m_DataIndexList;

    //
    // Thread
    //
    HANDLE m_hTransThread;
    size_t m_nThreadID;
    enum Thread_Msg_Def {
        Msg_Read_Some = WM_USER + 1,
        Msg_Just_Test,  
        Msg_Read_Finsih,
    };
    
    BYTE* m_pRawFileBuf;
    size_t m_nRawFileBufUse;

    BYTE* m_pResFileBuf;
    BYTE* m_pResFileBufNow;
    size_t m_nResFileBufRemain;

    size_t m_nFileAllDataSize;  // 记录压缩之后的数据总大小

    size_t m_nIndexCount;
    size_t m_nVolumeSize;
    bool m_bIsExistFileName;
    size_t m_nFileNameSize;
    BYTE* m_pFileNameBuf;

    CompressFn m_CompressFn[Compress_Count];
    Util::IResCrypto* m_pResCrypto;
};

extern string g_strErr;


}




#endif  // _RESPACKER_H_2010_


// End of file

