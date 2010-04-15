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



GroupTarget* CIVLiveDataBuf::ChannelTarget::Find(
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
        GroupTarget* pGroupTarget = *iter;
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
            GroupTarget* pGroupTarget = *iter;
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
                UpdateDataIndexToFile(Info->CloseTime);
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
            GroupTarget* pGroupTarget = *iter;
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


//
// ************************* 
//
HANDLE CIVPlaybackDataBuf::ChannelTarget::s_hFixFileThread = NULL;
DWORD CIVPlaybackDataBuf::ChannelTarget::s_dwFixFileThread = -1;

template<>
BOOL CIVPlaybackDataBuf::ChannelTarget::PraseHeadToMap<IVFile_Version_1_0>(
    const IVFileHead& Head)
{
    DataIndex.clear();
    if ( Head.FileFlag != g_dwIVFileOK )
    {
        return FALSE;
    }

    StlHelper::Array2STL(
        Head.DataIndex, 
        Head.dwIndexNum, 
        DataIndex );

    IVFileDataIndex TmpDataIndex; 
    if ( DataIndex.size()==0 ||
         DataIndex.front().TimeOffset != 0 )
    {
        TmpDataIndex.TimeOffset = 0;
        TmpDataIndex.DataOffset = sizeof(IVFileHead);
        DataIndex.push_front(TmpDataIndex);
    }

    DWORD EndTimeOffset = (DWORD)(Head.EndTime - Head.BeginTime);
    if ( DataIndex.size()==0 || 
         DataIndex.back().TimeOffset != EndTimeOffset )
    {
        TmpDataIndex.TimeOffset = EndTimeOffset;
        TmpDataIndex.DataOffset = Head.dwLastFramePos;
        DataIndex.push_front(TmpDataIndex);
    }

    return TRUE;
}

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

BOOL CIVPlaybackDataBuf::ChannelTarget::Open(
    const char* pPath,
    const FILETIME& time)
{
    if ( isValidString(pPath) )
    {
        return FALSE;
    }

    if ( Reader.is_open() )
    {
        Reader.close();
    }

    Reader.open(pPath, ios::binary|ios::in);
    if ( Reader.is_open() )
    {
        return FALSE;
    }

    IVFileHead Head;
    Reader.read((char*)&Head, sizeof(IVFileHead));
    switch ( Head.Version )
    {
    case IVFile_Version_1_0:
        PraseHeadToMap<IVFile_Version_1_0>(Head);
        break;
    default:
        return FALSE;
    }
    return MoveTo(time);
}

BOOL CIVPlaybackDataBuf::ChannelTarget::Close(
    const FILETIME& time)
{
    if (Reader.is_open())
    {
        Reader.close();
        return TRUE;
    }

    return FALSE;
}

BOOL CIVPlaybackDataBuf::ChannelTarget::MoveTo(
    const FILETIME& time)
{
    if ( Reader.is_open() )
    {
        return FALSE;
    }

    if ( time < BeginTime ||
        time > EndTime )
    {
        Reader.close();
        return FALSE;
    }

    return TRUE;
}


DWORD WINAPI CIVPlaybackDataBuf::ChannelTarget::OnFixFileThread( void* p )
{
    return 0;
}




// End of file