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

            // ��ֹ����������Start
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
    *@note 1. �ж��ļ��Ƿ�򿪣�
    *   NO , �Ƿ����µ�Path������û������һЩ����PreAlarm�����ݣ�
    *         ���򴴽����ļ���дPreAlarm�����ݣ�
    *            �ж��ļ��Ƿ���Close��ǣ����򱣴����ݵ�Closeʱ��
    *   Yes, ֱ��дPreAlarm�����ݣ��ж��ļ��Ƿ���Close��ǣ����򱣴����ݵ�Closeʱ��
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
*@note ��������㷨�Ƚϸ��ӣ���Ҫ��֤
*   ����㷨ԭ��������һ���ļ�ֻ��һ�����Σ������ļ������кܶ���Σ�����Ч���ͺ��ˣ�
*  ���Ը���ÿ���εĸ����������㶪��ֵ������ֻ����һ���δ�����ʱ����
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
    *@note 1. ��IV�ļ�ͷEndTime��ֵ��ע�����ֵ���ϲ㴫��������Ƶ�ļ����һ֡ʱ��
    *    ��Writer�ļ�ָ���Ƶ���ʼ�������Ϳ���ֱ�Ӹ����ļ��򿪵��Ǹ�
    *    ��MoreDataIndex�����ݣ���ô����Ҫ��FileHead��MoreDataIndex������������ƽ��������
    *    Ȼ�󽫵������ʱ�����FileHead��
    *    MoreDataIndexû���ݣ���ôֱ��дFileHead�ͺ�
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
        sizeof(IVFileHead<dwIVFileVersion>) );
}

template<DWORD dwIVFileVersion>
void CIVLiveDataBuf::ChannelTarget<dwIVFileVersion>::SaveByFileIsSetClose(
    int nPreAlarmTime,
    FileInfo& Info )
{
    /**
    *@note 1. ���û�еĴ�������ݶ���Ϣ����ֱ��ˢ���������ر��ļ�
    */
    if ( Info.UnFinishSection.empty() )
    {
        if ( Writer.is_open() )
        {
            // ���ȸ����ļ�������Ȼ��ر��ļ���ɾ����ǰʹ���ļ���Ϣ
            UpdateDataIndexToFile(Info);
            // ������һ������ǣ��ļ�·�����ˣ�����һֱû��ʼ����¼
            // ����·����û�ж���Ϣ��
            // ���ʱ����Ҫ���������ڼ����ݣ��ļ�Ҳû����
            Writer.close();
        }    
        FilePathList.pop_front();
        return;
    }

    /**
    *@note 2. ���滹û�б�����Ķ���Ϣ������ 
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    string& strPath = Info.strPath;
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
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
        *@note 2.2. �쳣�����ж϶���Ϣʱ���Ƿ�����
        *           �����������Ϊ0,
        *  ���Ϊ�쳣����������ȡ��һ��
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
        *@note 2.3. �ж�����Ҫ���浽��ʱ��������ʱ��Ĺ�ϵ
        *  if > ���ݴ��ڱ����ʱ����ȡ��һ������Ϣ��û����һ������ر��ļ�
        *  if =  ���浱ǰ���������ݣ�Ȼ��ر��ļ�
        *  if <  �������ݣ�����ѭ��
        */
        long nState = pGroupTarget->m_time ^ SectionTime.EndTime;
        if ( nState > 0 )
        {
            /**
            *@note 2.3.1 ��ʱ��������������ʱ�䣬Ȼ�󽫶���Ϣ�Ž�������Ķ����
            *  �ж��Ƿ���δ����ĶΣ�û�о����ļ��رռ�������ļ�����
            *  ����ȡ��һ���Σ�����ѭ��
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
        *@note 2.3.2 С�ڵ��ڣ��򱣴�����
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead, 
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.3.3. ��β(�ͷŻ�������ã�ɾ���ڵ�)
        */
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);

        if ( nState == 0 )
        {
            /**
            *@note 2.3.4. �պ���ȣ��򽫵�ǰ����Ϣ�Ž�������Ķ����
            *  �ж��Ƿ���δ����ĶΣ�û�о����ļ��رռ�������ļ�����
            *  ����ȡ��һ���Σ�����ѭ��
            */
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                // ʹ������������ʱ��
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
    *@note 1. ���û�еĴ�������ݶ���Ϣ��������������
    */
    if ( Info.UnFinishSection.empty() )
    {
        DropSomeData(nPreAlarmTime, Now);
        return;
    }

    /**
    *@note 2. ���滹û�б�����Ķ���Ϣ������
    */
    IVFileDataHead DataHead = {0};
    BOOL bNeedFlush = FALSE;
    FileSectionTime& SectionTime = Info.UnFinishSection.front();
    string& strPath = Info.strPath;  
    for ( TTargetList::iterator iter = TargetSaveList.begin();
        iter!= TargetSaveList.end(); )
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
        *@note 2.2. �쳣�����ж϶���Ϣ�Ŀ�ʼʱ���Ƿ�����
        *           ���ٿ�ʼʱ�䲻Ϊ0,
        *           ���Ϊ�쳣����������ȡ��һ��
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
        *@note 2.2. ����ö������ý���ʱ�䣬�򱣴浽�ý���ʱ�䣬
        *           Ȼ��ȡ��һ����
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

            // �н���ʱ�䣬��ʱ���������ʱ��,���ñ�־û��Ҫ�ж�Ԥ¼��ʱ��
            bMustSave = TRUE;
        }

        if ( !bMustSave )
        {
            /**
            *@note 2.3. �ж������Ƿ�ʱ��������Ԥ¼���ʱ��, 
            *  ��û�����ý���ʱ�䣬���Ǳ������ݱ��볬��Ԥ¼���ʱ��
            */
            __int64 nBetween = Now - pGroupTarget->m_time;
            if ( nBetween/1000 < nPreAlarmTime )
            {
                break;
            }
        }

        /**
        *@note 2.4. ��������
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead,
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.5. ��β(�ͷŻ�������ã�ɾ���ڵ�)
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
    // ������δ���ü�live���ŵ����ݷŽ��������
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
    *@note 1. ���û�еĴ�������ݶ���Ϣ����ֱ��ˢ���������ر��ļ�
    */
    if ( Info.UnFinishSection.empty() )
    { 
        if ( Writer.is_open() )
        {
            // ���ȸ����ļ�������Ȼ��ر��ļ���ɾ����ǰʹ���ļ���Ϣ
            UpdateDataIndexToFile(Info);
            // ������һ������ǣ��ļ�·�����ˣ�����һֱû��ʼ����¼
            // ����·����û�ж���Ϣ��
            // ���ʱ����Ҫ���������ڼ����ݣ��ļ�Ҳû����
            Writer.close();
        }    
        return;
    }

    /**
    *@note 2. ���滹û�б�����Ķ���Ϣ������ 
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
        *@note 2.2. �쳣�����ж϶���Ϣʱ���Ƿ�����
        *           �����������Ϊ0,
        *  ���Ϊ�쳣����������ȡ��һ��
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
        *@note 2.3. �ж�����Ҫ���浽��ʱ��������ʱ��Ĺ�ϵ
        *  if > ���ݴ��ڱ����ʱ����ȡ��һ������Ϣ��û����һ������ر��ļ�
        *  if =  ���浱ǰ���������ݣ�Ȼ��ر��ļ�
        *  if <  �������ݣ�����ѭ��
        */
        long nState = pGroupTarget->m_time ^ SectionTime.EndTime;
        if ( nState > 0 )
        {
            /**
            *@note 2.3.1 ��ʱ��������������ʱ�䣬Ȼ�󽫶���Ϣ�Ž�������Ķ����
            *  �ж��Ƿ���δ����ĶΣ�û�о����ļ��رռ�������ļ�����
            *  ����ȡ��һ���Σ�����ѭ��
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
        *@note 2.3.2 С�ڵ��ڣ��򱣴�����
        */
        SaveGroupDataToFile(
            Info,
            SectionTime,
            DataHead,
            pGroupTarget);
        bNeedFlush = TRUE;

        /**
        *@note 2.3.3. ��β(�ͷŻ�������ã�ɾ���ڵ�)
        */
        pGroupTarget->m_TargetQueue->nUse = Buf_No_Use;
        pGroupTarget->m_TargetQueue->nRef = 0;
        delete pGroupTarget;
        TargetSaveList.erase(iter);

        if ( nState == 0 )
        {
            /**
            *@note 2.3.4. �պ���ȣ��򽫵�ǰ����Ϣ�Ž�������Ķ����
            *  �ж��Ƿ���δ����ĶΣ�û�о����ļ��رռ�������ļ�����
            *  ����ȡ��һ���Σ�����ѭ��
            */
            Info.FinishSection.push_back(SectionTime);
            Info.UnFinishSection.pop_front();
            bIsFirstFrame = TRUE;

            if ( Info.UnFinishSection.empty() )
            {
                // ʹ������������ʱ��
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