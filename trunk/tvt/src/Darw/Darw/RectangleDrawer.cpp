/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:45
	filename: 	f:\code\Darw\Darw\RectangleDrawer.cpp
	file path:	f:\code\Darw\Darw
	file base:	RectangleDrawer
	file ext:	cpp
	author:		heliang
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "RectangleDrawer.h"


IMPLEMENT_DYNAMIC(CRectangleDrawer, CWnd)

CRectangleDrawer::CRectangleDrawer(void)
{
	m_PointQueue.push_back(new CPoint());
	m_PointQueue.push_back(new CPoint());
	m_PointQueue.push_back(new CPoint());
	m_PointQueue.push_back(new CPoint());
}

CRectangleDrawer::~CRectangleDrawer(void)
{
}


BEGIN_MESSAGE_MAP(CRectangleDrawer, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CRectangleDrawer::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( !(nFlags&MK_LBUTTON) )
    {
        return;
    }

    if ( m_bDragging )
    {
        TRACE("OnMouseMove %d\n", m_nDragIndex);
        ReFreshPoint(m_nDragIndex, point);
        Invalidate();
        return;
    }

    if ( m_bDrawing )
    {
        ReFreshPoint(2, point);
        Invalidate();
        return;
    }
}

void CRectangleDrawer::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_bDrawing ) 
    {
        UnLockCursor();
        ReFreshPoint(2, point);
        m_bDrawing = false;
    }

    if ( m_bDragging )
    {
        UnLockCursor();
        m_bDragging = false;
        ReFreshPoint(m_nDragIndex, point);
        m_nDragIndex = -1;
    }

    Invalidate();
}

void CRectangleDrawer::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_bDrawing )
    {
        // Log
        TRACE( CString("m_bIsStartDraw is True! function name is ") + __FUNCDNAME__ );
    }

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
    }

    LockCursor(Rect);
    m_bIsOK = m_bDrawing = true;
    *(m_PointQueue[0]) = *(m_PointQueue[1]) =
        *(m_PointQueue[2]) = *(m_PointQueue[3]) = point;
}

void CRectangleDrawer::OnPaint()
{
    if (!m_bIsOK)
    {
        return;
    }

    CPaintDC dc(this);
    CPoint& p0 = *(m_PointQueue[0]);
    //CPoint& p1 = *(m_PointQueue[1]);
    CPoint& p2 = *(m_PointQueue[2]);
    //CPoint& p3 = *(m_PointQueue[3]);


    CGdiObject *pOldObject= dc.SelectStockObject(NULL_BRUSH);
    dc.Rectangle(p0.x, p0.y, p2.x, p2.y);
    for (int i=0; i<=3; ++i)
    {
        CPoint* point = m_PointQueue[i];
        dc.Ellipse(
            point->x-Point_Radii,
            point->y-Point_Radii,
            point->x+Point_Radii,
            point->y+Point_Radii );
    }
    dc.SelectObject(pOldObject);
}


//void CRectangleDrawer::OnMouseDown(
//    CDC* pdc,
//    CPoint& point,
//    CRect& LockRect)
//{
//	if ( m_bDrawing )
//	{
//		// Log
//		TRACE( CString("m_bIsStartDraw is True! function name is ") + __FUNCDNAME__ );
//	}
//
//	if ( m_bIsOK )
//	{
//		if ( IsDargPoint(point) )
//		{
//			LockCursor(LockRect);
//			m_bDragging = true;
//			return;
//		}
//	}
//
//	LockCursor(LockRect);
//	m_LockRect = LockRect;
//	m_bIsOK = m_bDrawing = true;
//	*(m_PointQueue[0]) = *(m_PointQueue[1]) =
//		*(m_PointQueue[2]) = *(m_PointQueue[3]) = point;
//}
//
//void CRectangleDrawer::OnMouseMove(CDC* pdc, CPoint& point)
//{
//	if ( m_bDragging )
//	{
//        TRACE("OnMouseMove %d\n", m_nDragIndex);
//		ReFreshPoint(m_nDragIndex, point);
//		pdc->GetWindow()->Invalidate();
//		return;
//	}
//
//	if ( m_bDrawing )
//	{
//		ReFreshPoint(2, point);
//		pdc->GetWindow()->Invalidate();
//		return;
//	}
//}
//
//void CRectangleDrawer::OnMouseUp(CDC* pdc, CPoint& point)
//{
//	if ( m_bDrawing ) 
//	{
//		UnLockCursor();
//		ReFreshPoint(2, point);
//		m_bDrawing = false;
//	}
//
//	if ( m_bDragging )
//	{
//		UnLockCursor();
//		m_bDragging = false;
//		ReFreshPoint(m_nDragIndex, point);
//		m_nDragIndex = -1;
//	}
//	pdc->GetWindow()->Invalidate();
//}
//
//void CRectangleDrawer::OnPaint(CDC* pdc)
//{
//	if (!m_bIsOK)
//	{
//		return;
//	}
//
//	CPoint& p0 = *(m_PointQueue[0]);
//	//CPoint& p1 = *(m_PointQueue[1]);
//	CPoint& p2 = *(m_PointQueue[2]);
//	//CPoint& p3 = *(m_PointQueue[3]);
//
//
//	CGdiObject *pOldObject= pdc->SelectStockObject(NULL_BRUSH);
//	pdc->Rectangle(p0.x, p0.y, p2.x, p2.y);
//	for (int i=0; i<=3; ++i)
//	{
//		CPoint* point = m_PointQueue[i];
//		pdc->Ellipse(
//			point->x-Point_Radii,
//			point->y-Point_Radii,
//			point->x+Point_Radii,
//			point->y+Point_Radii );
//	}
//	pdc->SelectObject(pOldObject);
//}

void CRectangleDrawer::ReFreshPoint(int nIndex, CPoint& point)
{
    //TRACE("%d\n", nIndex);
	// Rect
	//  0    1
	//  3    2 
	switch (nIndex)
	{
	case 0:
		*(m_PointQueue[0]) = point;
		m_PointQueue[1]->y = point.y;
		m_PointQueue[3]->x = point.x;
		break;
	case 1: 
		m_PointQueue[0]->y = point.y;
		*(m_PointQueue[1]) = point;
		m_PointQueue[2]->x = point.x;
		break;
	case 2: 
		m_PointQueue[1]->x = point.x;
		*(m_PointQueue[2]) = point;
		m_PointQueue[3]->y = point.y;
		break;
	case 3: 
		m_PointQueue[0]->x = point.x;
		m_PointQueue[2]->y = point.y;
		*(m_PointQueue[3]) = point;
		break;
	}
}

size_t CRectangleDrawer::GetUserInput( CPoint (&szPoint)[Max_Point] )
{
    return 0;
}





// End of file




BOOL CRectangleDrawer::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return CDrawer::OnEraseBkgnd(pDC);
}