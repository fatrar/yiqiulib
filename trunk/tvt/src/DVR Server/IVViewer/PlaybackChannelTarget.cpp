/**CPP*************************************************************************
 File            : PlaybackChannelTarget.cpp
 Subsystem       : 
 Function Name(s): CIVPlaybackDataBuf::ChannelTarget
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
#include "ChannelTarget.h"

static const FILETIME s_ErrFileTime = {0, 0}; 

//
// ************ CIVPlaybackDataBuf::ChannelTarget ************
// {

    // __forceinline
#ifdef _FixIVFile
HANDLE CIVPlaybackDataBuf::ChannelTarget::s_hFixFileThread = NULL;
DWORD CIVPlaybackDataBuf::ChannelTarget::s_dwFixFileThread = -1;

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

CIVPlaybackDataBuf::ChannelTarget::ChannelTarget()
    : m_nBufLastPos(0)
{
    m_pTargetBuf = new PlayBackTargetQueue[Head_Index_Interval];
};

CIVPlaybackDataBuf::ChannelTarget::~ChannelTarget()
{
    safeDeleteArray(m_pTargetBuf);
};

BOOL CIVPlaybackDataBuf::ChannelTarget::ParseFile()
{
    IVFileHeadFrist FirstHead;
    m_Reader.read((char*)&FirstHead, sizeof(FirstHead));
    if ( FirstHead.FileFlag != g_dwIVFileOK )
    {
        return FALSE;
    }

// 有没有一种可以 等于 变量类型的 方法？？？？ 
    //m_Reader.seekg(0);
    switch ( FirstHead.Version )
    {
    case IVFile_Version_1_0:
        return ParseHeadToMap<IVFile_Version_1_0>();
    case IVFile_Version_2_0:
        return ParseHeadToMap<IVFile_Version_2_0>();
    default:
    	return FALSE;
    }
}

template<>
void CIVPlaybackDataBuf::ChannelTarget::PushFileIndexDataToMemory<IVFile_Version_1_0>(
    IVFileHead<IVFile_Version_1_0>& Head)
{
    StlHelper::Array2STL(
        Head.DataIndex, 
        Head.dwIndexNum, 
        m_DataIndex );
}

template<>
void CIVPlaybackDataBuf::ChannelTarget::PushFileIndexDataToMemory<IVFile_Version_2_0>(
    IVFileHead<IVFile_Version_2_0>& Head)
{
    StlHelper::Array2STL(
        Head.DataIndex, 
        Head.wIndexNum,
        m_DataIndex );
    if ( Head.wTailIndexNum == 0 )
    {
        return;
    }

    size_t nTailLen = Head.wTailIndexNum*sizeof(IVFileDataIndex);
    m_Reader.seekg(-1*nTailLen, ios::end);
    IVFileDataIndex* pTmpBuf = NULL;
    if ( Head.wTailIndexNum > Max_IVData_Index )
    {
        pTmpBuf = new IVFileDataIndex[Head.wTailIndexNum];
    }
    else
    {
        pTmpBuf = Head.DataIndex;
    }

    m_Reader.read((char*)pTmpBuf, nTailLen);
    StlHelper::Array2STL(
        pTmpBuf, 
        Head.wTailIndexNum,
        m_DataIndex );
    if ( pTmpBuf != Head.DataIndex )
    {
        delete[] pTmpBuf;
    }

    m_Reader.seekg(sizeof(Head), ios::beg);
}

template<IVFileVersionDefine Version>
BOOL CIVPlaybackDataBuf::ChannelTarget::ParseHeadToMap()
{
    IVFileHead<Version> Head;
    m_Reader.read(
        (char*)&Head+sizeof(IVFileHeadFrist),
        sizeof(Head)-sizeof(IVFileHeadFrist));

    m_BeginTime = Head.BeginTime;
    m_EndTime = Head.EndTime;
    if ( m_BeginTime.GetTime() == 0 ||
         m_EndTime.GetTime() == 0 )
    {
        return FALSE;
    }

    struct PushFileDataToMemory
    {

    };

    m_DataIndex.clear();
    PushFileIndexDataToMemory(Head);

    IVFileDataIndex TmpDataIndex; 
    if ( m_DataIndex.size()==0 ||
         m_DataIndex.front().TimeOffset != 0 )
    {
        TmpDataIndex.TimeOffset = 0;
        TmpDataIndex.DataOffset = sizeof(Head);
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
    //IVFileDataIndex TmpDataIndex; 
    //DWORD EndTimeOffset = (DWORD)(Head.EndTime - Head.BeginTime);
    //if ( m_DataIndex.size()==0 )
    //{
    //    TmpDataIndex.TimeOffset = 0;
    //    TmpDataIndex.DataOffset = sizeof(Head);
    //    m_DataIndex.push_front(TmpDataIndex);

    //    if ( Head.dwLastFramePos != sizeof(Head) )
    //    {
    //        TmpDataIndex.TimeOffset = EndTimeOffset;
    //        TmpDataIndex.DataOffset = Head.dwLastFramePos;
    //        m_DataIndex.push_back(TmpDataIndex);
    //    }
    //}
    //else
    //{
    //    if ( m_DataIndex.front().TimeOffset != 0 )
    //    {
    //        TmpDataIndex.TimeOffset = 0;
    //        TmpDataIndex.DataOffset = sizeof(Head);
    //        m_DataIndex.push_front(TmpDataIndex);
    //    }

    //    if ( m_DataIndex.back().TimeOffset != EndTimeOffset )
    //    {
    //        TmpDataIndex.TimeOffset = EndTimeOffset;
    //        TmpDataIndex.DataOffset = Head.dwLastFramePos;
    //        m_DataIndex.push_back(TmpDataIndex);
    //    }
    //}

    return TRUE;
}

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

    if ( !ParseFile() )
    {
        m_Reader.close();
        return FALSE;
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
    DWORD nTestTime = (DWORD)(time - m_BeginTime);
    size_t nQueueSize = m_DataIndex.size();
    for ( size_t i=0; i<nQueueSize; ++i )
    {
        IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
        if ( nTestTime == TmpDataIndex.TimeOffset )
        {
            return i;
        }

        if ( nTestTime > TmpDataIndex.TimeOffset )
        {
            if ( i == nQueueSize-1 )
            {
                return i;
            }

            if ( nTestTime < m_DataIndex[i+1].TimeOffset )
            {
                return i;
            }
            
            continue;
        }

        if ( nTestTime < TmpDataIndex.TimeOffset)
        {
            break;
        }
    }
    return size_t(-1);
    
    //double dGuessPercent = double(nTestTime)/double((FILETIME)m_EndTime-time);
    //size_t nGuessQueusPos = size_t(nQueueSize*dGuessPercent);
    //assert(nGuessQueusPos < nQueueSize);

    //IVFileDataIndex& GuessDataIndex = m_DataIndex[nGuessQueusPos];
    //if ( nGuessQueusPos == 0 )
    //{
    //    for ( size_t i=0; i<nQueueSize; ++i )
    //    {
    //        IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
    //        if ( nTestTime >= TmpDataIndex.TimeOffset  )
    //        {
    //            return i;
    //        }
    //    }
    //    return size_t(-1);
    //}

    //if ( nTestTime == GuessDataIndex.TimeOffset )
    //{
    //    return nGuessQueusPos;
    //}
    //else if ( nTestTime > GuessDataIndex.TimeOffset )
    //{
    //    for (size_t i=nGuessQueusPos-1; i>=0; --i)
    //    {
    //        IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
    //        if ( nTestTime <= TmpDataIndex.TimeOffset  )
    //        {
    //            return i;
    //        }
    //    }

    //    return size_t(-1);
    //}
    //else //( nTestTime < GuessDataIndex.TimeOffset )
    //{
    //    for (size_t i=nGuessQueusPos; i<nQueueSize; ++i)
    //    {
    //        IVFileDataIndex& TmpDataIndex = m_DataIndex[i];
    //        if ( nTestTime >= TmpDataIndex.TimeOffset  )
    //        {
    //            return i;
    //        }
    //    }

    //    return size_t(-1);
    //}

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
    SYSTEMTIME syt;
    BOOL bRc = FileTimeToSystemTime(&DataHead.t, &syt);
    if ( bRc )
    {
        bRc = IsValidSystemTime(syt);
    }
    if ( !bRc ||
         DataHead.wTargetNum > TARGET_MAX_NUM ||
         DataHead.dwPrePos > DataHead.dwNextPos )
    {
        ASSERT(FALSE);
        return FALSE;
    }
    
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
    m_Reader.read(
        (char*)pTemp->Tar,
        sizeof(WPG_Target)*DataHead.wTargetNum);
    m_IVSomeData.insert( PlayBackGroupTarget(DataHead.t, pTemp) );
    return TRUE;
}


CIVPlaybackDataBuf::TGroupTarget* CIVPlaybackDataBuf::ChannelTarget::Find(
    const FILETIME& time )
{
    if ( !m_Reader.is_open() )
    {
        return NULL;
    }

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