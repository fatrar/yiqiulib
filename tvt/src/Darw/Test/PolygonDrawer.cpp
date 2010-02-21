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


IMPLEMENT_DYNAMIC(CPolygonDrawer, CWnd)

CPolygonDrawer::CPolygonDrawer(void)
    : m_bFirst(true)
{
    m_PointQueue.push_back(CPoint());
}

CPolygonDrawer::~CPolygonDrawer(void)
{
}

BEGIN_MESSAGE_MAP(CPolygonDrawer, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()


void CPolygonDrawer::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( !(nFlags&MK_LBUTTON) )
    {
        return;
    }

    if ( m_bDragging && m_nDragIndex != -1 )
    {
        m_PointQueue[m_nDragIndex] = point;
        ParentInvalidate();
        return;
    }

    if ( m_bDrawing )
    {
        m_PointQueue.back() = point;
        ParentInvalidate();
        return;
    }

    if ( m_bDragCenter )
    {
        CenterPointMoveTo(point);
        ParentInvalidate();
    }
}

void CPolygonDrawer::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_bDrawing ) 
    {
        return;
    }

    if ( m_bDragging )
    {
        UnLockCursor();
        m_bDragging = false;
        m_nDragIndex = -1;
        return;
    }

    if ( m_bDragCenter )
    {
        UnLockCursor();
        CenterPointMoveTo(point);
        m_bDragCenter = false;
    }
}

// 注意这里要做最大多边形的判断，不能让用户无限的点下去
void CPolygonDrawer::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect Rect;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);

    if ( m_bIsOK )
    {
        if ( IsDargPoint(point) )
        {
            LockCursor(Rect);
            m_bDragging = true;
            return;
        }
        else if ( IsDargCenterPoint(point) )
        {
            m_bDragging = false;
            LockCursor(Rect);
            m_bDragCenter = true;
            return;
        }
 
        m_bDragging = false;
        return;
    }
    else
    {
        if ( m_bFirst )
        {
            LockCursor(Rect);
            m_bFirst = false;
        }

        if ( m_PointQueue.size() >= 3 &&
             Distance(point, m_PointQueue.front()) <= Point_Radii )
        {
            UnLockCursor();
            m_PointQueue.pop_back();
            //ReFreshPoint(m_PointQueue.size()-1, point);
            m_bDrawing = false;
            m_bIsOK = true;
            ParentInvalidate();
            AfxMessageBox(_T("Is OK!"));
            return;
        }
    }

    m_PointQueue.back() = point;
    m_PointQueue.push_back(point);
    ParentInvalidate();
}

void CPolygonDrawer::OnPaint()
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

    CPaintDC dc(this);
    CGdiObject *pOldObject= dc.SelectStockObject(NULL_BRUSH);
    deque<CPoint>::iterator iter = m_PointQueue.begin();
    for  ( ; iter != m_PointQueue.end(); ++iter )
    {
        CPoint& point = *iter;
        DrawCircle(&dc, point, Point_Radii);
//dc.Ellipse(
//            point.x-Point_Radii,
//            point.y-Point_Radii,
//            point.x+Point_Radii,
//            point.y+Point_Radii );
    }
   
    if ( nSize == 1 )
    {
        dc.SelectObject(pOldObject);
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
        DrawCenterPoint(&dc);
    }

    dc.SelectObject(pOldObject);   
}

// End of file