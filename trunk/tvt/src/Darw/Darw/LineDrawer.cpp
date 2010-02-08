/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:49
	filename: 	f:\code\Darw\Darw\LineDrawer.cpp
	file path:	f:\code\Darw\Darw
	file base:	LineDrawer
	file ext:	cpp
	author:		heliang
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "LineDrawer.h"
//#include "PointButton.h"

//CLineDrawer::CLineDrawer(void)
//{
//	m_PointQueue.push_back(new CPoint());
//	m_PointQueue.push_back(new CPoint());
//}
//
//CLineDrawer::~CLineDrawer(void)
//{
//}
//
//void CLineDrawer::OnMouseDown(
//	CDC* pdc,
//	CPoint& point,
//	CRect& LockRect )
//{
//	if ( m_bDrawing )
//	{
//		// Log
//		TRACE( CString("m_bIsStartDraw is True! function name is ") + __FUNCDNAME__ );
//	}
//
//    if ( m_bIsOK )
//    {
//        if ( IsDargPoint(point) )
//        {
//            LockCursor(LockRect);
//            m_bDragging = true;
//            return;
//        }
//    }
//
//	LockCursor(LockRect);
//    m_LockRect = LockRect;
//    m_bIsOK = m_bDrawing = true;
//	*(m_PointQueue[0]) = *(m_PointQueue[1]) = point;
//}
//
//void CLineDrawer::OnMouseMove( CDC* pdc, CPoint& point)
//{	
//    if ( m_bDragging )
//    {
//        *(m_PointQueue[m_nDragIndex]) = point;
//        pdc->GetWindow()->Invalidate();
//        return;
//    }
//
//    if ( m_bDrawing )
//    {
//        *(m_PointQueue[1]) = point;
//        pdc->GetWindow()->Invalidate();
//        return;
//    }
//}
//
//void CLineDrawer::OnMouseUp(CDC* pdc, CPoint& point)
//{
//	if ( m_bDrawing ) 
//	{
//        UnLockCursor();
//		*(m_PointQueue[1]) = point;	
//		m_bDrawing = false;
//	}
//	
//    if ( m_bDragging )
//    {
//        UnLockCursor();
//        m_bDragging = false;
//        *(m_PointQueue[m_nDragIndex]) = point;
//        m_nDragIndex = -1;
//    }
//	pdc->GetWindow()->Invalidate();
//}
//
//void CLineDrawer::OnPaint(CDC* pdc)
//{
//	if (!m_bIsOK)
//	{
//		return;
//	}
//
//	CPoint& p0 = *(m_PointQueue[0]);
//	CPoint& p1 = *(m_PointQueue[1]);
//	pdc->MoveTo(p0);
//	pdc->LineTo(p1);
//
//    CGdiObject *pOldObject= pdc->SelectStockObject(NULL_BRUSH);
//    //int nDrawmode = pdc->GetROP2();
//    //pdc->SetROP2(R2_NOTCOPYPEN);
//    // Rectangle
//    pdc->Ellipse(
//        p0.x-Point_Radii,
//        p0.y-Point_Radii,
//        p0.x+Point_Radii,
//        p0.y+Point_Radii );
//    pdc->Ellipse(
//        p1.x-Point_Radii,
//        p1.y-Point_Radii,
//        p1.x+Point_Radii,
//        p1.y+Point_Radii );
//    //pdc->SetROP2(nDrawmode);
//    //pdc->Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y);
//    pdc->SelectObject(pOldObject);
//}
//
//
//
//
//// End of file
//
//
//
//
//DrawerErr CArrowLine::OnCommand( DrawerCommand command, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
//{
//	switch (command)
//	{
//	case Line_Arrow_Change:
//		if ( wParam != Dir_All || wParam!=Dir_Left || wParam!=Dir_Right)
//		{
//			return Param_Input_Err;
//		}
//
//		m_dir = wParam;
//	default:
//		return Not_Found_Commad;
//	}
//	
//
//	return No_Error;
//}