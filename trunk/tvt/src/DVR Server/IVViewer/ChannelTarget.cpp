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
//#include "IVLiveDataBuf.h"
#include "ChannelTarget.h"

static const FILETIME s_ErrFileTime = {0, 0}; 

CIVLiveDataBuf::TGroupTarget* CIVLiveDataBuf::ChannelTarget::Find(
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

bool CIVLiveDataBuf::ChannelTarget::StartSection(
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
                    assert(false);
                    TRACE("StartSection Error!");
                    break;
                }
            }

            FileSectionTime SectionTime;
            SectionTime.BeginTime = time;
            iter->UnFinishSection.push_back( SectionTime );          
            return true;
        }
    }

    assert(false);
    return false;
}

bool CIVLiveDataBuf::ChannelTarget::EndSection(
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

            FileSectionTime& SectionTime = iter->UnFinishSection.back();
            if ( SectionTime.BeginTime.GetTime() == 0 ||
                 SectionTime.EndTime.GetTime() != 0 )
            {
                TRACE("EndSection Error!");
                assert(false);
                break;
            }

            SectionTime.EndTime = time;
            return true;
        }
    }

    assert(false);
    return false;
}

bool CIVLiveDataBuf::ChannelTarget::FileClose(
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

void CIVLiveDataBuf::ChannelTarget::DropSomeData(
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

void CIVLiveDataBuf::ChannelTarget::DropSomeData(int nPreAlarmTime)
{
    SYSTEMTIME syt;
    GetLocalTime(&syt);
    FILETIME Now;
    SystemTimeToFileTime(&syt, &Now);
    return DropSomeData(nPreAlarmTime, Now);
}

void CIVLiveDataBuf::ChannelTarget::TrySaveData(int nPreAlarmTime)
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
    bool IsFileOpen = Writer.is_open();
    if ( IsFileOpen )
    {
        return SaveData(nPreAlarmTime);
    }

    if ( FilePathList.size() == 0 )
    {
        return DropSomeData(nPreAlarmTime);
    }

    {
        AutoLockAndUnlock(cs);
        FileInfo& Info = FilePathList.front();
        if ( Info.UnFinishSection.empty() )
        {
            return;
        }

        string strPath = Info.strPath+c_szIVFileExt;
        Writer.open(
            strPath.c_str(),
            ios::binary | ios::trunc | ios::out );
        IsFileOpen = Writer.is_open();
        if ( IsFileOpen )
        {
            FillHeadToFile(Info.UnFinishSection.front().BeginTime);
        }
        else
        {
            assert(false);
        }

        SaveData(nPreAlarmTime, &Info);
    }
}

void CIVLiveDataBuf::ChannelTarget::SaveData(
    int nPreAlarmTime,
    FileInfo* Info)
{
    AutoLockAndUnlock(cs);
    if ( Info == NULL )
    {
        Info = &(FilePathList.front());
    }

    if ( Info->bIsColse )
    {
        SaveByFileIsSetClose(nPreAlarmTime, *Info);
    }
    else // Info->bIsColse
    {
        SaveByFileIsNoSetClose(nPreAlarmTime, *Info);
    }
}

void CIVLiveDataBuf::ChannelTarget::SaveDataHeadToFile( 
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

void CIVLiveDataBuf::ChannelTarget::UpdateDataIndex(
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

typedef CDropTableMgr<BYTE, Max_IVData_Index> ArrayDropTableMgr;
typedef ArrayDropTableMgr::c_DropTable ArrayDropTable;
typedef CDropTableMgr2<BYTE> QueueDropTableMgr;
typedef QueueDropTableMgr::c_DropTable QueueDropTable;

/**
*@note 这个函数算法比较复杂，需要验证
*   这个算法原本是用于一个文件只有一个区段，现在文件可能有很多个段，丢弃效果就好了，
*  可以根据每个段的个数，重新算丢弃值。现在只做当一个段处理，暂时不改
*/
void CIVLiveDataBuf::ChannelTarget::UpdateDataIndexToFile(
    const FileInfo& Info )
{
    const FileInfo::SectionTimeList& FinishSection = Info.FinishSection;
    if ( FinishSection.empty() )
    {
        return;
    }

    /**
    *@note 1. 将IV文件头EndTime赋值，注意这个值是上层传过来的视频文件最后一帧时间
    *    将Writer文件指针移到开始，这样就可以直接覆盖文件打开的那个
    *    当MoreDataIndex有数据，那么就需要将FileHead和MoreDataIndex的索引数据在平均丢弃，
    *    然后将调整后的时候放在FileHead中
    *    MoreDataIndex没数据，那么直接写FileHead就好
    */
    FileHead.BeginTime = FinishSection.front().BeginTime;
    FileHead.EndTime = FinishSection.back().EndTime;
    FileHead.FileFlag = g_dwIVFileOK;
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
        sizeof(IVFileHead) );
}

void CIVLiveDataBuf::ChannelTarget::SaveByFileIsSetClose(
    int nPreAlarmTime,
    FileInfo& Info )
{
    /**
    *@note 1. 如果没有的处理的数据段信息，就直接刷新索引，关闭文件
    */
    if ( Info.UnFinishSection.empty() )
    {
        // 首先更新文件索引，然后关闭文件，删除当前使用文件信息
        UpdateDataIndexToFile(Info);
        Writer.close();
        FilePathList.pop_front();
        return;
    }

    /**
    *@note 2. 保存还没有保存完的段信息的数据 
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
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
            if ( Info.UnFinishSection.empty() )
            {
                UpdateDataIndexToFile(Info);
                Writer.close();
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

            if ( Info.UnFinishSection.empty() )
            {
                UpdateDataIndexToFile(Info);
                Writer.close();
                FilePathList.pop_front();
                return;
            }

            SectionTime = Info.UnFinishSection.front();
            continue;
        }

        /**
        *@note 2.3.2 小于等于，则保存数据
        */
        SaveGroupDataToFile(DataHead, pGroupTarget);
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

void CIVLiveDataBuf::ChannelTarget::SaveByFileIsNoSetClose( 
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
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
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
                SectionTime.EndTime = pGroupTarget->m_time;
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
        SaveGroupDataToFile(DataHead, pGroupTarget);
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

// }
// CIVLiveDataBuf::ChannelTarget

//
// ************ CIVPlaybackDataBuf::ChannelTarget ************
// {

    // __forceinline
#ifdef _FixIVFile
HANDLE CIVPlaybackDataBuf::ChannelTarget::s_hFixFileThread = NULL;
DWORD CIVPlaybackDataBuf::ChannelTarget::s_dwFixFileThread = -1;
#endif // _FixIVFile

CIVPlaybackDataBuf::ChannelTarget::ChannelTarget()
    : m_nBufLastPos(0)
{
    m_pTargetBuf = new PlayBackTargetQueue[Head_Index_Interval];
};

CIVPlaybackDataBuf::ChannelTarget::~ChannelTarget()
{
    safeDeleteArray(m_pTargetBuf);
};

template<>
BOOL CIVPlaybackDataBuf::ChannelTarget::PraseHeadToMap<IVFile_Version_1_0>(
    const IVFileHead& Head)
{
    m_DataIndex.clear();
    if ( Head.FileFlag != g_dwIVFileOK )
    {
        //return FALSE;
    }

    m_BeginTime = Head.BeginTime;
    m_EndTime = Head.EndTime;
    StlHelper::Array2STL(
        Head.DataIndex, 
        Head.dwIndexNum, 
        m_DataIndex );

    IVFileDataIndex TmpDataIndex; 
    if ( m_DataIndex.size()==0 ||
         m_DataIndex.front().TimeOffset != 0 )
    {
        TmpDataIndex.TimeOffset = 0;
        TmpDataIndex.DataOffset = sizeof(IVFileHead);
        m_DataIndex.push_front(TmpDataIndex);
    }

    DWORD EndTimeOffset = (DWORD)(Head.EndTime - Head.BeginTime);
    if ( m_DataIndex.size()==0 || 
         m_DataIndex.back().TimeOffset != EndTimeOffset )
    {
        TmpDataIndex.TimeOffset = EndTimeOffset;
        TmpDataIndex.DataOffset = Head.dwLastFramePos;
        m_DataIndex.push_back(TmpDataIndex);
    }

    return TRUE;
}

#ifdef _FixIVFile
void CIVPlaybackDataBuf::ChannelTarget::Init()
{
    if ( s_hFixFileThread )
    {
        s_hFixFileThread = CreateThread(
            NULL,
            0,
            OnFixFileThread,
            NULL,
            0,
            &s_dwFixFileThread);
    }
}

void CIVPlaybackDataBuf::ChannelTarget::Unit()
{
    if ( s_hFixFileThread != NULL )
    {
        return;
    }

    PostThreadMessage(s_dwFixFileThread, WM_QUIT, 0, 0);
    WaitForSingleObject(s_hFixFileThread, 500);
}

DWORD WINAPI CIVPlaybackDataBuf::ChannelTarget::OnFixFileThread( void* p )
{
    return 0;
}

#endif // _FixIVFile

BOOL CIVPlaybackDataBuf::ChannelTarget::Open(
    const char* pPath,
    const FILETIME& time)
{
    if ( !isValidString(pPath) )
    {
        return FALSE;
    }

    if ( m_Reader.is_open() )
    {
        m_Reader.close();
    }

    m_Reader.clear();
    string strPath = pPath;
    strPath += c_szIVFileExt;
    m_Reader.open(
        strPath.c_str(),
        ios::binary |ios::in | ios::_Nocreate );
    if ( !m_Reader.is_open() )
    {
        return FALSE;
    }

    BOOL bRc = FALSE;
    IVFileHead Head;
    m_Reader.read((char*)&Head, sizeof(IVFileHead));
    switch ( Head.Version )
    {
    case IVFile_Version_1_0:
        bRc = PraseHeadToMap<IVFile_Version_1_0>(Head);
        break;
    default:
        return FALSE;
    }

    if ( !bRc )
    {
        return bRc;
    }

    return MoveToAndReadSome(time);
}

BOOL CIVPlaybackDataBuf::ChannelTarget::Close(
    const FILETIME& time)
{
    if (m_Reader.is_open())
    {
        m_Reader.close();
        return TRUE;
    }

    return FALSE;
}

BOOL CIVPlaybackDataBuf::ChannelTarget::MoveToAndReadSome(
    const FILETIME& time)
{
    if ( !m_Reader.is_open() )
    {
        return FALSE;
    }

    size_t nQueueSize = m_DataIndex.size();
    if ( (time < m_BeginTime || time > m_EndTime) ||
         nQueueSize == 0 )
    {
        //m_Reader.close();
        return FALSE;
    }

    ClearTempData();
    size_t nPos = GetPos(time);
    if ( nPos == size_t(-1) )
    {
        assert(FALSE);
        m_Reader.close();
        return FALSE;
    }

    FILETIME CurrentFrameTime;
    m_IVSomeData.clear();
    IVFileDataIndex& TmpDataIndex = m_DataIndex[nPos];
    m_Reader.seekg(TmpDataIndex.DataOffset);
    if ( nPos == nQueueSize - 1 )
    {    
        if ( ReadSome(CurrentFrameTime) )
        {
             m_BufStartTime = m_BufEndTime = CurrentFrameTime;
             assert(CurrentFrameTime==m_EndTime);
        }
     
        m_BufStartTime = m_BufEndTime = s_ErrFileTime;
        return FALSE;
    }

    BOOL bFirstFrame = TRUE;
    IVFileDataIndex& TmpDataIndex2 = m_DataIndex[nPos+1];
    size_t nCurrentPos;
    while ( (nCurrentPos = m_Reader.tellg()) < TmpDataIndex2.DataOffset )
    {
        if ( !ReadSome(CurrentFrameTime) )
        {
            if ( bFirstFrame )
            {
                m_BufStartTime = m_BufEndTime = s_ErrFileTime;
            }
            
            return FALSE;
        }

        if ( bFirstFrame )
        {
            m_BufStartTime = m_BufEndTime = CurrentFrameTime;
            bFirstFrame = FALSE;
        }
        else
        {
            m_BufEndTime = CurrentFrameTime;
        }
    }  

    return TRUE;
}

size_t CIVPlaybackDataBuf::ChannelTarget::GetPos( const FILETIME& time )
{
    size_t nQueueSize = m_DataIndex.size();
    DWORD nTestTime = (DWORD)(time - m_BeginTime);
    double dGuessPercent = double(nTestTime)/double(m_EndTime-time);
    size_t nGuessQueusPos = size_t(nQueueSize*dGuessPercent);
    assert(nGuessQueusPos < nQueueSize);

    IVFileDataIndex& GuessDataIndex = m_DataIndex[nGuessQueusPos];
    if ( nGuessQueusPos == 0 )
    {
        for ( size_t i=0; i<nQueueSize; ++i )
        {
            IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
            if ( nTestTime >= TmpDataIndex.TimeOffset  )
            {
                return i;
            }
        }
        return size_t(-1);
    }

    if ( nTestTime == GuessDataIndex.TimeOffset )
    {
        return nGuessQueusPos;
    }
    else if ( nTestTime > GuessDataIndex.TimeOffset )
    {
        for (size_t i=nGuessQueusPos-1; i>=0; --i)
        {
            IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
            if ( nTestTime <= TmpDataIndex.TimeOffset  )
            {
                return i;
            }
        }

        return size_t(-1);
    }
    else //( nTestTime < GuessDataIndex.TimeOffset )
    {
        for (size_t i=nGuessQueusPos; i<nQueueSize; ++i)
        {
            IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
            if ( nTestTime >= TmpDataIndex.TimeOffset  )
            {
                return i;
            }
        }

        return size_t(-1);
    }

}

size_t CIVPlaybackDataBuf::ChannelTarget::FindBuf()
{
    return IVViewerFindBuf(m_nBufLastPos, m_pTargetBuf, Head_Index_Interval);
}

BOOL CIVPlaybackDataBuf::ChannelTarget::ReadSome(FILETIME& CurrentFrameTime)
{
    IVFileDataHead DataHead;
    PlayBackTargetQueue* pTemp = NULL;
    m_Reader.read(
        (char*)&DataHead, 
        sizeof(IVFileDataHead));
    
    //
    // [] 这里需要校验IVFileDataHead
    //
    /**
    *@note 1. 找是否有目标数据缓冲(TargetQueue)
    */
    int nBufPos = FindBuf();
    if ( nBufPos == NO_BUF_Ramain )
    {
        DebugOut("CIVDataBuf::OnIVDataSend No Find Buf!\n");
        assert(false);
        return FALSE;
    }

    CurrentFrameTime = DataHead.t;
    pTemp = &m_pTargetBuf[nBufPos];
    pTemp->AddRef();
    pTemp->nCount = DataHead.wTargetNum;
    assert(pTemp->nCount<=TARGET_MAX_NUM);
    m_Reader.read(
        (char*)pTemp->Tar,
        sizeof(WPG_Target)*DataHead.wTargetNum);
    m_IVSomeData.insert( PlayBackGroupTarget(DataHead.t, pTemp) );
    return TRUE;
}

CIVPlaybackDataBuf::TGroupTarget* CIVPlaybackDataBuf::ChannelTarget::Find(
    const FILETIME& time )
{
    if ( time < m_BeginTime ||
         time > m_EndTime )
    {
        return NULL;
    }

    if ( time < m_BufStartTime ||
         time > m_BufEndTime )
     {
        if ( !MoveToAndReadSome(time) )
        {
            return NULL;
        }
    }

    set<PlayBackGroupTarget>::iterator iter = m_IVSomeData.find(
        PlayBackGroupTarget(time,NULL) );
    if ( iter == m_IVSomeData.end() )
    {
        return NULL;
    }

    return &(*iter);
}

void CIVPlaybackDataBuf::ChannelTarget::ClearTempData()
{
    set<PlayBackGroupTarget>::iterator iter;
    for ( iter = m_IVSomeData.begin(); 
          iter!= m_IVSomeData.end();
          ++iter )
    {
        iter->m_TargetQueue->Release();
    }
    m_IVSomeData.clear();
}
// }
// CIVPlaybackDataBuf::ChannelTarget


// End of file