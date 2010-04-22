/**CPP*************************************************************************
 File            : IVPlaybackViewer.cpp
 Subsystem       : 
 Function Name(s): CIVPlaybackViewer
 Author          : YiQiu
 Date            : 2010-4-14  
 Time            : 15:39
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVPlaybackViewer.h"

IIVDataBuf* CIVPlaybackViewer::s_pIVDataBuf = CIVPlaybackDataBuf::getInstancePtr();


CIVPlaybackViewer::CIVPlaybackViewer(void)
    : m_PlayBackMode(Dir__)
    , m_NeedClearBuf(FALSE)
{
}

CIVPlaybackViewer::~CIVPlaybackViewer(void)
{
}

BaseTargetQueue* CIVPlaybackViewer::GetIVData(
    int nChannelID,
    const FILETIME& time )
{
    return s_pIVDataBuf->GetData(nChannelID,time);
}

void CIVPlaybackViewer::RefrehPoint( 
    ChannelPoint& PointBuf, 
    const BaseTargetQueue* DataQueue )
{
    if ( m_NeedClearBuf )
    {
        PointBuf.clear();
        m_NeedClearBuf = FALSE;
    }

    if ( m_PlayBackMode == Dir__ )
    {
        return CBaseIVViewer<IIVPlayBackViewer>::RefrehPoint(
            PointBuf,
            DataQueue);
    }

    ChannelPoint PointTmpBuf;
    ChannelPoint::iterator iter;
    for (int i=0; i<DataQueue->nCount; ++i)
    {
        // 如果有就利用原来的那个指针，插入数据
        // 并将数据放入临时的ChannelPoint中
        const WPG_Target& tar = DataQueue->Tar[i];
        iter = PointBuf.find(tar.equalId);
        PointList* pPointList  = NULL;
        if ( iter != PointBuf.end() )
        {
            pPointList = iter->second;
            pPointList->push_back(tar.centroid);
            PointBuf.erase(iter);
        }
        else
        {
            // 没找到就直接new一个新的内存
            pPointList = new PointList();

            ++g_PointList;
            pPointList->push_back(tar.centroid);
        }

        assert(PointTmpBuf[tar.equalId]==NULL);
        PointTmpBuf[tar.equalId] = pPointList;
    }

    // 释放那些目标丢失的数据
    for ( iter = PointBuf.begin();
        iter!= PointBuf.end();
        ++iter )
    {
        --g_PointList;
        delete iter->second;
    }
    PointBuf.clear();
    PointBuf = PointTmpBuf;
}

void CIVPlaybackViewer::SetPlayBackMode( PlayBack_Mode Mode )
{
    if ( m_PlayBackMode == Mode )
    {
        return;
    }
    m_PlayBackMode = Mode;
    m_NeedClearBuf = TRUE;
}


// End of file