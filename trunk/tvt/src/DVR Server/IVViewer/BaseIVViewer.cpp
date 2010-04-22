/*CPP*************************************************************************
File            : BaseIVViewer.cpp
Subsystem       : 
Function Name(s): CBaseIVViewer

Description     : 
Author          : Yiqiu
Date            : 2010/01/18
Revision        : 

History
-------


Copyright (c) xx Tech Co.,Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "BaseIVViewer.h"


template CBaseIVViewer<IIVLiveViewerEx>;
template CBaseIVViewer<IIVPlayBackViewer>;

template<typename TViewer>
CBaseIVViewer<TViewer>::CBaseIVViewer(void)
{
    m_pViewerBuf = new ViewerBuf[Max_Device_Num];
    m_hPen = ::CreatePen(PS_SOLID, Line_Default_Width, Point_Default_Color);
    m_hBrush = ::GetStockObject(NULL_BRUSH);
    ZeroMemory(m_dwRecord, sizeof(m_dwRecord));
}

template<typename TViewer>
CBaseIVViewer<TViewer>::~CBaseIVViewer(void)
{
    safeDeleteArray(m_pViewerBuf);

    DebugOut("g_PointList = %d\n", g_PointList);
    ::DeleteObject(m_hPen);
}

template<typename TViewer>
BOOL CBaseIVViewer<TViewer>::Paint(
    int nChannelID,
    const HDC dc, 
    const RECT& rect, 
    const FILETIME& time )
{
    if ( NULL == dc )
    {
        return FALSE;
    }
  
    WPG_Target* pTarBuf = NULL;
    int nTarCount = 0;
    int nDeviceID = nChannelID/Every_Device_Channel;

    /**
    *@note 1.得到目标数据及对新目标处理
    */
    BaseTargetQueue* DataQueue = GetIVData(nChannelID, time);
    if ( DataQueue == NULL )
    {  
        if ( nChannelID != m_pViewerBuf[nDeviceID].ChannelID )
        {
            //m_pViewerBuf[nDeviceID].ChannelID = -1;
            return FALSE;
        }
        
        DWORD dwNow = GetTickCount();
        if ( dwNow - m_dwRecord[nChannelID] > Draw_Invaild_Time )
        {
            return FALSE;
        }
    }
    else
    {
        m_dwRecord[nChannelID] = GetTickCount();
        m_pViewerBuf[nDeviceID].ChannelID = nChannelID;

        /**
        *@note 有的新的目标数据，刷新目标及增加轨迹数据
        */
        ViewerBuf& viewBuf = m_pViewerBuf[nDeviceID];
        viewBuf.ChannelID = nChannelID;
        viewBuf.time = time;
        memcpy(viewBuf.TarBuf, DataQueue->Tar, DataQueue->nCount*sizeof(WPG_Target));
        viewBuf.nTarCount = DataQueue->nCount;
        RefrehPoint(m_pViewerBuf[nDeviceID].PointBuf, DataQueue);
        DataQueue->Release();
    }
    pTarBuf = m_pViewerBuf[nDeviceID].TarBuf;
    nTarCount = m_pViewerBuf[nDeviceID].nTarCount;


    /**
    *@note 2. 画目标和轨迹
    */
    HGDIOBJ hOldPen = ::SelectObject(dc, m_hPen);  
    TShowState& ShowState = m_ShowState[nChannelID];
    if ( ShowState.bShow && (ShowState.nState & Show_Object) )
    {
        DrawObject(dc, pTarBuf, nTarCount, rect);
    }

    if ( ShowState.bShow && (ShowState.nState & Show_Trace) )
    {
        DrawTrace(dc, m_pViewerBuf[nDeviceID].PointBuf, rect);
    } 
    ::SelectObject(dc, hOldPen);
    return TRUE;
}

template<typename TViewer>
void CBaseIVViewer<TViewer>::SetObjTraceShowState( int nChannelID, bool bShow )
{
    m_ShowState[nChannelID].bShow = bShow;
}

template<typename TViewer>
void CBaseIVViewer<TViewer>::GetObjTraceShowState( int nChannelID, bool& bShow )
{
    bShow = m_ShowState[nChannelID].bShow;
}

template<typename TViewer>
void CBaseIVViewer<TViewer>::SetDataShowState( int nChannelID, int nState )
{
    m_ShowState[nChannelID].nState = nState;
}

template<typename TViewer>
void CBaseIVViewer<TViewer>::GetDataShowState( int nChannelID, int& nState )
{
    nState = m_ShowState[nChannelID].nState;
}


// 好像这部分有内存泄漏，要具体查下。
template<typename TViewer>
void CBaseIVViewer<TViewer>::RefrehPoint( 
    ChannelPoint& PointBuf, 
    const BaseTargetQueue* DataQueue )
{
    ChannelPoint PointTmpBuf;
    ChannelPoint::iterator iter;
    for (int i=0; i<DataQueue->nCount; ++i)
    {
        // 如果有就利用原来的那个指针，插入数据
        // 并将数据放入临时的ChannelPoint中
        const WPG_Target& tar = DataQueue->Tar[i];
        iter = PointBuf.find(tar.equalId);
        PointInfo* pPointInfo  = NULL;
        if ( iter != PointBuf.end() )
        {
            pPointInfo = iter->second;
            pPointInfo->PointQueue.push_back(tar.centroid);
            pPointInfo->nPreviousID = tar.equalId;
            PointBuf.erase(iter);
        }
        else
        {
            // 没找到就直接new一个新的内存
            pPointInfo = new PointInfo();
           
            ++g_PointList;
            pPointInfo->PointQueue.push_back(tar.centroid);
            pPointInfo->nPreviousID = tar.equalId;
        }

        assert(PointTmpBuf[tar.equalId]==NULL);
        PointTmpBuf[tar.equalId] = pPointInfo;
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

template<typename TViewer>
void CBaseIVViewer<TViewer>::DrawTrace( 
    const HDC dc,
    const ChannelPoint& PointBuf,
    const RECT& rect  )
{
    POINT p;
    for ( ChannelPoint::const_iterator iter = PointBuf.begin();
          iter != PointBuf.end();
          ++iter )
    {
        PointInfo* pPointInfo = iter->second;
        if ( pPointInfo == NULL )
        {
            // Trace;
            continue;
        }

        PointList& PointList = pPointInfo->PointQueue;
        if ( PointList.size() == 0 ||
             PointList.size() == 1 )
        {
            continue;
        }

        PointList::iterator listIter = PointList.begin();
        int x = int(listIter->x * (rect.right-rect.left)) + rect.left;
        int y = int(listIter->y * (rect.bottom-rect.top)) + rect.top;
        MoveToEx(dc, x, y, &p);
        for ( ++listIter; listIter != PointList.end();
              ++listIter )
        {
            x = int(listIter->x * (rect.right-rect.left)) + rect.left;
            y = int(listIter->y * (rect.bottom-rect.top)) + rect.top;
            LineTo(dc, x, y);
        }
    }
}

template<typename TViewer>
void CBaseIVViewer<TViewer>::DrawObject(
    const HDC dc,
    WPG_Target* pTarBuf,
    size_t nTarCount,
    const RECT& rect )
{
    HGDIOBJ hOldBrush = ::SelectObject(dc, m_hBrush);
    //RECT ObjRect;
    int left, top, right, bottom;
    for (size_t i =0; i<nTarCount; ++i)
    {
        WPG_Target& Tar = pTarBuf[i];

        // 过滤超过视频窗口的目标，
        // 不能在SDK和Buf数据接收事过滤，不然会造成目标丢失
        if ( !ViewHelper::IsVaildWPGTarget(Tar) )
        {
            continue;
        }

        ViewHelper::TranslateToRect(rect,Tar.boundingBox, left, top, right, bottom);
        ::Rectangle(dc, left, top, right, bottom);
    }
    ::SelectObject(dc, hOldBrush);
}

// End fo file

