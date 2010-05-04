/*H***************************************************************************
 File            : Drawer.cpp
 Subsystem       : 
 Function Name(s): CDrawer
 Author          : YiQiu
 Date            : 2010-3-2  
 Time            : 15:36
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "Drawer.h"



CDrawer::CDrawer()
    : m_bDrawing(false)
    , m_nDragIndex(-1)
    , m_bDragging(false)
    , m_bIsOK(false)
    , m_dwLineColor(Point_Default_Color)
    , m_nLineWidth(Line_Default_Width)
    , m_bDragCenter(false)   
    , m_bEnbale(TRUE)
    , m_Pen(PS_SOLID, Line_Default_Width, Point_Default_Color)
    , m_Brush(Point_Default_Color) {}

size_t CDrawer::GetUserInput(CPoint* pPointBuf, size_t nCount)
{
    if ( !m_bIsOK || nCount < m_PointQueue.size() )
    {
        return 0;
    }

    size_t i = 0;
    for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
          iter != m_PointQueue.end();
          ++iter, ++i )
    {
        pPointBuf[i] = *iter;
    }
    return i;
}

size_t CDrawer::PointCount() 
{
    if ( m_bIsOK )
    {
        return m_PointQueue.size();
    }
    return 0;
}

void CDrawer::SetLineColour(DWORD dwColor)
{
    m_dwLineColor = dwColor;
    m_Pen.DeleteObject();
    m_Pen.CreatePen(PS_SOLID, m_nLineWidth, m_dwLineColor);
    m_Brush.DeleteObject();
    m_Brush.CreateSolidBrush(dwColor);
    //m_pWnd->Invalidate();
    m_pWnd->RedrawWindow();
}

void CDrawer::SetLineWidth(int nWidth)
{
    m_nLineWidth = nWidth;
    m_Pen.DeleteObject();
    m_Pen.CreatePen(PS_SOLID, m_nLineWidth, m_dwLineColor);
    //m_pWnd->Invalidate();
    m_pWnd->RedrawWindow();
}

void CDrawer::Clear()
{ 
    //m_PointQueue.clear(); 
    m_bIsOK=false;
    m_pWnd->RedrawWindow();
};

void CDrawer::SetDefault(const CPoint* pPoint, size_t nCount)
{
    ASSERT(pPoint);

    m_PointQueue.clear();
    m_bIsOK=true;

    for (size_t i = 0; i<nCount; ++i)
    {
        m_PointQueue.push_back(pPoint[i]);
    }
}

bool CDrawer::IsDargPoint(CPoint& point)
{
    m_nDragIndex = ::IsDargPoint(point, m_PointQueue);
    return  m_nDragIndex != -1 ;
}

CPoint CDrawer::CenterPoint()
{
    CPoint p(0, 0);
    for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
        iter != m_PointQueue.end();
        ++iter )
    {
        p += *iter;
    }

    size_t nSize = m_PointQueue.size();
    p.x /= nSize;
    p.y /= nSize;
    return p;
}

bool CDrawer::IsDargCenterPoint(CPoint& point)
{
    if ( !m_bIsOK )
    {
        return false;
    }

    CPoint p = CenterPoint();
    return Distance(point, p) <= Point_Radii;
}

void CDrawer::DrawCenterPoint(CDC* pdc)
{
    if ( !m_bIsOK )
    {
        return;
    }

    CPoint& p = CenterPoint();
    DrawCircle(pdc, p, Point_Radii);
}

void CDrawer::CenterPointMoveTo(const CPoint& CenterpointNow)
{
    CPoint CenterpointOld = CenterPoint();
    CPoint Offset = CenterpointNow - CenterpointOld;
    for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
          iter != m_PointQueue.end();
          ++iter )
    {
        *iter += Offset;
    }
}

void CDrawer::Redraw()
{
    //if ( )
    //{
    //}
    //ParentInvalidateEx();
}






// End of file