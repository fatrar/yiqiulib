// ZonePaint.cpp : implementation file
//

#include "stdafx.h"
#include "Darw.h"
#include "ZonePaint.h"


// CZonePaint

IMPLEMENT_DYNAMIC(CZonePaint, CWnd)

CZonePaint::CZonePaint()
{

}

CZonePaint::~CZonePaint()
{
}


BEGIN_MESSAGE_MAP(CZonePaint, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()



// CZonePaint message handlers



void CZonePaint::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnMouseMove(nFlags, point);
}

void CZonePaint::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnLButtonUp(nFlags, point);
}

void CZonePaint::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnLButtonDown(nFlags, point);
}

void CZonePaint::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages
}
