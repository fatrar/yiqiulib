/*H***************************************************************************
 File            : IVDataBuf.cpp
 Subsystem       : 
 Function Name(s): CIVDataBuf
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 14:25
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVDataBuf.h"
#include "ChannelTarget.h"


CIVDataBuf::CIVDataBuf(void)
    : m_nLastPos(0)
    , m_IsInit(FALSE)
    , m_pTargetBuf(NULL)
    , m_Event(NULL)
    , m_Thread(NULL)
    , m_nPreAlarmTime()
{
    DebugOut("CIVDataBuf() \n");
    memory_check.Init("IV Viewer");
}

CIVDataBuf::~CIVDataBuf(void)
{
    DebugOut("~CIVDataBuf() \n");
    if ( m_IsInit )
    {
        Unit();
    }
}

//
// IIVDataBuf
//
TargetQueue* CIVDataBuf::GetData(
    int nChannelID,
    const FILETIME& time )
{ 
//#ifdef _DEBUG
//    SYSTEMTIME syt;
//    FileTimeToSystemTime(&time, &syt);
//    char szbuf[64] = {0};
//    sprintf_s(
//        szbuf, "Get IV Data Time =%d:%d:%d.%d\n", 
//        syt.wHour, syt.wMinute,
//        syt.wSecond, syt.wMilliseconds);
//    OutputDebugString(szbuf);
//#endif

    if ( !m_IsInit )
    {
        return NULL;
    }

    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    GroupTarget* pGroupTarget = NULL;
   
    {
        AutoLockAndUnlock(m_cs);
        pGroupTarget = ChanTarget.Find(time);
    }
    if ( NULL == pGroupTarget )
    {
        return FALSE;
    }

    pGroupTarget->m_TargetQueue->AddRef();
    return pGroupTarget->m_TargetQueue;
}

//
// IIVDataSender
//
BOOL CIVDataBuf::OnIVDataSend( 
    int nChannelID,
    const FILETIME& time,
    const WPG_Target* pData,
    size_t nLen )
{  
    if ( !m_IsInit )
    {
        return FALSE;
    }

    if ( 0 == nLen )
    {
        return TRUE;
    }
    
    /**
    *@note 1. 找是否有目标数据缓存(TargetQueue)
    */
    int nBufPos = FindBuf();
    if ( nBufPos == NO_BUF_Ramain )
    {
        DebugOut("CIVDataBuf::OnIVDataSend No Find Buf!\n");
        return FALSE;
    }

    /**
    *@note 2. 对TargetQueue设置使用标志，然后将数据指针传给GroupTarget，
    * 由他去加载新的数据到TargetQueue
    */
    TargetQueue* pTemp = &m_pTargetBuf[nBufPos];
    pTemp->Use(GetMyWantEvent(SaveFile_Event,nChannelID));
    GroupTarget* pGroupTarget = new GroupTarget(
        time, pData, nLen, pTemp);
   
    /**
    *@note 3. 将数据插入队列，等待外部取这个数据进行显示
    */
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    AutoLockAndUnlock(m_cs);
    ChanTarget.PushBack(pGroupTarget);
    return TRUE;
}

BOOL CIVDataBuf::Init(
    int nDeviceCount,
    int nEveryDeviceChannelNum)
{
    if ( m_IsInit )
    {
        return TRUE;
    }

    m_TargetMap = new ChannelTarget[nDeviceCount*nEveryDeviceChannelNum];

    m_dwMaxBufCount = Single_Device_Buf_Size*nDeviceCount;
    m_pTargetBuf = new TargetQueue[m_dwMaxBufCount];

    m_nDeviceCount = nDeviceCount;
    m_nEveryDeviceChannelNum = nEveryDeviceChannelNum;

    // 第一个是为设置事件要他结束线程，
    // Save File / open / close * nDeviceCount
    int nEventCount = GetEventCount();
    m_Event = new HANDLE[nEventCount];
    for ( int i=0; i<nEventCount; ++i )
    {
        m_Event[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
    }

    m_Thread = (HANDLE)_beginthreadex(
        NULL, 0, 
        CIVDataBuf::SaveFileThread,
        this,
        0,
        &m_nThreadID);
    return m_IsInit=TRUE;
}

BOOL CIVDataBuf::Unit()
{
    if ( !m_IsInit )
    {
        return FALSE;
    }
  
    SetEvent(m_Event[0]);
    if ( m_Thread )
    {
        WaitForSingleObject(m_Thread, -1);
        CloseHandle(m_Thread); m_Thread = NULL;
    }
    
    int nEventCount = GetEventCount();
    for ( int i=0; i<nEventCount; ++i )
    {
        CloseHandle(m_Event[i]); m_Event[i] = NULL;
    }
    delete[] m_Event; m_Event = NULL;

    delete[] m_pTargetBuf; m_pTargetBuf = NULL;
    delete[] m_TargetMap; m_TargetMap =NULL;
    m_IsInit = FALSE;
    return TRUE;
}

//
// IIVDataSaver
//
BOOL CIVDataBuf::Open(
    int nChannelID,
    const char* pPath,
    const FILETIME& time )
{
    if ( !m_IsInit ||
         !isValidString(pPath) )
    {
        return FALSE;
    }

    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    ChanTarget.NewFileComing(pPath, time);
    return SetEvent(
        GetMyWantEvent(OpenFile_Event,nChannelID) );
}

BOOL CIVDataBuf::Close(
    int nChannelID, 
    const FILETIME& time )
{
    if ( !m_IsInit )
    {
        return FALSE;
    }

    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    if ( !ChanTarget.FileClose(time) )
    {
        return FALSE;
    }

    return SetEvent(
        GetMyWantEvent(CloseFile_Evnet,nChannelID) );
}

BOOL CIVDataBuf::DeleteIVFile( 
    const char* pPath )
{
    if ( !m_IsInit ||
         !isValidString(pPath) )
    {
        return FALSE;
    }

    // 这里要判断文件是否在读。 PlayBack
    // [x] 这个时候按道理视频文件也在读，所以依赖那边就好。当删除不了时，会再次删除
    string strPath = pPath;
    strPath += c_szIVFileExt;
    return DeleteFile(strPath.c_str());
}

BOOL CIVDataBuf::TellPreAlarmTime( int time )
{
    m_nPreAlarmTime = time;
    return TRUE;
}

size_t WINAPI CIVDataBuf::SaveFileThread( void* pParm )
{
    return ((CIVDataBuf*)pParm)->SaveFileLoopFun();
}

size_t CIVDataBuf::SaveFileLoopFun()
{
    DWORD dwEventCount = GetEventCount();
    while (1)
    {
        DWORD dwRc = WaitForMultipleObjects(
            dwEventCount, m_Event, FALSE, -1 );
        switch (dwRc)
        {
        case WAIT_ABANDONED:
        case WAIT_FAILED:
            break;	
        case WAIT_TIMEOUT:
        	break;
        case WAIT_OBJECT_0:
            return 0;
        default:
            DWORD dwOffsetValue = dwRc - (WAIT_OBJECT_0 +1);
            DWORD dwEventID = dwOffsetValue%(m_nDeviceCount*m_nEveryDeviceChannelNum);
            //DWORD dwDeviceID = dwOffsetValue%(_EventTypeCount*m_nEveryDeviceChannelNum);
            DWORD dwChannelID = dwOffsetValue%(Event_Count);
            if ( dwChannelID >= m_nEveryDeviceChannelNum )
            {
                // log
                assert(false);
                break;
            }

            switch (dwEventID)
            {
            case SaveFile_Event:
                //ResetEvent()
                DoSaveFileEvent(dwChannelID);
                break;
            case OpenFile_Event:
                DoOpenFileEvevt(dwChannelID);
                break;
            case CloseFile_Evnet:
                DoCloseFileEvent(dwChannelID);
                break;
            default:
                // log
                break;
            }
        }
    }
    
    //MSG msg;
    //PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    //if(!SetEvent(h))
    //{
    //    // log("set event error,%d\n",GetLastError());
    //    return 1;
    //}

    //while(true)
    //{
    //    if ( !GetMessage(&msg,0,0,0) )
    //    {
    //        continue;
    //    }
    // 
    //    switch(msg.message)
    //    {
    //    case :
    //      break;
    //    }
    //}
    return 0;
}

int CIVDataBuf::FindBuf()
{
    // 算法需要检验是否正确
    for (WORD i=m_nLastPos; i<m_dwMaxBufCount; ++i)
    {
        if ( m_pTargetBuf[i].nUse == Buf_No_Use )
        {
            // 好像m_nLastPos为MAX_IV_BUF_Size也没问题
            m_nLastPos = i + 1;
            return i;
        }
    }

    for (WORD j=0; j<m_nLastPos; ++j)
    {
        if ( m_pTargetBuf[j].nUse == Buf_No_Use )
        {
            m_nLastPos = j + 1;
            return j;
        }
    }

    // log
    return NO_BUF_Ramain;
}

void CIVDataBuf::DoSaveFileEvent(DWORD dwChannel)
{
    ChannelTarget& ChanTarget = m_TargetMap[dwChannel];
    if ( ChanTarget.TargetSaveList.size() == 0 )
    {
        return;
    }
   
    ChanTarget.TrySaveData(m_nPreAlarmTime);
}

void CIVDataBuf::DoOpenFileEvevt( DWORD dwChannel )
{

}

void CIVDataBuf::DoCloseFileEvent( DWORD dwChannel )
{

}

HANDLE CIVDataBuf::GetMyWantEvent( IVBufEvent e, int nChannelID )
{
    return m_Event[1+Event_Count*nChannelID + e];
}

DWORD CIVDataBuf::GetEventCount()
{
    static DWORD s_nEventCount = 1+m_nDeviceCount*m_nEveryDeviceChannelNum*Event_Count;
    return s_nEventCount; 
}




IIVDataSaver* IVLiveFactory::GetDataSaver(void)
{
    return CIVDataBuf::getInstancePtr();
}

IIVDataSender* IVLiveFactory::GetDataSender(void)
{
    return CIVDataBuf::getInstancePtr();
}


// End of file