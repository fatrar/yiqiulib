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
    Raw_File_Buf = 64*1024*1024,  // 64MB
    Res_File_Buf = 32*1024*1024,  // 32MB

    File_Read_Flag = 3,

    //Invaild_Pointer = -1, // 0xffffffff
};

template<typename T>
inline void CopyParam(T& TParam, void* pParam)
{
    TParam = *((T*)pParam);
}

IResPacker* CreateResPacker( 
    const char* pResFlodPath,
    eCompressAlgo cAlgo /*= LZMA2_C_Algo*/, 
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
        cAlgo = LZMA2_C_Algo;
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
bool CResPacker<File_Version_1_0>::MakeFile(
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
    DoWrite();
    Unit();
    return true;
}

// Thread

template<>
DWORD CResPacker<File_Version_1_0>::DataTransform()
{
    DWORD dwFileHeadSize = Util::GetFileHeadSize(m_FileInfoList.size());
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
    m_pRawFileBuf = new BYTE[Raw_File_Buf];
    m_nRawFileBufUse = 0;
    m_pResFileBuf = new BYTE[Res_File_Buf];
    m_nResFileBufUse = 0;

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
    for ( ; iter!= m_FileInfoList.end();  )
    {
        FileInfo& TmpFileInfo = *iter;
        string strFilePath = m_strResFold + TmpFileInfo.strFileName;
        if ( bNeedNew )
        {
            void* pFileData = NULL;
            size_t nRead = FileSystem::CFile::Read(
                strFilePath.c_str(), pFileData);
            if ( nRead == 0 )
            {
                cout << "Can`t Open File:" << strFilePath.c_str() << endl;
                Olditer = iter;
                ++iter;
                m_FileInfoList.erase(Olditer);
                continue;
                // [] Todo: Exception          
            }
            else
            {
                TmpFileInfo.pBuf = (BYTE*)pFileData;
                TmpFileInfo.nBufSize = nRead;
                TmpFileInfo.bNeedDelete = TRUE;
            }  
        }
        else
        {
            FileSystem::size_t nFileBufSize = nRawFileBufRemain;
            size_t nRead = FileSystem::CFile::Read(
                strFilePath.c_str(), (void*)pRawFileBufNow, nFileBufSize);
            if ( nRead == 0 )
            {
                if ( nFileBufSize == 0 )
                {
                    cout << "Can`t Open File:" << strFilePath.c_str() << endl;
                    // [] Todo: Exception
                    Olditer = iter;
                    ++iter;
                    m_FileInfoList.erase(Olditer);
                    continue;
                }
                else
                {
                    bNeedNew = TRUE;
                    continue;
                }
            }
            else
            {
                TmpFileInfo.pBuf = pRawFileBufNow;
                TmpFileInfo.nBufSize = nRead;
                TmpFileInfo.bNeedDelete = FALSE;

                pRawFileBufNow += nRead;
                nRawFileBufRemain -= nRead;
            }         
        }
        ++nReadCount;
        if ( nReadCount == File_Read_Flag )
        {
            SetEvent(m_hReadEvent[ReadSome_Event]);
            nReadCount = 0;
        }
        ++iter;    
    }
}

template<>
void CResPacker<File_Version_1_0>::DoWrite()
{

}

template<>
void CResPacker<File_Version_1_0>::TransformOne(
    FileInfo& Info )
{

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