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
//#include <assert.h>
#include "ResPacker_1_0.cpp"
#include "ResPacker_1_1.cpp"

namespace ResFile
{
//template class CResPacker<File_Version_1_0>;

template class CResPacker<File_Version_1_0>;
template class CResPacker<File_Version_1_1>;

string g_strErr = "";

template<typename T>
inline void CopyParam(T& TParam, void* pParam)
{
    TParam = *((T*)pParam);
}

IResPacker* CreateResPacker( 
   const char* pResFlodPath, 
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo )
{
    if ( NULL == pResFlodPath )
    {
        pResFlodPath = "";
    }
    
    return new CResPacker<File_Version_1_0>(
        pResFlodPath, 
        eAlgo,szKey, cAlgo );
}

void DestroyResPacker( IResPacker*& pResPacker )
{
    safeDelete(pResPacker);
}

template<DWORD Version>
CResPacker<Version>::CResPacker(
    const char* pResFlodPath,
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo )
    : m_strResFold(pResFlodPath)
    , m_eAlgo(eAlgo)
    , m_cAlgo(cAlgo)
    , m_nRawAllDataSize(0)
    , m_nFileNameSize(0)
    , m_pFileNameBuf(NULL)
{
    memcpy(m_szKey, szKey, 8);
}

template<DWORD Version>
void CResPacker<Version>::AddFile(
    const char* pFileName )
{
    if ( !isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    m_nFileNameSize += Info.strFileName.length() + 1;
    m_FileInfoList.push_back(Info);
}

template<DWORD Version>
void CResPacker<Version>::MakeFile(
    const char* pPackFilePath,
    size_t nVolumeSize,
    bool bIsExistFileName )
{
    m_nVolumeSize = nVolumeSize;
    m_bIsExistFileName = bIsExistFileName;
    /**
    *@note 操作流程，由主线程读文件和写。另外开一个线程专门做数据的转换
    *      首先主线程读一小部分文件就发送一个事件给另外一个线程转换，转换线程转换后写进Res文件缓存
    *      当主线程全部读完后会发一个读完的事件，这个时候转换线程如果发现现有的转换完就退出
    *      主线程发读完事件后会等待转换线程退出，然后将Res文件缓存一次性写进文件
    */
    Init();
    DoRead();
    WaitForSingleObject(m_hTransThread, INFINITE);
    DoWrite(pPackFilePath);
    ShowLog();
    Unit();
}

template<DWORD Version>
unsigned int CResPacker<Version>::DataTransform()
{
    FileInfoList::iterator iter = m_FileInfoList.begin();
    volatile BOOL bLoop = TRUE;
    while(bLoop)
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
            for ( ; iter!= m_FileInfoList.end() && iter->pRawDataBuf != NULL; ++iter )
            {
                if ( iter->pRawDataBuf != Invaild_Pointer )
                {
                    TransformOne(*iter);
                }
            }
    	   
            if ( dwRc == WAIT_OBJECT_0 + ReadFinsih_Event )
            {
                bLoop = FALSE;
            }
            break;
        default:
    	    break;
        }
    }
    return 0;
}

template<DWORD Version>
void CResPacker<Version>::Init()
{
    m_CompressFn[Raw_C_Algo] = &CResPacker<Version>::RawCompress;
    m_CompressFn[Zip_C_Algo] = &CResPacker<Version>::ZipCompress;
    m_CompressFn[Lzma_C_Algo] = &CResPacker<Version>::LzmaCompress;

    m_EncryptFn[Raw_E_Algo] = &CResPacker<Version>::RawEncrypt;
    m_EncryptFn[Xor_E_Algo] = &CResPacker<Version>::XorEncrypt;
    m_EncryptFn[BlowFish_E_Algo] = &CResPacker<Version>::BlowFishEncrypt;

    m_pRawFileBuf = new BYTE[Raw_File_Buf];
    m_nRawFileBufUse = 0;
    m_pResFileBuf = new BYTE[Res_File_Buf];
    m_nResFileBufRemain = Res_File_Buf;
    m_pResFileBufNow = m_pResFileBuf;

    if ( m_bIsExistFileName )
    {
        InitFileName();
    }

    for ( int i = 0; i< ReadEvent_Count; ++i )
    {
        m_hReadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    m_hTransThread = (HANDLE)_beginthreadex(
        NULL, 0, 
        &CResPacker<Version>::DataTransform, this,
        0, NULL );
}

template<DWORD Version>
void CResPacker<Version>::Unit()
{
    safeDeleteArray(m_pRawFileBuf);
    safeDeleteArray(m_pResFileBuf);
    safeDeleteArray(m_pFileNameBuf);

    for ( int i = 0; i< ReadEvent_Count; ++i )
    {
        CloseHandle(m_hReadEvent[i]);
    }
    CloseHandle(m_hTransThread);
}

//template<>
//void CResPacker<File_Version_1_0>::DoRead()
//{
//    BYTE* pRawFileBufNow = m_pRawFileBuf;
//    size_t nRawFileBufRemain = Raw_File_Buf;
//    int nReadCount = 0;
//    string strFilePath;
//
//    FileInfoList::iterator iter = m_FileInfoList.begin();
//    if ( m_bIsExistFileName )
//    {
//        ++iter; // 第一个为已经读好数据的文件名
//    }
//    for ( ; iter!= m_FileInfoList.end(); ++iter )
//    {
//        FileInfo& TmpFileInfo = *iter;
//
//        /**
//        *@note 1. Get Raw Res File Path
//        */
//        string& strTmpPath = TmpFileInfo.strFileName;
//        if ( m_strResFold.size() == 0 )
//        {
//            strFilePath = strTmpPath;
//        }
//        else if ( m_strResFold[m_strResFold.size()-1] == '\\' ||
//                  m_strResFold[m_strResFold.size()-1] == '/' )
//        {
//            strFilePath = m_strResFold + strTmpPath;
//        }
//        else
//        {
//            strFilePath = m_strResFold + "/" + strTmpPath;
//        }
//        
//        /**
//        *@note 2. Open File And Test File Exist And Buffer is enough
//        */
//        FileSystem::CFile Reader;
//        if ( ! Reader.OpenByRead(strFilePath.c_str()) )
//        {
//            g_strErr =  "Can`t Open File:";
//            g_strErr += strFilePath;
//            throw g_strErr.c_str();
//        }
//        FileSystem::size_t nFileSize = Reader.GetLength();
//        if ( nFileSize == 0 ||
//             nFileSize > nRawFileBufRemain )
//        {
//            throw "Tatal File Size obove 128MB or File Size is 0";
//        }
//
//        /**
//        *@note 3. Read All File Data To Buffer 
//        */
//        FileSystem::size_t nFileBufSize = nRawFileBufRemain;
//        if ( nFileSize != Reader.Read((void*)pRawFileBufNow, nFileBufSize) )
//        {
//            g_strErr = "Read File Failed at File " + strFilePath;
//            throw g_strErr.c_str();
//        }
//
//        /**
//        *@note 4. Set Info for Data Compress and Save To File
//        */
//        TmpFileInfo.pRawDataBuf = pRawFileBufNow;
//        TmpFileInfo.nRawDataSize = nFileSize;
//
//        /**
//        *@note 5. Refresh Buffer Point and Buffer Remain size
//        */
//        pRawFileBufNow += nFileSize;
//        nRawFileBufRemain -= nFileSize;
//
//        /**
//        *@note 6. Test is Notify DataTransform Thread of Compress Data
//        */
//        ++nReadCount;
//        if ( nReadCount == File_Read_Flag )
//        {
//            SetEvent(m_hReadEvent[ReadSome_Event]);
//            nReadCount = 0;
//        }
//    }
//
//    /**
//    *@note Notify DataTransform Thread of Raw Res File Data Read Finish
//    */
//    SetEvent(m_hReadEvent[ReadFinsih_Event]);
//}
//
//template<>
//void CResPacker<File_Version_1_0>::DoWrite(
//    const char* pPackFilePath )
//{
//    /**
//    *@note 1. Create File
//    */
//    FileSystem::CFile Writer;
//    Writer.OpenByWrite(pPackFilePath);
//    if ( !Writer.IsOpen() )
//    {
//        g_strErr = string("Can`t Open ") + pPackFilePath;
//        throw g_strErr.c_str();
//    }
//
//    /**
//    *@note 2. Write File Head 
//    */
//    size_t nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
//        Writer, m_FileInfoList.size(),
//        m_bIsExistFileName, m_eAlgo, m_szKey );
//       
//    // 这个部分我暂时不用
//    DWORD dwRawDataMem[TDataMemInfo::Max_Num] = {0};
//    DWORD dwCompressDataMem[TDataMemInfo::Max_Num] = {0};
//    Writer.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
//    Writer.Write((void*)dwCompressDataMem, sizeof(dwCompressDataMem));
//
//    typedef TFileHead<File_Version_1_0>::TDataIndex DataIndex;
//    DataIndexList<File_Version_1_0>::iterator iter;
//    for ( iter = m_DataIndexList.begin();
//          iter!= m_DataIndexList.end();
//          ++iter )
//    {
//        /**
//        *@note 原先记录只是数据本身之间的，实际的需要加文件头
//        */
//        DataIndex& Index = *iter;
//        Index.dwOffset += nHeadSize;
//        Writer.Write((void*)&Index, sizeof(DataIndex));
//    }
//
//    /**
//    *@note 3. Write File Data
//    */
//    size_t nResDataSize = Res_File_Buf - m_nResFileBufRemain;
//    Writer.Write(m_pResFileBuf, nResDataSize);
//
//    Writer.Close();
//}

template<DWORD Version>
int CResPacker<Version>::LzmaCompress(
    void* pIn, size_t nIn,
    void* pOut, size_t& nOut )
{
    return LzmaUtil::LzmaCompress(
        (unsigned char*)pOut, &nOut,
        (const unsigned char*)pIn, nIn, Compress_High);
}

template<DWORD Version>
void CResPacker<Version>::BlowFishEncrypt(
    void* pIn, size_t nIn )
{
    if ( nIn < Default_Encrypt_Len )
    {
        return;
    }
}

template<DWORD Version>
void CResPacker<Version>::XorEncrypt(
    void* pIn, size_t nIn )
{
    if ( nIn < Default_Encrypt_Len )
    {
        return;
    }

    QWORD* pStart = (QWORD*)pIn;
    for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
    {
        *pStart ^= m_dwKey;
        ++pStart;
    }
}

template<DWORD Version>
void CResPacker<Version>::ShowLog()
{
    FileInfoList::iterator iter = m_FileInfoList.begin();
    for ( int i = 1; iter!= m_FileInfoList.end(); ++iter,++i )
    {
        const FileInfo& Info = *iter;
        cout << '[' << i << "] " 
            << Info.strFileName.c_str() << " : " 
            << Info.nRawDataSize << "-->"
            << Info.nPackDataSize << "   Ratio=" 
            << Info.nPackDataSize*100.0/Info.nRawDataSize << '%' << endl;
    }
    cout << "File Head size=" 
        << Util::GetFileHeadSize<File_Version_1_0>(m_DataIndexList.size())
        << "\n All Data size = " << DWORD(m_pResFileBufNow- m_pResFileBuf)
        << "\n Data size = " << m_nRawAllDataSize << endl;  
}

}



// End of file