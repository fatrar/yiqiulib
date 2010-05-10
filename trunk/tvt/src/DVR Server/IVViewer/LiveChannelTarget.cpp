/**CPP*************************************************************************
 File            : LiveChannelTarget.cpp
 Subsystem       : 
 Function Name(s): CIVLiveDataBuf::ChannelTarget
 Author          : YiQiu
 Date            : 2010-5-8  
 Time            : 17:08
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "ChannelTarget.h"


template CIVLiveDataBuf::ChannelTarget<IVFile_Version_1_0>;
template CIVLiveDataBuf::ChannelTarget<IVFile_Version_2_0>;



template<DWORD dwIVFileVersion>
CIVLiveDataBuf::TGroupTarget* CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::Find(
    const FILETIME& time)
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
    TGroupTarget* pGroupTarget = NULL;
    TargetListIter iter = TargetList.begin();
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

    /**
    *@note 下面理论上按我的引用模型，
    * 凡是被剪贴到TargetSaveList的数据都要调用Release
    * 但是为了效率考虑 数据保存线程做特殊处理
    * 因为没有被上层使用的数据肯定是没有调用Use，从而nUse标志为Buf_No_Use
    */
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
            DebugOut("ChannelTarget::Find Find Obj!\n");
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

template<DWORD dwIVFileVersion>
bool CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::StartSection(
    const char* pPath,
    const FILETIME& time )
{
    {
        AutoLockAndUnlock(cs);
        FileInfoList::iterator iter;
        for ( iter = FilePathList.begin();
            iter!= FilePathList.end();
            ++iter )
        {
            if (iter->strPath != pPath)
            {
                continue;
            }

            if ( iter->bIsColse )
            {
                assert(iter->bIsColse);
                break;
            }

            // 防止连续调两次Start
            if ( !iter->UnFinishSection.empty() )
            {
                if ( iter->UnFinishSection.back().EndTime == 0 )
                {
                    //assert(false);
                    TRACE("StartSection Error!\n");
                    break;
                }
            }

            FileSectionTime SectionTime;
            SectionTime.BeginTime = time;
            iter->UnFinishSection.push_back( SectionTime );          
            return true;
        }
    }

    //assert(false);
    return false;
}

template<DWORD dwIVFileVersion>
bool CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::EndSection(
    const char* pPath,
    const FILETIME& time )
{
    {
        AutoLockAndUnlock(cs);
        FileInfoList::iterator iter;
        for ( iter = FilePathList.begin();
            iter!= FilePathList.end();
            ++iter )
        {
            if (iter->strPath != pPath)
            {
                continue;
            }

            if ( iter->bIsColse )
            {
                assert(iter->bIsColse);
                break;
            }

            if ( iter->UnFinishSection.empty() )
            {
                TRACE("EndSection UnFinishSection empty!\n");
                break;
            }

            FileSectionTime& SectionTime = iter->UnFinishSection.back();
            if ( SectionTime.BeginTime.GetTime() == 0 ||
                SectionTime.EndTime.GetTime() != 0 )
            {
                TRACE("EndSection Error!\n");
                assert(false);
                break;
            }

            if ( SectionTime.BeginTime >= time ||
                time - SectionTime.BeginTime < 300 )
            {
                assert(false);
                FilePathList.erase(iter);
                break;
            }

            SectionTime.EndTime = time;
            return true;
        }
    }

    assert(false);
    return false;
}

template<DWORD dwIVFileVersion>
bool CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::FileClose(
    const char* pPath )
{
    {
        AutoLockAndUnlock(cs);
        FileInfoList::iterator iter;
        for ( iter = FilePathList.begin();
            iter!= FilePathList.end();
            ++iter )
        {
            if (iter->strPath == pPath)
            {
                iter->bIsColse = TRUE;
                return true;
            }
        }
    }

    // [] there, need Add a Event to Save File
    assert(false);
    return false;
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::DropSomeData(
    int nPreAlarmTime,
    const FILETIME& Now )
{
    AutoLockAndUnlock(cs);
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
    {
        /**
        *@note 1. 判断数据是否还被引用
        */
        TGroupTarget* pGroupTarget = *iter;
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

template<>
void CIVLiveDataBuf::ChannelTarget<IVFile_Version_1_0>::ResetFileHead()
{
    FileHead.HeadFirst.FileFlag = g_dwIVFileFail;
    FileHead.dwIndexNum = 0;
}

template<>
void CIVLiveDataBuf::ChannelTarget<IVFile_Version_2_0>::ResetFileHead()
{
    FileHead.HeadFirst.FileFlag = g_dwIVFileFail;
    FileHead.wIndexNum = 0;
    FileHead.wTailIndexNum = 0;
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::DropSomeData(
    int nPreAlarmTime)
{
    SYSTEMTIME syt;
    GetLocalTime(&syt);
    FILETIME Now;
    SystemTimeToFileTime(&syt, &Now);
    return DropSomeData(nPreAlarmTime, Now);
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::TrySaveData(
    int nPreAlarmTime )
{
    if ( TargetSaveList.size() == 0 )
    {
        return;    
    }

    /**
    *@note 1. 判断文件是否打开，
    *   NO , 是否有新的Path过来，没有则丢弃一些超过PreAlarm的数据，
    *         有则创建新文件，写PreAlarm的数据，
    *            判断文件是否有Close标记，有则保存数据到Close时间
    *   Yes, 直接写PreAlarm的数据，判断文件是否有Close标记，有则保存数据到Close时间
    */
    //bool IsFileOpen = Writer.is_open();
    //if ( IsFileOpen )
    //{
    //    return SaveData(nPreAlarmTime);
    //}

    if ( FilePathList.size() == 0 )
    {
        return DropSomeData(nPreAlarmTime);
    }

    {
        AutoLockAndUnlock(cs);
        FileInfo& Info = FilePathList.front();
        if ( Info.bIsColse )
        {
            SaveByFileIsSetClose(nPreAlarmTime, Info);
        }
        else // Info->bIsColse
        {
            SaveByFileIsNoSetClose(nPreAlarmTime, Info);
        }
    }
    //{
    //    AutoLockAndUnlock(cs);
    //    FileInfo& Info = FilePathList.front();
    //    if ( Info.UnFinishSection.empty() )
    //    {
    //        return;
    //    }

    //    string strPath = Info.strPath+c_szIVFileExt;
    //    Writer.open(
    //        strPath.c_str(),
    //        ios::binary | ios::trunc | ios::out );
    //    IsFileOpen = Writer.is_open();
    //    if ( IsFileOpen )
    //    {
    //        FillHeadToFile(Info.UnFinishSection.front().BeginTime);
    //    }
    //    else
    //    {
    //        assert(false);
    //    }

    //    SaveData(nPreAlarmTime, &Info);
    //}
}

//void CIVLiveDataBuf::ChannelTarget::SaveData(
//    int nPreAlarmTime,
//    FileInfo* Info)
//{
//    AutoLockAndUnlock(cs);
//    if ( Info == NULL )
//    {
//        Info = &(FilePathList.front());
//    }
//
//    if ( Info->bIsColse )
//    {
//        SaveByFileIsSetClose(nPreAlarmTime, *Info);
//    }
//    else // Info->bIsColse
//    {
//        SaveByFileIsNoSetClose(nPreAlarmTime, *Info);
//    }
//}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::SaveDataHeadToFile( 
    IVFileDataHead& DataHead,
    WORD nTargetCount,
    const FILETIME& t )
{
    DataHead.dwPrePos = dwPrePos;
    DataHead.dwNextPos = dwCurPos + sizeof(IVFileDataHead) + nTargetCount*sizeof(WPG_Target);
    DataHead.wTargetNum = nTargetCount;
    DataHead.t = t;
    Writer.write((char*)&DataHead, sizeof(IVFileDataHead));

    UpdateDataIndex(t, dwCurPos);

    dwPrePos = dwCurPos;
    dwCurPos = DataHead.dwNextPos;
}

template<>
void CIVLiveDataBuf::ChannelTarget<IVFile_Version_1_0>::UpdateDataIndex(
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

template<>
void CIVLiveDataBuf::ChannelTarget<IVFile_Version_2_0>::UpdateDataIndex(
    const FILETIME& t,
    DWORD DataOffset)
{
    if ( ++dwCount < Head_Index_Interval )
    {
        return;
    }

    dwCount = 0;  
    if ( FileHead.wIndexNum == Max_IVData_Index )
    {
        IVFileDataIndex TmpDataIndex;
        TmpDataIndex.TimeOffset = (DWORD)(t-FileHead.BeginTime);
        TmpDataIndex.DataOffset = DataOffset;
        MoreDataIndex.push_back(TmpDataIndex);
        ++FileHead.wTailIndexNum;
    }
    else
    {
        IVFileDataIndex& TmpDataIndex = 
            FileHead.DataIndex[FileHead.wIndexNum];
        TmpDataIndex.TimeOffset = (DWORD)(t-FileHead.BeginTime);
        TmpDataIndex.DataOffset = DataOffset;
        ++FileHead.wIndexNum;
    }
}

typedef CDropTableMgr<BYTE, Max_IVData_Index> ArrayDropTableMgr;
typedef ArrayDropTableMgr::c_DropTable ArrayDropTable;
typedef CDropTableMgr2<BYTE> QueueDropTableMgr;
typedef QueueDropTableMgr::c_DropTable QueueDropTable;

/**
*@note 这个函数算法比较复杂，需要验证
*   这个算法原本是用于一个文件只有一个区段，现在文件可能有很多个段，丢弃效果就好了，
*  可以根据每个段的个数，重新算丢弃值。现在只做当一个段处理，暂时不改
*/
template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::UpdateDataIndexToFile(
    const FileInfo& Info )
{
    //const FileInfo::SectionTimeList& FinishSection = Info.FinishSection;
    //if ( FinishSection.empty() )
    //{
    //    return;
    //}

    /**
    *@note 1. 将IV文件头EndTime赋值，注意这个值是上层传过来的视频文件最后一帧时间
    *    将Writer文件指针移到开始，这样就可以直接覆盖文件打开的那个
    *    当MoreDataIndex有数据，那么就需要将FileHead和MoreDataIndex的索引数据在平均丢弃，
    *    然后将调整后的时候放在FileHead中
    *    MoreDataIndex没数据，那么直接写FileHead就好
    */
    FileHead.BeginTime = Info.FrameBeginTime;
    FileHead.EndTime = Info.FrameEndTime;
    FileHead.HeadFirst.FileFlag = g_dwIVFileOK;
    FileHead.dwLastFramePos = dwPrePos;
    Writer.seekp(0);
    size_t nQueueSize = MoreDataIndex.size();
    if ( nQueueSize != 0 )
    {
        /**
        *@note 2. 写算出FileHead和MoreDataIndex分别要丢多少
        */
        IVFileDataIndex (&DataIndex)[Max_IVData_Index] = FileHead.DataIndex;
        size_t nArrayDrop = 
            (Max_IVData_Index*nQueueSize)/(Max_IVData_Index+nQueueSize);
        size_t nQueueDrop = nQueueSize - nArrayDrop;

        /**
        *@note 3. 根据FileHead的丢弃值，根据ArrayDropTableMgr得到丢弃表
        *         然后根据丢弃表，将丢弃不要，重新移动队列使其连续
        */
        ArrayDropTableMgr* p = ArrayDropTableMgr::getInstancePtr();
        ArrayDropTable* t = p->GetDropTable(nArrayDrop);
        size_t nSize = t->size();
        size_t nIndex = 0;
        assert( nSize != 0 && nSize == nArrayDrop );
        {       
            /**
            *@note 3.1. 首先头段因为不需要移动，直接将nIndex移到一个丢弃位置
            */
            ArrayDropTable::const_iterator iter1 = t->begin(),iter2=iter1; 
            nIndex = *iter1;

            /**
            *@note 3.2. 处理中间段，只要将两个丢弃位置的数据左移
            *           左移到由每次处理后，nIndex移动的最后位置决定
            */
            ++iter2;
            for ( ; iter2 != t->end() ; iter1 = iter2, ++iter2)
            {
                for (BYTE i=*iter1; i<*iter2-1; ++i, ++nIndex)
                {
                    DataIndex[nIndex] = DataIndex[i+1];
                }
            }

            /**
            *@note 3.3. 处理尾段，尾端是没有丢弃位置的结束值，
            *           那么用Buf长度决定
            */
            for (BYTE i=*iter1; i<Max_IVData_Index-1; ++i,++nIndex)
            {
                DataIndex[nIndex] = DataIndex[i+1];
            } 
        }

        /**
        *@note 3. 根据MoreDataIndex的丢弃值，根据QueueDropTableMgr得到丢弃表
        *         然后将不需要丢弃的数据拷贝到FileHead
        */
        QueueDropTableMgr* q = QueueDropTableMgr::getInstancePtr();
        QueueDropTable* t2 = q->GetDropTable(nQueueDrop, nQueueSize);
        nSize = t2->size();
        assert( nSize != 0 && nSize == nQueueDrop);
        {
            QueueDropTable::const_iterator iter = t2->begin();
            deque<IVFileDataIndex>::iterator IndexIter = MoreDataIndex.begin();
            for ( BYTE nQueueIndex = 0;
                IndexIter!=MoreDataIndex.end();
                ++IndexIter, ++nQueueIndex )
            {
                if ( iter!=t2->end() && *iter == nQueueIndex )
                {
                    ++iter;
                    continue;      
                } 

                DataIndex[nIndex] = *IndexIter;
                ++ nIndex;
            }
        }
    }

    Writer.write(
        (char*)&FileHead,
        sizeof(IVFileHead<dwIVFileVersion>) );
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::SaveByFileIsSetClose(
    int nPreAlarmTime,
    FileInfo& Info )
{
    /**
    *@note 1. 如果没有的处理的数据段信息，就直接刷新索引，关闭文件
    */
    if ( Info.UnFinishSection.empty() )
    {
        if ( Writer.is_open() )
        {
            // 首先更新文件索引，然后关闭文件，删除当前使用文件信息
            UpdateDataIndexToFile(Info);
            // 这里有一种情况是，文件路径给了，但是一直没开始设置录
            // 即有路径但没有段信息，
            // 这个时候需要丢弃所有期间数据，文件也没创建
            Writer.close();
        }    
        FilePathList.pop_front();
        return;
    }

    /**
    *@note 2. 保存还没有保存完的段信息的数据 
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    string& strPath = Info.strPath;
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
    {
        /**
        *@note 2.1. 判断数据是否还被引用
        */
        TGroupTarget* pGroupTarget = *iter;
        if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
            pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
        {
            break;
        }

        /**
        *@note 2.2. 异常处理，判断段信息时间是否有误，
        *           正常情况都不为0,
        *  如果为异常数据则丢弃，取下一个
        */
        if ( SectionTime.BeginTime.GetTime() == 0 ||
            SectionTime.EndTime.GetTime() == 0 )
        {
            assert(false);

            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                UpdateDataIndexToFile(Info);
                if ( Writer.is_open() )
                {
                    Writer.close();
                }
                FilePathList.pop_front();
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            continue;
        }

        /**
        *@note 2.3. 判断数据要保存到的时间与数据时间的关系
        *  if > 数据大于保存的时间则，取下一个段信息。没有下一个段则关闭文件
        *  if =  保存当前的这组数据，然后关闭文件
        *  if <  保存数据，继续循环
        */
        long nState = pGroupTarget->m_time ^ SectionTime.EndTime;
        if ( nState > 0 )
        {
            /**
            *@note 2.3.1 段时间用真正的数据时间，然后将段信息放进处理完的队列里，
            *  判断是否还有未处理的段，没有就做文件关闭及其更新文件索引
            *  有则取下一个段，继续循环
            */
            SectionTime.EndTime = pGroupTarget->m_time;
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                if ( Writer.is_open() )
                {
                    UpdateDataIndexToFile(Info);
                    Writer.close();
                }
                FilePathList.pop_front();
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            continue;
        }

        /**
        *@note 2.3.2 小于等于，则保存数据
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead, 
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.3.3. 收尾(释放缓存的引用，删除节点)
        */
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);

        if ( nState == 0 )
        {
            /**
            *@note 2.3.4. 刚好相等，则将当前段信息放进处理完的队列里，
            *  判断是否还有未处理的段，没有就做文件关闭及其更新文件索引
            *  有则取下一个段，继续循环
            */
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                // 使用真正的数据时间
                UpdateDataIndexToFile(Info);
                Writer.close();
                FilePathList.pop_front();
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            // continue;
        }

        iter = TargetSaveList.begin();
    }

    if ( bNeedFlush )
    {
        Writer.flush();
    } 
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::SaveByFileIsNoSetClose( 
    int nPreAlarmTime,
    FileInfo& Info )
{
    SYSTEMTIME syt;
    GetLocalTime(&syt);
    FILETIME Now;
    SystemTimeToFileTime(&syt, &Now);
    /**
    *@note 1. 如果没有的处理的数据段信息，就做丢弃处理
    */
    if ( Info.UnFinishSection.empty() )
    {
        DropSomeData(nPreAlarmTime, Now);
        return;
    }

    /**
    *@note 2. 保存还没有保存完的段信息的数据
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    string& strPath = Info.strPath;  
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
    {
        /**
        *@note 2.1. 判断数据是否还被引用
        */
        TGroupTarget* pGroupTarget = *iter;
        if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
            pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
        {
            break;
        }

        /**
        *@note 2.2. 异常处理，判断段信息的开始时间是否有误，
        *           至少开始时间不为0,
        *           如果为异常数据则丢弃，取下一个
        */
        if ( SectionTime.BeginTime.GetTime() == 0 )
        {
            assert(false);

            bIsFirstFrame = TRUE;
            Info.UnFinishSection.pop_front();
            if ( Info.UnFinishSection.empty() )
            {
                DropSomeData(nPreAlarmTime, Now);
                return;
            }

            SectionTime = Info.UnFinishSection.front();
        }

        /**
        *@note 2.2. 如果该段有设置结束时间，则保存到该结束时间，
        *           然后取下一个段
        */
        BOOL bMustSave = FALSE;
        if ( SectionTime.EndTime.GetTime() != 0 )
        {
            long nState = pGroupTarget->m_time ^ SectionTime.EndTime;
            if ( nState > 0 )
            {
                bIsFirstFrame = TRUE;
                SectionTime.EndTime = pGroupTarget->m_time;
                Info.FinishSection.push_back(SectionTime);
                Info.UnFinishSection.pop_front();
                if ( Info.UnFinishSection.empty() )
                {
                    DropSomeData(nPreAlarmTime, Now);
                    return;
                }

                SectionTime = Info.UnFinishSection.front();

                continue;
            }

            // 有结束时间，且时间大于数据时间,设置标志没必要判断预录像时间
            bMustSave = TRUE;
        }

        if ( !bMustSave )
        {
            /**
            *@note 2.3. 判断数据是否超时，即超过预录像的时间, 
            *  当没有设置结束时间，我们保存数据必须超过预录像的时间
            */
            __int64 nBetween = Now - pGroupTarget->m_time;
            if ( nBetween/1000 < nPreAlarmTime )
            {
                break;
            }
        }

        /**
        *@note 2.4. 保存数据
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead,
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.5. 收尾(释放缓存的引用，删除节点)
        */
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);

        iter = TargetSaveList.begin();
    }

    if ( bNeedFlush )
    {
        Writer.flush();
    }
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::FinallySave()
{
    // 将所有未来得及live播放的数据放进保存队列
    TargetSaveList.splice(TargetSaveList.end(), TargetList);

    AutoLockAndUnlock(cs);
    FileInfoList::iterator iter;
    for ( iter = FilePathList.begin();
        iter!= FilePathList.end();
        ++iter )
    {
        FileInfo& Info = *iter;
        FinallySaveAFile(Info);
    }

    FilePathList.clear();
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::FinallySaveAFile(
    FileInfo& Info )
{
    /**
    *@note 1. 如果没有的处理的数据段信息，就直接刷新索引，关闭文件
    */
    if ( Info.UnFinishSection.empty() )
    { 
        if ( Writer.is_open() )
        {
            // 首先更新文件索引，然后关闭文件，删除当前使用文件信息
            UpdateDataIndexToFile(Info);
            // 这里有一种情况是，文件路径给了，但是一直没开始设置录
            // 即有路径但没有段信息，
            // 这个时候需要丢弃所有期间数据，文件也没创建
            Writer.close();
        }    
        return;
    }

    /**
    *@note 2. 保存还没有保存完的段信息的数据 
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    string& strPath = Info.strPath;
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
    {
        TGroupTarget* pGroupTarget = *iter;

        /**
        *@note 2.2. 异常处理，判断段信息时间是否有误，
        *           正常情况都不为0,
        *  如果为异常数据则丢弃，取下一个
        */
        if ( SectionTime.BeginTime.GetTime() == 0 ||
            SectionTime.EndTime.GetTime() == 0 )
        {
            assert(false);

            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                UpdateDataIndexToFile(Info);
                if ( Writer.is_open() )
                {
                    Writer.close();
                }
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            continue;
        }

        /**
        *@note 2.3. 判断数据要保存到的时间与数据时间的关系
        *  if > 数据大于保存的时间则，取下一个段信息。没有下一个段则关闭文件
        *  if =  保存当前的这组数据，然后关闭文件
        *  if <  保存数据，继续循环
        */
        long nState = pGroupTarget->m_time ^ SectionTime.EndTime;
        if ( nState > 0 )
        {
            /**
            *@note 2.3.1 段时间用真正的数据时间，然后将段信息放进处理完的队列里，
            *  判断是否还有未处理的段，没有就做文件关闭及其更新文件索引
            *  有则取下一个段，继续循环
            */
            SectionTime.EndTime = pGroupTarget->m_time;
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                if ( Writer.is_open() )
                {
                    UpdateDataIndexToFile(Info);
                    Writer.close();
                }
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            continue;
        }

        /**
        *@note 2.3.2 小于等于，则保存数据
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead,
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.3.3. 收尾(释放缓存的引用，删除节点)
        */
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);

        if ( nState == 0 )
        {
            /**
            *@note 2.3.4. 刚好相等，则将当前段信息放进处理完的队列里，
            *  判断是否还有未处理的段，没有就做文件关闭及其更新文件索引
            *  有则取下一个段，继续循环
            */
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                // 使用真正的数据时间
                UpdateDataIndexToFile(Info);
                Writer.close();
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            // continue;
        }

        iter = TargetSaveList.begin();
    }

    if ( Info.UnFinishSection.empty() )
    {
        assert(false);
        if ( Writer.is_open() )
        {
            UpdateDataIndexToFile(Info);
            Writer.close();
        }
        return;
    }

    if ( Writer.is_open() )
    {
        Info.UnFinishSection.front().EndTime = Info.FrameBeginTime;
        UpdateDataIndexToFile(Info);
        Writer.close();
    }

    return;
}

// }
// CIVLiveDataBuf::ChannelTarget





// End of file