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

CLineDrawer::CLineDrawer(void)
	: m_bIsOK(false)
    , m_nDargIndex(-1)
    , m_bDarging(false)
{
}

CLineDrawer::~CLineDrawer(void)
{
}

void CLineDrawer::OnMouseDown(
	CDC* pdc,
	CPoint& point,
	CRect& LockRect )
{
	if ( m_bDrawing )
	{
		// Log
		TRACE( CString("m_bIsStartDraw is True! function name is ") + __FUNCDNAME__ );
	}

    if ( m_bIsOK )
    {
        if ( IsDargPoint(point) )
        {
            LockCursor(LockRect);
            m_bDarging = true;
            return;
        }
    }

	LockCursor(LockRect);
    m_LockRect = LockRect;
    m_bIsOK = m_bDrawing = true;
	m_Point[0] = m_Point[1] = point;
}

void CLineDrawer::OnMouseMove( CDC* pdc, CPoint& point)
{	
    if ( m_bDarging )
    {
        m_Point[m_nDargIndex] = point;
        pdc->GetWindow()->Invalidate();
        return;
    }

    if ( m_bDrawing )
    {
        m_Point[1] = point;
        pdc->GetWindow()->Invalidate();
        return;
    }
}

void CLineDrawer::OnMouseUp(CDC* pdc, CPoint& point)
{
	if ( m_bDrawing ) 
	{
        UnLockCursor();
		m_Point[1] = point;	
		m_bDrawing = false;
	}
	
    if ( m_bDarging )
    {
        UnLockCursor();
        m_bDarging = false;
        m_Point[m_nDargIndex] = point;
        m_nDargIndex = -1;
    }
	pdc->GetWindow()->Invalidate();
}

void CLineDrawer::OnPaint(CDC* pdc)
{
	if (!m_bIsOK)
	{
		return;
	}

	pdc->MoveTo(m_Point[0]);
	pdc->LineTo(m_Point[1]);

    CGdiObject *pOldObject= pdc->SelectStockObject(NULL_BRUSH);
    //int nDrawmode = pdc->GetROP2();
    //pdc->SetROP2(R2_NOTCOPYPEN);
    // Rectangle
    pdc->Ellipse(
        m_Point[0].x-Point_Radii,
        m_Point[0].y-Point_Radii,
        m_Point[0].x+Point_Radii,
        m_Point[0].y+Point_Radii );
    pdc->Ellipse(
        m_Point[1].x-Point_Radii,
        m_Point[1].y-Point_Radii,
        m_Point[1].x+Point_Radii,
        m_Point[1].y+Point_Radii );
    //pdc->SetROP2(nDrawmode);
    //pdc->Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y);
    pdc->SelectObject(pOldObject);
}

bool CLineDrawer::IsDargPoint(CPoint& point)
{
    int s1 = (int)Distance(point, m_Point[0]);
    int s2 = (int)Distance(point, m_Point[1]);
    
    if ( s1 > Point_Radii )
    {
        if ( s2 > Point_Radii )
        {
            return false;
        }

        m_nDargIndex = 1;
        return true;
    }
   
    if ( s2 > Point_Radii )
    {
        m_nDargIndex = 0;
        return true;
    }

    m_nDargIndex = s1<s1?0:1;
    return true;
}