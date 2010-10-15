/**CPP*************************************************************************
 File            : ResPacker.cpp
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-13  
 Time            : 10:47
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "ResPacker.h"
#include <assert.h>

namespace ResFile
{
//template class CResPacker<File_Version_1_0>;

enum
{
    Raw_File_Buf = 128*1024*1024,  // 64MB
    Res_File_Buf = 64*1024*1024,  // 32MB

    File_Read_Flag = 3,

    Min_Raw_File_Filter = 512,
    Default_Encrypt_Len = 32,
    //Invaild_Pointer = -1, // 0xffffffff
};

template<typename T>
inline void CopyParam(T& TParam, void* pParam)
{
    TParam = *((T*)pParam);
}

IResPacker* CreateResPacker( 
    const char* pResFlodPath,
    eCompressAlgo cAlgo /*= Lzma_C_Algo*/, 
    void* pcParam /*= (void*)&g_DefeParam*/, 
    eEncryptAlgo eAlgo /*= Raw_E_Algo*/,
    void* peParam /*= NULL */ )
{
    if ( isValidString(pResFlodPath) )
    {
        pResFlodPath = "";
    }

    typedef TEncryptParam<File_Version_1_0> EncryptParam;
    typedef TCompressParam<File_Version_1_0> CompressParam;

    
    EncryptParam eParam;
    CompressParam cParam = g_DefcParam;
    if ( eAlgo != Raw_E_Algo )
    {
        if ( NULL != peParam )
        {
            CopyParam(eParam, peParam);
        }
        else
        {
            eAlgo = Raw_E_Algo;
        }
    }

    if ( NULL == pcParam )
    {
        CopyParam(cParam, pcParam);
        cAlgo = Lzma_C_Algo;
    }
    else
    {
        CopyParam(cParam, pcParam);
    }
    
    return new CResPacker<File_Version_1_0>(
        pResFlodPath,
        cAlgo, cParam,
        eAlgo, eParam );
}

void DestroyResPacker( IResPacker*& pResPacker )
{
    safeDelete(pResPacker);
}

template<>
CResPacker<File_Version_1_0>::CResPacker(
    const char* pResFlodPath, 
    eCompressAlgo cAlgo,
    const CompressParam& pcParam,
    eEncryptAlgo eAlgo,
    const EncryptParam& peParam )
    : m_strResFold(pResFlodPath)
    , m_DefeAlgo(eAlgo)
    , m_DefcAlgo(cAlgo)
    , m_DefcParam(pcParam)
    , m_DefeParam(peParam){}

template<>
CResPacker<File_Version_1_0>::~CResPacker(void)
{
}

template<>
void CResPacker<File_Version_1_0>::AddFile(
    const char* pFileName )
{
    if ( isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    Info.eAlgo = m_DefeAlgo;
    Info.eParam = m_DefeParam;
    Info.cAlgo = m_DefcAlgo;
    Info.cParam  = m_DefcParam;
    m_FileInfoList.push_back(Info);
}

template<>
void CResPacker<File_Version_1_0>::AddFile(
    const char* pFileName,
    eCompressAlgo cAlgo, void* pcParam,
    eEncryptAlgo eAlgo, void* peParam )
{
    if ( isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    
    /**
    *@note 如果参数为0，则用默认的
    */
    if ( pcParam == NULL )
    {
        Info.cAlgo = m_DefcAlgo;
        Info.cParam = m_DefcParam;
    }
    else
    {
        Info.cAlgo = cAlgo;
        CopyParam(Info.cParam, pcParam);
        //Info.cParam  = *((CompressParam*)pcParam);
    }

    if ( eAlgo != Raw_E_Algo )
    {
        if ( NULL == peParam )
        {
            CopyParam(Info.eParam, peParam);
            Info.eAlgo = m_DefeAlgo;
        }
        else
        {
            CopyParam(Info.eParam, peParam);
            Info.eAlgo = m_DefeAlgo;
        }
    }
    
    m_FileInfoList.push_back(Info);
}

template<>
void CResPacker<File_Version_1_0>::MakeFile(
    const char* pPackFilePath,
    eFileNamePos eFileNamePos )
{
    /**
    *@note 操作流程，由主线程读文件和写。另外开一个线程专门做数据的转换
    *      首先主线程读一小部分文件就发送一个事件给另外一个线程转换，转换线程转换后写进Res文件缓存
    *      当主线程全部读完后会发一个读完的事件，这个时候转换线程如果发现现有的转换完就退出
    *      主线程发读完事件后会等待转换线程退出，然后将Res文件缓存一次性写进文件
    */
    Init();
    DoRead();
    WaitForSingleObject(m_hTransThread, INFINITE);
    DoWrite(pPackFilePath, eFileNamePos);
    Unit();
}

template<>
DWORD CResPacker<File_Version_1_0>::DataTransform()
{
    //DWORD dwFileHeadSize = Util::GetFileHeadSize<File_Version_1_0>(m_FileInfoList.size());
    FileInfoList::iterator iter = m_FileInfoList.begin();
    BOOL bStopLoop = FALSE;
    while(bStopLoop)
    {
        DWORD dwRc = WaitForMultipleObjects(
            ReadEvent_Count, m_hReadEvent, FALSE, INFINITE);
        switch (dwRc)
        {
        case WAIT_ABANDONED: 
        case WAIT_FAILED:
        case WAIT_TIMEOUT:
            break;	

        case WAIT_OBJECT_0 + ReadSome_Event:
        case WAIT_OBJECT_0 + ReadFinsih_Event:
            for ( ; iter!= m_FileInfoList.end() && iter->pBuf != NULL; ++iter )
            {
                TransformOne(*iter);
            }
    	    break;
 
            if ( dwRc == WAIT_OBJECT_0 + ReadFinsih_Event )
            {
                bStopLoop = TRUE;
            }
        default:
    	    break;
        }
    }
    return 0;
}

template<>
void CResPacker<File_Version_1_0>::Init()
{
    m_CompressFn[Raw_C_Algo] = &CResPacker<File_Version_1_0>::RawCompress;
    m_CompressFn[Zip_C_Algo] = &CResPacker<File_Version_1_0>::ZipCompress;
    m_CompressFn[Lzma_C_Algo] = &CResPacker<File_Version_1_0>::LzmaCompress;

    m_EncryptFn[Raw_E_Algo] = &CResPacker<File_Version_1_0>::RawEncrypt;
    m_EncryptFn[Xor_E_Algo] = &CResPacker<File_Version_1_0>::XorEncrypt;
    m_EncryptFn[BlowFish_E_Algo] = &CResPacker<File_Version_1_0>::BlowFishEncrypt;

    m_pRawFileBuf = new BYTE[Raw_File_Buf];
    m_nRawFileBufUse = 0;
    m_pResFileBuf = new BYTE[Res_File_Buf];
    m_nResFileBufRemain = Res_File_Buf;
    m_pResFileBufNow = m_pResFileBuf;

    for ( int i = 0; i< ReadEvent_Count; ++i )
    {
        m_hReadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    m_hTransThread = CreateThread(
        NULL, 0, 
        &CResPacker<File_Version_1_0>::DataTransform, this,
        0, NULL );
}

template<>
void CResPacker<File_Version_1_0>::Unit()
{
    safeDeleteArray(m_pRawFileBuf);
    safeDeleteArray(m_pResFileBuf);

    for ( int i = 0; i< ReadEvent_Count; ++i )
    {
        CloseHandle(m_hReadEvent[i]);
    }
    CloseHandle(m_hTransThread);
}

template<>
void CResPacker<File_Version_1_0>::DoRead()
{
    BYTE* pRawFileBufNow = m_pRawFileBuf;
    size_t nRawFileBufRemain = Raw_File_Buf;
    BOOL bNeedNew = FALSE;
    int nReadCount = 0;

    FileInfoList::iterator iter = m_FileInfoList.begin();
    FileInfoList::iterator Olditer;
    for ( ; iter!= m_FileInfoList.end(); ++iter )
    {
        FileInfo& TmpFileInfo = *iter;
        string strFilePath = m_strResFold + TmpFileInfo.strFileName;
        FileSystem::CFile Reader;
        if ( ! Reader.OpenByRead(strFilePath.c_str()) )
        {
            string strErr =  "Can`t Open File:";
            strErr += strFilePath;
            throw strErr.c_str();
        }
        
        FileSystem::size_t nFileSize = Reader.GetLength();
        if ( nFileSize > nRawFileBufRemain )
        {
            throw "Tatal File Size obove 128MB";
        }

        FileSystem::size_t nFileBufSize = nRawFileBufRemain;
        if ( nFileSize != Reader.Read((void*)pRawFileBufNow, nFileBufSize) )
        {
            string strErr = "Read File Failed at File";
            strErr += strFilePath;
            throw strErr.c_str();
        }

        TmpFileInfo.pBuf = pRawFileBufNow;
        TmpFileInfo.nBufSize = nFileSize;

        pRawFileBufNow += nFileSize;
        nRawFileBufRemain -= nFileSize;

        ++nReadCount;
        if ( nReadCount == File_Read_Flag )
        {
            SetEvent(m_hReadEvent[ReadSome_Event]);
            nReadCount = 0;
        }  
    }
}

template<>
void CResPacker<File_Version_1_0>::DoWrite(
    const char* pPackFilePath,
    eFileNamePos FileNamePos )
{
    FileNamePos = Not_Exist;  //(暂时不考虑写文件名)
    /**
    *@note 1. Create File
    */
    FileSystem::CFile Writer;
    Writer.OpenByWrite(pPackFilePath);
    if ( !Writer.IsOpen() )
    {
        string strErr = "Can`t Open ";
        strErr += pPackFilePath;
        throw strErr.c_str();
    }

    size_t nResDataSize = Res_File_Buf - m_nResFileBufRemain;

    /**
    *@note 2. Write File Head 
    */
    size_t nHeadSize = Util::GetFileHeadSize<File_Version_1_0>(m_FileInfoList.size());
    TFileHeadBase HeadBase;
    HeadBase.dwSize = nHeadSize + nResDataSize;
    HeadBase.FormatFlag = File_Format_Flag;
    HeadBase.Version = File_Version_1_0;
    HeadBase.dwFileCount = m_FileInfoList.size();
    HeadBase.nFileNameFlag = FileNamePos;
    Writer.Write(&HeadBase, sizeof(TFileHeadBase));

    typedef TDataMemInfo<File_Version_1_0> DataMemInfo;
    DWORD dwRawDataMem[DataMemInfo::Max_Num];          
    DWORD dwCompressDataMem[DataMemInfo::Max_Num];
    Writer.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
    Writer.Write((void*)dwCompressDataMem, sizeof(dwCompressDataMem));

    typedef TFileHead<File_Version_1_0>::TDataIndex DataIndex;
    
    DataIndexList<File_Version_1_0>::iterator iter;
    for ( iter = m_DataIndexList.begin();
          iter!= m_DataIndexList.end();
          ++iter )
    {
        DataIndex& Index = *iter;
        Index.Info.dwDataOffset += nHeadSize;
        Writer.Write((void*)&Index, sizeof(DataIndex));
    }

    /**
    *@note 3. Write File Data
    */
    Writer.Write(m_pResFileBufNow, nResDataSize);
    Writer.Close();
}

template<>
void CResPacker<File_Version_1_0>::TransformOne(
    FileInfo& Info )
{
    typedef TDataHead<File_Version_1_0> DataHead;
    DWORD dwDataOffset = m_pResFileBufNow - m_pResFileBuf;

    /**
    *@note 1. 处理Filter
    */
    if ( Min_Raw_File_Filter > Info.nBufSize )
    {
        Info.cAlgo = Raw_C_Algo;
    }
    
    /**
    *@note 2. 填数据头
    */
    DataHead* pDataHead = (DataHead*)m_pResFileBufNow;
    pDataHead->dwRawDataLen = Info.nBufSize;
    //pDataHead->nEncryptAlgo = Info.eAlgo;
    pDataHead->nCompressAlgo = Info.cAlgo;
    pDataHead->nCompressLevel = Info.cParam.cParam;
    //pDataHead->nDataEncryptLen = Default_Encrypt_Len;
    pDataHead->eParam = Info.eParam;
    m_pResFileBufNow += sizeof(DataHead);
    m_nResFileBufRemain -= sizeof(DataHead);

    /**
    *@note 3. 压缩
    */
    CompressFn pCompressFn = m_CompressFn[Info.cAlgo];
    size_t nPackLen = m_nResFileBufRemain;
    int nRc = (this->*pCompressFn)(
        Info.pBuf, Info.nBufSize,
        (void*)m_pResFileBufNow, nPackLen, Info.cParam);
    assert(nRc != 0);
    if ( nRc != 0 )
    {
        throw "Compress Failed!";
    }
    void* pRawEncryptBuf = m_pResFileBufNow;
    m_pResFileBufNow += nPackLen;
    m_nResFileBufRemain -= nPackLen;

    /**
    *@note 4. 加密
    */
    if ( nPackLen < Default_Encrypt_Len && Info.eAlgo != Raw_E_Algo )
    {
        pDataHead->nEncryptAlgo = Info.eAlgo = Raw_E_Algo;
        pDataHead->nDataEncryptLen = 0;
    }
    else
    {
        pDataHead->nEncryptAlgo = Info.eAlgo;
        pDataHead->nDataEncryptLen = Default_Encrypt_Len;
    }
    EncryptFn pEncryptFn = m_EncryptFn[Info.eAlgo];
    (this->*pEncryptFn)(pRawEncryptBuf, Default_Encrypt_Len, Info.eParam);
 
    /**
    *@note 5. 添加数据索引
    */
    TFileHead<File_Version_1_0>::TDataIndex Index;
    Index.HashValue = OCI::HashStringEx(Info.strFileName.c_str());
    Index.Info.dwDataOffset = dwDataOffset;
    Index.Info.dwDataLen = nPackLen + sizeof(DataHead);
    m_DataIndexList.insert(Index);
}

template<>
int CResPacker<File_Version_1_0>::LzmaCompress(
    void* pIn, size_t nIn,
    void* pOut, size_t& nOut,
    const CResPacker<File_Version_1_0>::CompressParam& p )
{
    return LzmaUtil::LzmaCompress(
        (unsigned char*)pOut, &nOut,
        (const unsigned char*)pIn, nIn, p.cParam);
}

}



//namespace GG
//{
//    typedef unsigned int size_t; 
//
//    typedef unsigned char size_t; 
//}
//
//class JJ
//{
//
//};






// End of file