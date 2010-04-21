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
    *@note if Find Time > Buffer list[i]��Move list 0-i to Save list,return NULL
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
    *@note ���������ϰ��ҵ�����ģ�ͣ�
    * ���Ǳ�������TargetSaveList�����ݶ�Ҫ����Release
    * ����Ϊ��Ч�ʿ��� ���ݱ����߳������⴦��
    * ��Ϊû�б��ϲ�ʹ�õ����ݿ϶���û�е���Use���Ӷ�nUse��־ΪBuf_No_Use
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

bool CIVLiveDataBuf::ChannelTarget::FileClose(
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

void CIVLiveDataBuf::ChannelTarget::DropSomeData(int nPreAlarmTime)
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
        *@note 1. �ж������Ƿ񻹱�����
        */
        TGroupTarget* pGroupTarget = *iter;
        if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
            pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
        {
            break;
        }

        /**
        *@note 2. �ж������Ƿ�ʱ��������Ԥ¼���ʱ��
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

void CIVLiveDataBuf::ChannelTarget::TrySaveData(int nPreAlarmTime)
{
    if ( TargetSaveList.size() == 0 )
    {
        return;
    }

    /**
    *@note 1. �ж��ļ��Ƿ�򿪣�
    *   NO , �Ƿ����µ�Path������û������һЩ����PreAlarm�����ݣ�
    *         ���򴴽����ļ���дPreAlarm�����ݣ�
    *            �ж��ļ��Ƿ���Close��ǣ����򱣴����ݵ�Closeʱ��
    *   Yes, ֱ��дPreAlarm�����ݣ��ж��ļ��Ƿ���Close��ǣ����򱣴����ݵ�Closeʱ��
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

void CIVLiveDataBuf::ChannelTarget::SaveData(
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
            *@note 1.1. �ж������Ƿ񻹱�����
            */
            TGroupTarget* pGroupTarget = *iter;
            if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
                 pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
            {
                break;
            }

            /**
            *@note 1.2. �ж�����Ҫ���浽��ʱ��������ʱ��Ĺ�ϵ
            *  if > ���ݴ��ڱ����ʱ���򣬹ر��ļ�
            *  if =  ���浱ǰ���������ݣ�Ȼ��ر��ļ�
            *  if <  �������ݣ�����ѭ��
            */
            long nState = pGroupTarget->m_time ^ Info->CloseTime;
            if ( nState > 0 )
            {
                // ʹ������������ʱ��
                UpdateDataIndexToFile(pGroupTarget->m_time);
                Writer.close();
                UpdateFileInfo();
                return;
            }

            /**
            *@note 1.3. ��������
            */
            SaveDataHeadToFile(
                DataHead,
                pGroupTarget->m_TargetQueue->nCount,
                pGroupTarget->m_time);
            SaveTargetToFile(pGroupTarget->m_TargetQueue);
            bNeedFlush = TRUE;

            /**
            *@note 1.4. ��β(�ͷŻ�������ã�ɾ���ڵ�)
            */
            pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
            pGroupTarget->m_TargetQueue->nRef = 0;
            delete pGroupTarget;
            TargetSaveList.erase(iter);

            if ( nState == 0 )
            {
                UpdateDataIndexToFile(Info->CloseTime);
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
            *@note 2.1. �ж������Ƿ񻹱�����
            */
            TGroupTarget* pGroupTarget = *iter;
            if ( pGroupTarget->m_TargetQueue->nRef != 0 &&
                pGroupTarget->m_TargetQueue->nUse != Buf_No_Use )
            {
                break;
            }

            /**
            *@note 2.2. �ж������Ƿ�ʱ��������Ԥ¼���ʱ��
            *  (��������������ڵ����⴦���Ͼ���������ڵ�����ǻ��������ܵ�)
            */
            __int64 nBetween = Now - pGroupTarget->m_time;
            if ( nBetween/1000 < nPreAlarmTime )
            {
                break;
            }

            /**
            *@note 2.3. ��������
            */
            SaveDataHeadToFile(
                DataHead,
                pGroupTarget->m_TargetQueue->nCount,
                pGroupTarget->m_time);
            SaveTargetToFile(pGroupTarget->m_TargetQueue);
            bNeedFlush = TRUE;

            /**
            *@note 2.4. ��β(�ͷŻ�������ã�ɾ���ڵ�)
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
*@note ��������㷨�Ƚϸ��ӣ���Ҫ��֤
*/
void CIVLiveDataBuf::ChannelTarget::UpdateDataIndexToFile(
    const FILETIME& EndTime)
{
    /**
    *@note 1. ��IV�ļ�ͷEndTime��ֵ��ע�����ֵ���ϲ㴫��������Ƶ�ļ����һ֡ʱ��
    *    ��Writer�ļ�ָ���Ƶ���ʼ�������Ϳ���ֱ�Ӹ����ļ��򿪵��Ǹ�
    *    ��MoreDataIndex�����ݣ���ô����Ҫ��FileHead��MoreDataIndex������������ƽ��������
    *    Ȼ�󽫵������ʱ�����FileHead��
    *    MoreDataIndexû���ݣ���ôֱ��дFileHead�ͺ�
    */
    FileHead.EndTime = EndTime;
    FileHead.dwLastFramePos = dwPrePos;
    Writer.seekp(0);
    size_t nQueueSize = MoreDataIndex.size();
    if ( nQueueSize != 0 )
    {
        /**
        *@note 2. д���FileHead��MoreDataIndex�ֱ�Ҫ������
        */
        IVFileDataIndex (&DataIndex)[Max_IVData_Index] = FileHead.DataIndex;
        size_t nArrayDrop = 
            (Max_IVData_Index*nQueueSize)/(Max_IVData_Index+nQueueSize);
        size_t nQueueDrop = nQueueSize - nArrayDrop;

        /**
        *@note 3. ����FileHead�Ķ���ֵ������ArrayDropTableMgr�õ�������
        *         Ȼ����ݶ�������������Ҫ�������ƶ�����ʹ������
        */
        ArrayDropTableMgr* p = ArrayDropTableMgr::getInstancePtr();
        ArrayDropTable* t = p->GetDropTable(nArrayDrop);
        size_t nSize = t->size();
        size_t nIndex = 0;
        assert( nSize != 0 && nSize == nArrayDrop );
        {       
            /**
            *@note 3.1. ����ͷ����Ϊ����Ҫ�ƶ���ֱ�ӽ�nIndex�Ƶ�һ������λ��
            */
            ArrayDropTable::const_iterator iter1 = t->begin(),iter2=iter1; 
            nIndex = *iter1;

            /**
            *@note 3.2. �����м�Σ�ֻҪ����������λ�õ���������
            *           ���Ƶ���ÿ�δ����nIndex�ƶ������λ�þ���
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
            *@note 3.3. ����β�Σ�β����û�ж���λ�õĽ���ֵ��
            *           ��ô��Buf���Ⱦ���
            */
            for (BYTE i=*iter1; i<Max_IVData_Index-1; ++i,++nIndex)
            {
                DataIndex[nIndex] = DataIndex[i+1];
            } 
        }

        /**
        *@note 3. ����MoreDataIndex�Ķ���ֵ������QueueDropTableMgr�õ�������
        *         Ȼ�󽫲���Ҫ���������ݿ�����FileHead
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
        return FALSE;
    }

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
        m_DataIndex.push_front(TmpDataIndex);
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

    m_Reader.open(pPath, ios::binary|ios::in);
    if ( m_Reader.is_open() )
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
        m_Reader.close();
        return FALSE;
    }

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
    while ( m_Reader.tellg() < TmpDataIndex2.DataOffset )
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
            m_BufStartTime = CurrentFrameTime ;
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
    assert(nGuessQueusPos > nQueueSize);

    IVFileDataIndex& GuessDataIndex = m_DataIndex[nGuessQueusPos];
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
        for (size_t i=nGuessQueusPos+1; i<nQueueSize; ++i)
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
    // [] ������ҪУ��IVFileDataHead
    //
    /**
    *@note 1. ���Ƿ���Ŀ�����ݻ���(TargetQueue)
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

// }
// CIVPlaybackDataBuf::ChannelTarget


// End of file