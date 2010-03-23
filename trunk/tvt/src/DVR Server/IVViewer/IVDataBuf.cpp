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

static const char c_szIVFileExt[] = ".iv";

CIVDataBuf::CIVDataBuf(void)
    : m_nLastPos(0)
    , m_IsInit(FALSE)
    , m_pTargetBuf(NULL)
    , m_Event(NULL)
    , m_Thread(NULL)
    , m_nPreAlarmTime()
{
}

CIVDataBuf::~CIVDataBuf(void)
{
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
    return TRUE;
}

size_t WINAPI CIVDataBuf::SaveFileThread( void* pParm )
{
    return ((CIVDataBuf*)pParm)->SaveFileLoopFun();
}

size_t CIVDataBuf::SaveFileLoopFun()
{
    DWORD dwEventCount = GetEventCount();
    BOOL bExit = FALSE;
    while (!bExit)
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
            bExit = TRUE;
        	break;
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
    

}

void CIVDataBuf::DoOpenFileEvevt( DWORD dwChannel )
{

}

void CIVDataBuf::DoCloseFileEvent( DWORD dwChannel )
{

}

HANDLE CIVDataBuf::GetMyWantEvent( IVBufEvent e, int nChannelID )
{
    return m_Event[1+Event_Count*nChannelID + (e+1)];
}

DWORD CIVDataBuf::GetEventCount()
{
    static DWORD s_nEventCount = 1+m_nDeviceCount*m_nEveryDeviceChannelNum*Event_Count;
    return s_nEventCount; 
}



/**
****************** ChannelTarget ******************
*/
GroupTarget* CIVDataBuf::ChannelTarget::Find(const FILETIME& time)
{
    if ( 0 == TargetList.size() )
    {
        return NULL;
    }

    /**
    *@note if Find Time > Buffer list[i]，Move list 0-i to Save list,return NULL
    *   if Find time == Buffer list[i],Move list 0-i to Save list, and return find
    *   if Find time < Buffer[any], do nothing,return NULL
    *@note use **iter ^ time(A) and not use **iter > time and **iter == time(B),
    * the reason is A compare once, B compare twice
    */
    GroupTarget* pGroupTarget = NULL;
    list<GroupTarget*>::iterator iter = TargetList.begin();
    long nState = **iter ^ time;
    if ( nState > 0 )
    {
        return NULL;
    }
    else if ( nState == 0 )
    {
        pGroupTarget = *iter;     
        TargetSaveList.push_back(pGroupTarget);
        TargetList.pop_front(); 
        return pGroupTarget;
    }

    for ( ++iter; ; ++iter)
    {
        if ( iter == TargetList.end() )
        {
            TargetSaveList.splice(TargetSaveList.end(), TargetList);
            break;
        }

        nState = **iter ^ time;
        if ( nState == 0 )
        {
            DebugOut("Find Obj!\n");
            pGroupTarget = *iter;
            TargetSaveList.splice(
                TargetSaveList.end(), TargetList, TargetList.begin(), ++iter);
            break;
        }
        else if ( nState > 0 )
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

void CIVDataBuf::ChannelTarget::NewFileComing( 
    const char* pPath, 
    const FILETIME& time )
{
    AutoLockAndUnlock(cs);
    FilePathList.push_back( FileInfo(pPath, time) );
}

bool CIVDataBuf::ChannelTarget::FileClose(
    const FILETIME& time )
{
    FileInfoList::iterator iter;
    for ( iter = FilePathList.begin();
          iter!= FilePathList.end();
          ++iter )
    {
        if (!iter->IsColse)
        {
            iter->CloseTime = time;
            iter->IsColse = TRUE;
            return true;
        }
    }

    assert(false);
    return false;
}

void CIVDataBuf::ChannelTarget::FillHeadToFile(
    const FILETIME& OpenTime )
{
    static IVFileHead s_IVFileHead;
    s_IVFileHead.BeginTime = OpenTime;
    Writer.write((char*)&s_IVFileHead, sizeof(IVFileHead));
    UpdatePos();
}

void CIVDataBuf::ChannelTarget::DropSomeData(int nPreAlarmTime)
{
    SYSTEMTIME syt;
    GetLocalTime(&syt);
    FILETIME Now;
    SystemTimeToFileTime(&syt, &Now);
    AutoLockAndUnlock(cs);
    for ( TTargetList::iterator iter = TargetSaveList.begin();
          iter!= TargetSaveList.end(); )
    {
        /**
        *@note 1. 判断数据是否还被引用
        */
        GroupTarget* pGroupTarget = *iter;
        if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
             pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
        {
            break;
        }
    
        /**
        *@note 2. 判断数据是否超时，即超过预录像的时间
        */
        __int64 nBetween = Now - pGroupTarget->m_time;
        if ( nBetween/1000 < nPreAlarmTime )
        {
             break;
        }
           
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);
        iter = TargetSaveList.begin();
    }
}

void CIVDataBuf::ChannelTarget::TrySaveData(int nPreAlarmTime)
{
    /**
    *@note 1. 判断文件是否打开，
    *   NO , 是否有新的Path过来，没有则丢弃一些超过PreAlarm的数据，
    *         有则创建新文件，写PreAlarm的数据，
    *            判断文件是否有Close标记，有则保存数据到Close时间
    *   Yes, 直接写PreAlarm的数据，判断文件是否有Close标记，有则保存数据到Close时间
    */
    bool IsFileOpen = Writer.is_open();
    if ( IsFileOpen )
    {
        return SaveData(nPreAlarmTime);
    }
    
    if ( FilePathList.size() == 0 )
    {
        return DropSomeData(nPreAlarmTime);
    }
    
    FileInfo Info;
    {
        AutoLockAndUnlock(cs);
        Info = FilePathList.front();
    }

    string strPath = Info.Path+c_szIVFileExt;
    Writer.open(
        strPath.c_str(),
        ios::binary | ios::trunc | ios::out );
    IsFileOpen = Writer.is_open();
    if ( IsFileOpen )
    {
        FillHeadToFile(Info.OpenTime);
    }
    else
    {
        assert(false);
    }

    SaveData(nPreAlarmTime, &Info);
}

void CIVDataBuf::ChannelTarget::SaveData(
    int nPreAlarmTime,
    FileInfo* Info)
{
    AutoLockAndUnlock(cs);
    if ( Info == NULL )
    {
        Info = &(FilePathList.front());
    }

    TTargetList::iterator iter = TargetSaveList.begin();
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    if ( Info->IsColse )
    {
        for ( ; iter!= TargetSaveList.end(); )
        {
            /**
            *@note 1.1. 判断数据是否还被引用
            */
            GroupTarget* pGroupTarget = *iter;
            if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
                 pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
            {
                break;
            }

            /**
            *@note 1.2. 判断数据要保存到的时间与数据时间的关系
            *  if > 数据大于保存的时间则，关闭文件
            *  if =  保存当前的这组数据，然后关闭文件
            *  if <  保存数据，继续循环
            */
            long nState = Info->CloseTime ^ pGroupTarget->m_time;
            if ( nState < 0 )
            {
                Writer.close();
                UpdateFileInfo();
                return;
            }

            /**
            *@note 1.3. 保存数据
            */
            SaveDataHeadToFile(
                DataHead,
                pGroupTarget->m_TargetQueue->nCount,
                pGroupTarget->m_time);
            SaveTargetToFile(pGroupTarget->m_TargetQueue);
            bNeedFlush = TRUE;

            /**
            *@note 1.4. 收尾
            */
            pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
            pGroupTarget->m_TargetQueue->nRef = 0;
            delete pGroupTarget;
            TargetSaveList.erase(iter);
            
            if ( nState == 0 )
            {
                Writer.close();
                UpdateFileInfo();
                return;
            }
            
            iter = TargetSaveList.begin();
        }
    }
    else
    {
        SYSTEMTIME syt;
        GetLocalTime(&syt);
        FILETIME Now;
        SystemTimeToFileTime(&syt, &Now);
        for ( ;iter!= TargetSaveList.end(); )
        {
            /**
            *@note 2.1. 判断数据是否还被引用
            */
            GroupTarget* pGroupTarget = *iter;
            if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
                 pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
            {
                break;
            }

            /**
            *@note 2.2. 判断数据是否超时，即超过预录像的时间
            *  (在这里做上面等于的特殊处理，毕竟在这里等于的情况是基本不可能的)
            */
            __int64 nBetween = Now - pGroupTarget->m_time;
            if ( nBetween/1000 < nPreAlarmTime )
            {
                break;
            }

            /**
            *@note 2.3. 保存数据
            */
            SaveDataHeadToFile(
                DataHead,
                pGroupTarget->m_TargetQueue->nCount,
                pGroupTarget->m_time);
            SaveTargetToFile(pGroupTarget->m_TargetQueue);
            bNeedFlush = TRUE;

            /**
            *@note 2.4. 收尾
            */
            pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
            pGroupTarget->m_TargetQueue->nRef = 0;
            delete pGroupTarget;
            TargetSaveList.erase(iter);
            iter = TargetSaveList.begin();
        }
    }

    if ( bNeedFlush )
    {
        Writer.flush();
    } 
}

void CIVDataBuf::ChannelTarget::SaveDataHeadToFile( 
    IVFileDataHead& DataHead,
    WORD nTargetCount,
    const FILETIME& t )
{
    DataHead.dwPrePos = dwPrePos;
    DataHead.dwNextPos = dwCurPos + sizeof(IVFileDataHead) + nTargetCount*sizeof(WPG_Target);
    DataHead.dwTargetNum = nTargetCount;
    DataHead.t = t;
    Writer.write((char*)&DataHead, sizeof(IVFileDataHead));

    dwPrePos = dwCurPos;
    dwCurPos = DataHead.dwNextPos;
}

void CIVDataBuf::ChannelTarget::SaveTargetToFile(
    const TargetQueue* m_TargetQueue )
{
    size_t nSize = m_TargetQueue->nCount*sizeof(WPG_Target);
    Writer.write((char*)&m_TargetQueue->Tar, nSize);
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