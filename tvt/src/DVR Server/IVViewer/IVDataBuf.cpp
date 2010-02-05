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


struct ThreadParm
{
    ThreadParm(CIVDataBuf* p, HANDLE handle):pthis(p),h(handle){}
    CIVDataBuf* pthis;
    HANDLE h;
};

enum ThreadMsg
{
    On_Send_Data,

    On_End_Thread,
};

enum
{
    _SaveFileEvent = 0,
    _OpenFileEvent = 1,
    _CloseFileEvnet = 2,
};


static const char c_szIVFileExt[] = ".iv";

#define _EventTypeCount 3
#define _GetEventCount(d, c) (1+d*c*_EventTypeCount)

CIVDataBuf::CIVDataBuf(void)
    : m_nLastPos(0)
    , m_nBufRemain(MAX_IV_BUF_Size)
    , m_IsInit(FALSE)
    , m_pTargetBuf(NULL)
    , m_Event(NULL)
    , m_Thread(NULL)
{
}

CIVDataBuf::~CIVDataBuf(void)
{
    if ( m_IsInit )
    {
        Unit();
        m_IsInit = FALSE;
    }
}

//
// IIVDataBuf
//
TargetQueue* CIVDataBuf::GetData(
    int nChannelID,
    const FILETIME& time )
{ 
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
    SetEvent(m_Event[1+nChannelID]);
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

    if ( 0 ==  nLen )
    {
        return TRUE;
    }

    int nBufPos = FindBuf();
    if ( nBufPos == NO_BUF_Ramain )
    {
        return FALSE;
    }

    GroupTarget* pGroupTarget = new GroupTarget(
        time, pData, nLen, &m_pTargetBuf[nBufPos]);
   
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    AutoLockAndUnlock(m_cs);
    ChanTarget.PushBack(pGroupTarget);
    return TRUE;
}

BOOL CIVDataBuf::Init(int nDeviceCount,int nEveryDeviceChannelNum)
{
    if ( m_IsInit )
    {
        return TRUE;
    }

    m_TargetMap = new ChannelTarget[nDeviceCount*nEveryDeviceChannelNum];
    m_pTargetBuf = new TargetQueue[MAX_IV_BUF_Size];

    m_nDeviceCount = nDeviceCount;
    m_nEveryDeviceChannelNum = nEveryDeviceChannelNum;

    // 第一个是为设置事件要他结束线程，Save File / open / close * nDeviceCount
    int nEventCount = _GetEventCount(nDeviceCount, nEveryDeviceChannelNum);
    m_Event = new HANDLE[nEventCount];
    for ( int i=0; i<nEventCount; ++i )
    {
        m_Event[i] = CreateEvent(NULL,TRUE,FALSE,NULL);
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
    int nEventCount = _GetEventCount(m_nDeviceCount, m_nEveryDeviceChannelNum);
    if ( m_Thread )
    {
        WaitForSingleObject(m_Thread, -1);
        CloseHandle(m_Thread); m_Thread = NULL;
    }
    
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

//BOOL CIVDataBuf::ShowObjTrace(int nChannelID,  bool bState )
//{
//    return TRUE;
//}
//
//BOOL CIVDataBuf::GetObjTraceState(int nChannelID,  bool& bState )
//{
//    return TRUE;
//}

//
// IIVDataSaver
//
BOOL CIVDataBuf::Open(
    int nChannelID,
    const char* pPath,
    const FILETIME& time )
{
    if ( !m_IsInit )
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CIVDataBuf::Close(
    int nChannelID, 
    const FILETIME& time )
{
    if ( !m_IsInit )
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CIVDataBuf::DeleteIVFile( 
    const char* pPath )
{
    if ( !m_IsInit )
    {
        return FALSE;
    }

    if ( !isValidString(pPath) )
    {
        return FALSE;
    }

    // 这里要判断文件是否在读。 PlayBack
    // [x] 这个时候按道理视频文件也在读，所以依赖那边就好。当删除不了时，会再次删除
    string strPath = pPath;
    strPath += pPath;
    DeleteFile(strPath.c_str());
    return TRUE;
}

BOOL CIVDataBuf::TellPreAlarmTime( int time )
{
    return TRUE;
}

size_t WINAPI CIVDataBuf::SaveFileThread( void* pParm )
{
    return ((CIVDataBuf*)pParm)->SaveFileLoopFun(NULL);
}

size_t CIVDataBuf::SaveFileLoopFun(HANDLE h)
{
    BOOL bExit = FALSE;
    while (bExit)
    {
        DWORD dwRc = WaitForMultipleObjects(
            1+m_nDeviceCount*3,
            m_Event, FALSE, -1 );
        switch (dwRc)
        {
        case WAIT_ABANDONED:
        case WAIT_FAILED:
            break;	
        case WAIT_TIMEOUT:
        	break;
        case WAIT_OBJECT_0:
            bExit = TRUE;
        	break;
        default:
            DWORD dwOffsetValue = dwRc - (WAIT_OBJECT_0 +1);
            DWORD dwEventID = dwOffsetValue%(m_nDeviceCount*m_nEveryDeviceChannelNum);
            //DWORD dwDeviceID = dwOffsetValue%(_EventTypeCount*m_nEveryDeviceChannelNum);
            DWORD dwChannelID = dwOffsetValue%(_EventTypeCount);
            if ( dwChannelID >= m_nEveryDeviceChannelNum )
            {
                // log
                break;
            }

            switch (dwEventID)
            {
            case _SaveFileEvent:
                DoSaveFileEvent(dwChannelID);
                break;
            case _OpenFileEvent:
                DoOpenFileEvevt(dwChannelID);
                break;
            case _CloseFileEvnet:
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
   /* if ( m_nBufRemain == 0 )
    {
        return NO_BUF_Ramain;
    }*/

    // 算法需要检验是否正确
    for (WORD i=m_nLastPos; i<MAX_IV_BUF_Size; ++i)
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
  
    AutoLockAndUnlock(m_cs);
    list<GroupTarget*>& TarList = ChanTarget.TargetSaveList;
    for ( list<GroupTarget*>::iterator listIter = TarList.begin();
          listIter!= TarList.end(); )
    {
        GroupTarget* pGroupTarget = *listIter;
        if ( pGroupTarget->m_TargetQueue->nRef == 0 )
        {
            pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
            TarList.erase(listIter);
            listIter = TarList.begin();

            // compare alarm time 
            // Save to File ..
            // []
        }
        else
        {
            break;
        }
    }
}

void CIVDataBuf::DoOpenFileEvevt( DWORD dwChannel )
{

}

void CIVDataBuf::DoCloseFileEvent( DWORD dwChannel )
{

}

GroupTarget* CIVDataBuf::ChannelTarget::Find( const FILETIME& time )
{
    if ( 0 == TargetList.size() )
    {
        return NULL;
    }

    GroupTarget* pGroupTarget = NULL;
    list<GroupTarget*>::iterator iter;
    for ( iter = TargetList.begin(); ; ++iter)
    {
        if ( iter == TargetList.end() )
        {
            TargetSaveList.splice(TargetSaveList.end(), TargetList);
            break;
        }

        if ( **iter == time )
        {
            pGroupTarget = *iter;
            TargetSaveList.splice(
                TargetSaveList.end(), TargetList, TargetList.begin(), ++iter);
            break;
        }
        else if ( **iter > time )
        {
            TargetSaveList.splice(
                TargetSaveList.end(), TargetList, TargetList.begin(), ++iter);
            break;
        }  
     
    }

    return pGroupTarget;
}

void CIVDataBuf::ChannelTarget::PushBack( GroupTarget* pGroupTarget )
{
    TargetList.push_back(pGroupTarget);
}

long CIVDataBuf::ChannelTarget::GetTimeMaxBetween()
{
    GroupTarget* pFront = TargetList.front();   
    GroupTarget* pBack = TargetList.back();
    return pBack->m_time - pFront->m_time;
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