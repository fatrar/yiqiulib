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



CBaseIVViewer::CBaseIVViewer(void)
{
    m_pViewerBuf = new ViewerBuf[Max_Device_Num];
}

CBaseIVViewer::~CBaseIVViewer(void)
{
    safeDeleteArray(m_pViewerBuf);

    DebugOut("g_PointList = %d\n", g_PointList);
}

inline void TranslateToRect(
    const RECT ClientRect,
    const WPG_RectangleF& WPG_Rect,
    int& left, int& top, int& right, int& bottom)
{
    int nWidth = ClientRect.right-ClientRect.left;
    int nHeight = ClientRect.bottom-ClientRect.top;
    left = int(nWidth*WPG_Rect.x) + ClientRect.left;
    top = int(nHeight*WPG_Rect.y) + ClientRect.top;
    right = int(nWidth*(WPG_Rect.x+WPG_Rect.width)) + ClientRect.left;
    bottom = int(nHeight*(WPG_Rect.y+WPG_Rect.height)) + ClientRect.top;
}

/**
@note 将画出窗口的目标过滤
*/
bool IsVaildWPGTarget(const WPG_Target& Data)
{
    const WPG_RectangleF& Rect = Data.boundingBox;
    if ( (Rect.height+Rect.y) > 1 ||
         (Rect.width+Rect.x) > 1 ||
         Rect.x > 1 ||
         Rect.y > 1 )
    {
        return false;
    }
    return true;
}

BOOL CBaseIVViewer::Paint(
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
    BOOL bNeedFresh = FALSE;
    int nDeviceID = nChannelID/Every_Device_Channel;

    TargetQueue* DataQueue = GetIVData(nChannelID, time);
    if ( DataQueue == NULL )
    {  
        if ( nChannelID != m_pViewerBuf[nDeviceID].ChannelID )
        {
            //m_pViewerBuf[nDeviceID].ChannelID = -1;
            return FALSE;
        }
        
        pTarBuf = m_pViewerBuf[nDeviceID].TarBuf;
        nTarCount = m_pViewerBuf[nDeviceID].nTarCount;
    }
    else
    {
        m_pViewerBuf[nDeviceID].ChannelID = nChannelID;
        pTarBuf = DataQueue->Tar;
        nTarCount = DataQueue->nCount;
        bNeedFresh = TRUE;
    }

    TShowState& ShowState = m_ShowState[nChannelID];
    if ( ShowState.bShow && (ShowState.nState& Show_Object) )
    {
        static HGDIOBJ hBrush = ::GetStockObject(NULL_BRUSH);
        HGDIOBJ hOldObj = ::SelectObject(dc, hBrush);
        //RECT ObjRect;
        int left, top, right, bottom;
        for (int i =0; i<nTarCount;++i)
        {
            WPG_Target& Tar = pTarBuf[i];

            // 过滤超过视频窗口的目标，
            // 不能在SDK和Buf数据接收事过滤，不然会造成目标丢失
            if ( !IsVaildWPGTarget(Tar) )
            {
                continue;
            }

            TranslateToRect(rect,Tar.boundingBox, left, top, right, bottom);
            ::Rectangle(dc, left, top, right, bottom);
        }
        ::SelectObject(dc, hOldObj);
    }

    if ( bNeedFresh )
    {
        ViewerBuf& viewBuf = m_pViewerBuf[nDeviceID];
        viewBuf.ChannelID = nChannelID;
        viewBuf.time = time;
        memcpy(viewBuf.TarBuf, DataQueue->Tar, DataQueue->nCount*sizeof(WPG_Target));
        viewBuf.nTarCount = DataQueue->nCount;
        RefrehPoint(m_pViewerBuf[nDeviceID].PointBuf, DataQueue, time);
        DataQueue->Release();
    }


    if ( ShowState.bShow && (ShowState.nState& Show_Trace) )
    {
        DrawTrace(dc, m_pViewerBuf[nDeviceID].PointBuf, rect);
    }
    
    return TRUE;
}

void CBaseIVViewer::SetObjTraceShowState( int nChannelID, bool bShow )
{
    m_ShowState[nChannelID].bShow = bShow;
}

void CBaseIVViewer::GetObjTraceShowState( int nChannelID, bool& bShow )
{
    bShow = m_ShowState[nChannelID].bShow;
}

void CBaseIVViewer::SetDataShowState( int nChannelID, int nState )
{
    m_ShowState[nChannelID].nState = nState;
}

void CBaseIVViewer::GetDataShowState( int nChannelID, int& nState )
{
    nState = m_ShowState[nChannelID].nState;
}

BOOL CBaseIVViewer::ResetStatistic( int nChannelID )
{
    return TRUE;
}

BOOL CBaseIVViewer::StartStatistic( int nChannelID, bool bFlag )
{
    return TRUE;
}

BOOL CBaseIVViewer::GetStatisticState( int nChannelID, bool& bFlag )
{
    return TRUE;
}

// 好像这部分有内存泄漏，要具体查下。
void CBaseIVViewer::RefrehPoint( 
    ChannelPoint& PointBuf, 
    const TargetQueue* DataQueue,
    const FILETIME& time )
{
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

void CBaseIVViewer::DrawTrace( 
    const HDC dc,
    const ChannelPoint& PointBuf,
    const RECT& rect  )
{
    POINT p;
    for ( ChannelPoint::const_iterator iter = PointBuf.begin();
          iter != PointBuf.end();
          ++iter )
    {
        PointList* pPointList = iter->second;
        if ( pPointList == NULL )
        {
            // Trace;
            continue;
        }

        if ( pPointList->size() == 0 ||
             pPointList->size() == 1 )
        {
            continue;
        }

        PointList::iterator listIter = pPointList->begin();
        int x = int(listIter->x * (rect.right-rect.left)) + rect.left;
        int y = int(listIter->y * (rect.bottom-rect.top)) + rect.top;
        MoveToEx(dc, x, y, &p);
        for ( ++listIter; listIter != pPointList->end();
              ++listIter )
        {
            x = int(listIter->x * (rect.right-rect.left)) + rect.left;
            y = int(listIter->y * (rect.bottom-rect.top)) + rect.top;
            LineTo(dc, x, y);
        }
    }
}


// End fo file

