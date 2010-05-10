/*H***************************************************************************
 File            : RectangleDrawer.cpp
 Subsystem       : 
 Function Name(s): CRectangleDrawer
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 14:05
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "RectangleDrawer.h"



#define PointPush(PointQueue, TmpPoint, i, j)  \
    TmpPoint = m_PointQueue[i] + m_PointQueue[j]; \
    TmpPoint.x /= 2;\
    TmpPoint.y /= 2;\
    PointQueue.push_back( TmpPoint );

#define GetMedPointQueue(PointQueue, TmpPoint) \
    PointPush(PointQueue, TmpPoint, 0, 1);\
    PointPush(PointQueue, TmpPoint, 1, 2);\
    PointPush(PointQueue, TmpPoint, 2, 3);\
    PointPush(PointQueue, TmpPoint, 3, 0);


//IMPLEMENT_DYNAMIC(CRectangleDrawer, CWnd)

CRectangleDrawer::CRectangleDrawer(CWnd* pWnd)
    : m_nDragDir(NO_Darg)
    //, m_PointQueue(4)
{
    m_pWnd = pWnd;
    m_nMaxPoint = 4;
    m_PointQueue.resize(4);
}

CRectangleDrawer::~CRectangleDrawer(void)
{
}


//BEGIN_MESSAGE_MAP(CRectangleDrawer, CWnd)
//    ON_WM_MOUSEMOVE()
//    ON_WM_LBUTTONUP()
//    ON_WM_LBUTTONDOWN()
//    ON_WM_PAINT()
//    ON_WM_ERASEBKGND()
//END_MESSAGE_MAP()

BOOL CRectangleDrawer::OnMouseMove(UINT nFlags, CPoint& point)
{
    do 
    {
        if ( m_bDragging )
        {
            //TRACE("OnMouseMove %d\n", m_nDragIndex);
            ReFreshPoint(m_nDragIndex, point);
            break;
        }

        if ( m_bDrawing )
        {
            //TRACE("OnMouseMove m_bDrawing\n");
            ReFreshPoint(2, point);
            break;
        }

        if ( m_nDragDir !=  NO_Darg )
        {
            ReFreshPoint2(m_nDragDir, point);
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

BOOL CRectangleDrawer::OnLButtonUp(UINT nFlags, CPoint& point)
{
    if ( m_bDrawing )
    {
        UnLockCursor();
        //ReFreshPoint(2, point);
        m_bDrawing = false;
        //return;
        TRACE("CRectangleDrawer::OnLButtonUp\n");
        return TRUE;
    }

    if ( m_bDragging )
    {
        UnLockCursor();
        m_bDragging = false;
        m_nDragIndex = -1;
        return TRUE;
    }

    if ( m_nDragDir != NO_Darg )
    {
        UnLockCursor();
        m_nDragDir = NO_Darg;
        return TRUE;
    }

    if ( m_bDragCenter )
    {
        UnLockCursor();
        //CenterPointMoveTo(point);
        m_bDragCenter = false;
        return TRUE;
    }
    return FALSE;
}

BOOL CRectangleDrawer::OnLButtonDown(UINT nFlags, CPoint& point)
{
    CRect Rect;
    m_pWnd->GetClientRect(&Rect);
    m_pWnd->ClientToScreen(&Rect);

    if ( m_bIsOK )
    {
        if ( IsDargPoint(point) )
        {
            LockCursor(Rect);
            m_bDragging = true;
            return TRUE;
        }
        else if ( (m_nDragDir = IsDargPoint2(point)) != NO_Darg )
        {
            m_bDragging = false;
            LockCursor(Rect);
            return TRUE;
        }
        else if ( IsDargCenterPoint(point) )
        {
            m_bDragging = false;
            LockCursor(Rect);
            m_bDragCenter = true;
            return TRUE;
        }

        m_bDragging = false;
        return FALSE;
    }

    LockCursor(Rect);
    m_bIsOK = m_bDrawing = true;
    m_PointQueue[0] = m_PointQueue[1] =
        m_PointQueue[2] = m_PointQueue[3] = point;
    Redraw();
    
    return TRUE;
}

void CRectangleDrawer::OnPaint(CDC& dc, const RECT& rect, BOOL bSelect)
{
    if (!m_bIsOK)
    {
        return;
    }

    CRect WindowRect;
    m_pWnd->GetClientRect(&WindowRect);
    double fWidth = (rect.right-rect.left)*1.0/WindowRect.Width();
    double fHeight = (rect.bottom-rect.top)*1.0/WindowRect.Height();

    CPoint p0 = ZoomPoint(m_PointQueue[0], fWidth, fHeight);
    CPoint p2 = ZoomPoint(m_PointQueue[2], fWidth, fHeight);

    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
    dc.Rectangle(p0.x, p0.y, p2.x, p2.y);

    pOldBrush = dc.SelectObject(&m_Brush);

    if ( bSelect )
    {
        for (int i=0; i<=3; ++i)
        {
            CPoint point = ZoomPoint(m_PointQueue[i], fWidth, fHeight);
            DrawSquare(&dc, point, Point_Radii);
        }

        deque<CPoint> PointQueue;
        CPoint TmpPoint;
        GetMedPointQueue(PointQueue, TmpPoint);
        for (int i=0; i<=3; ++i)
        {
            CPoint point = ZoomPoint(PointQueue[i], fWidth, fHeight);
            DrawSquare(&dc, point, Point_Radii);
        }
        DrawCenterPoint(&dc, fWidth, fHeight);
    }

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

void CRectangleDrawer::ReFreshPoint(int nIndex, const CPoint& point)
{
    //TRACE("%d\n", nIndex);
	// Rect
	//  0    1
	//  3    2 
	switch (nIndex)
	{
	case 0:
		m_PointQueue[0] = point;
		m_PointQueue[1].y = point.y;
		m_PointQueue[3].x = point.x;
		break;
	case 1: 
		m_PointQueue[0].y = point.y;
		m_PointQueue[1] = point;
		m_PointQueue[2].x = point.x;
		break;
	case 2: 
		m_PointQueue[1].x = point.x;
		m_PointQueue[2] = point;
		m_PointQueue[3].y = point.y;
		break;
	case 3: 
		m_PointQueue[0].x = point.x;
		m_PointQueue[2].y = point.y;
		m_PointQueue[3] = point;
		break;
	}
}

CRectangleDrawer::DargDir CRectangleDrawer::IsDargPoint2(
    const CPoint& point )
{
    deque<CPoint> PointQueue;
    CPoint TmpPoint;
    GetMedPointQueue(PointQueue, TmpPoint);
//#define PointPush(i, j)  \
//    TmpPoint = m_PointQueue[i] + m_PointQueue[j]; \
//    TmpPoint.x /= 2;\
//    TmpPoint.y /= 2;\
//    PointQueue.push_back( TmpPoint );
//
//    PointPush(0, 1);
//    PointPush(1, 2);
//    PointPush(2, 3);
//    PointPush(3, 0);
    
    return (DargDir)::IsDargPoint(point, PointQueue);
}

void CRectangleDrawer::ReFreshPoint2( DargDir nIndex, const CPoint& point )
{
    // Rect
    //  0    1
    //  3    2 
    //TRACE("ReFreshPoint2 %d", nIndex);
    switch (nIndex)
    {
    case Darg_Top:
        m_PointQueue[0].y = point.y;
        m_PointQueue[1].y = point.y;
        break;
    case Darg_Right: 
        m_PointQueue[1].x = point.x;
        m_PointQueue[2].x = point.x;
        break;
    case Darg_Bottom: 
        m_PointQueue[2].y = point.y;
        m_PointQueue[3].y = point.y;
        break;
    case Darg_Left: 
        m_PointQueue[0].x = point.x;
        m_PointQueue[3].x = point.x;
        break;
    }
}



// End of file




//CDoubleRectangleDrawer::CDoubleRectangleDrawer( void )
//{
//
//}
//
//CDoubleRectangleDrawer::~CDoubleRectangleDrawer( void )
//{
//
//}