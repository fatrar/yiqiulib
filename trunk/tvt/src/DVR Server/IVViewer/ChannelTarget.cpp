/**CPP*************************************************************************
 File            : ChannelTarget.cpp
 Subsystem       : 
 Function Name(s): CChannelTarget
 Author          : YiQiu
 Date            : 2010-3-24  
 Time            : 11:01
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "IVDataBuf.h"
#include "ChannelTarget.h"



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

void CIVDataBuf::ChannelTarget::PushBack(
    GroupTarget* pGroupTarget )
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
            *@note 1.4. 收尾(释放缓存的引用，删除节点)
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
            *@note 2.4. 收尾(释放缓存的引用，删除节点)
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

    UpdateDataIndex(t, dwCurPos);

    dwPrePos = dwCurPos;
    dwCurPos = DataHead.dwNextPos;
}

void CIVDataBuf::ChannelTarget::UpdateDataIndex(
    const FILETIME& t,
    DWORD DataOffset)
{
    if ( ++dwCount < Head_Index_Interval )
    {
        return;
    }

    dwCount = 0;  
    if ( FileHead.dwIndexNum == Max_IVData_Index )
    {
        IVFileDataIndex TmpDataIndex;
        TmpDataIndex.TimeOffset = (DWORD)(t-FileHead.BeginTime);
        TmpDataIndex.DataOffset = DataOffset;
        MoreDataIndex.push_back(TmpDataIndex);
    }
    else
    {
        IVFileDataIndex& TmpDataIndex = 
            FileHead.DataIndex[FileHead.dwIndexNum];
        TmpDataIndex.TimeOffset = (DWORD)(t-FileHead.BeginTime);
        TmpDataIndex.DataOffset = DataOffset;
        ++FileHead.dwIndexNum;
    }
}

void CIVDataBuf::ChannelTarget::UpdateDataIndexToFile()
{

}






// End of file