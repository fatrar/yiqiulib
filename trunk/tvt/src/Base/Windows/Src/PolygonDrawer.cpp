/*H***************************************************************************
 File            : PolygonDrawer.cpp
 Subsystem       : 
 Function Name(s): CPolygonDrawer
 Author          : YiQiu
 Date            : 2010-2-8  
 Time            : 15:04
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "PolygonDrawer.h"

//IMPLEMENT_DYNAMIC(CPolygonDrawer, CWnd)

CPolygonDrawer::CPolygonDrawer(CWnd* pWnd)
    : m_bFirst(true)
{
    m_pWnd = pWnd;
    m_nMaxPoint =  Polygon_Default_Max_Point;
    m_PointQueue.push_back(CPoint());
}

CPolygonDrawer::~CPolygonDrawer(void)
{
}

//BEGIN_MESSAGE_MAP(CPolygonDrawer, CWnd)
//    ON_WM_MOUSEMOVE()
//    ON_WM_LBUTTONUP()
//    ON_WM_LBUTTONDOWN()
//    ON_WM_PAINT()
//END_MESSAGE_MAP()

void CPolygonDrawer::SendCommond(DrawCommond c, void* p1, void* p2)
{
    switch (c)
    {
    case Set_Max_Point:
        size_t nMaxPoint = (size_t)p1;
        if ( nMaxPoint >= 3 )
        {
            m_nMaxPoint = (size_t)p1;
        }  
        break;
    }
}

BOOL CPolygonDrawer::OnMouseMove(UINT nFlags, CPoint& point)
{
    if ( !(nFlags&MK_LBUTTON) )
    {
        return FALSE;
    }

    do 
    {
        if ( m_bDragging && m_nDragIndex != -1 )
        {
            m_PointQueue[m_nDragIndex] = point;
            break;
        }

        if ( m_bDrawing )
        {
            m_PointQueue.back() = point;
            break;
        }

        if ( m_bDragCenter )
        {
            CenterPointMoveTo(point);
            break;
        }

        return FALSE;
    }
    while (0);
    
    Redraw();
    return TRUE;
}

BOOL CPolygonDrawer::OnLButtonUp(UINT nFlags, CPoint& point)
{
    if ( m_bDrawing ) 
    {
        return TRUE;
    }

    if ( m_bDragging )
    {
        //UnLockCursor();
        m_bDragging = false;
        m_nDragIndex = -1;
        return TRUE;
    }

    if ( m_bDragCenter )
    {
        //UnLockCursor();
        CenterPointMoveTo(point);
        m_bDragCenter = false;
        return TRUE;
    }
    return FALSE;
}

// 注意这里要做最大多边形的判断，不能让用户无限的点下去
BOOL CPolygonDrawer::OnLButtonDown(UINT nFlags, CPoint& point)
{
    CRect Rect;
    m_pWnd->GetClientRect(&Rect);
    m_pWnd->ClientToScreen(&Rect);

    if ( m_bIsOK )
    {
        if ( IsDargPoint(point) )
        {
            //LockCursor(Rect);
            m_bDragging = true;
            return TRUE;
        }
        else if ( IsDargCenterPoint(point) )
        {
            m_bDragging = false;
            //LockCursor(Rect);
            m_bDragCenter = true;
            return TRUE;
        }
 
        m_bDragging = false;
        return FALSE;
    }
    else
    {
        if ( m_bFirst )
        {
            //LockCursor(Rect);
            m_bFirst = false;
        }

        // 如果超过最大点数或者用户连接了起始点
        size_t nPointCount = m_PointQueue.size();
        if ( nPointCount >= m_nMaxPoint ||
             (nPointCount >= 3 && Distance(point, m_PointQueue.front()) <= Point_Radii) )
        {
            //UnLockCursor();
            m_PointQueue.pop_back();
            //ReFreshPoint(m_PointQueue.size()-1, point);
            m_bDrawing = false;
            m_bIsOK = true;
            Redraw();
            AfxMessageBox(_T("Is OK!"));
            return TRUE;
        }
    }

    m_PointQueue.back() = point;
    m_PointQueue.push_back(point);
    m_bDrawing = true;
    m_bDragging = false;
    Redraw();
    return TRUE;
}

void CPolygonDrawer::OnPaint(CDC& dc, BOOL bSelect)
{
    //if (!m_bIsOK && !m_bDrawing && )
    //{
    //    return;
    //}
    size_t nSize = m_PointQueue.size();
    if ( nSize == 0 )
    {
        return;
    }
   
    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectObject(&m_Brush);

    deque<CPoint>::iterator iter = m_PointQueue.begin();
    if ( bSelect )
    {    
        for  ( ; iter != m_PointQueue.end(); ++iter )
        {
            CPoint& point = *iter;
            DrawSquare(&dc, point, Point_Radii);
        }
    }
   
    if ( nSize == 1 )
    {
        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush); 
        return;
    }
 
    iter = m_PointQueue.begin();
    CPoint& BeginPoint = *iter;
    dc.MoveTo(BeginPoint);
    for  ( ++iter; iter != m_PointQueue.end(); ++iter )
    {
        dc.LineTo(*iter);
    }

    if ( m_bIsOK )
    {
        dc.LineTo(BeginPoint);

        if ( bSelect )
        {
            DrawCenterPoint(&dc);
        }
    }

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);  
}
//CRgn
// End of file