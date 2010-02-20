/*H***************************************************************************
 File            : LineDrawer.cpp
 Subsystem       : 
 Function Name(s): CLineDrawer
 Author          : YiQiu
 Date            : 2010-2-9  
 Time            : 13:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "LineDrawer.h"


IMPLEMENT_DYNAMIC(CLineDrawer, CWnd)

CLineDrawer::CLineDrawer(void)
{
    m_PointQueue.resize(2);
}

CLineDrawer::~CLineDrawer(void)
{
}

BEGIN_MESSAGE_MAP(CLineDrawer, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()


void CLineDrawer::OnMouseMove(UINT nFlags, CPoint point)
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
        m_PointQueue[1] = point;
        ParentInvalidate();
        return;
    }
}

void CLineDrawer::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_bDrawing ) 
    {
        m_bDrawing = false;
        UnLockCursor();
        return;
    }

    if ( m_bDragging )
    {
        UnLockCursor();
        m_bDragging = false;
        m_PointQueue[m_nDragIndex] = point;
        m_nDragIndex = -1;
        return;
    }
}

void CLineDrawer::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_bDrawing )
    {
        // Log
        TRACE( _T("OnLButtonDown") );
        m_bDrawing = false;
        return;
    }

    if ( m_bIsOK )
    {
        if ( IsDargPoint(point) )
        {
            CRect Rect;
            GetClientRect(&Rect);
            ClientToScreen(&Rect);
            LockCursor(Rect);
            m_bDragging = true;
            return;
        }
       
        m_bDragging = false;
    }

    m_PointQueue[0] = m_PointQueue[1] = point;
    m_bIsOK = true;
    m_bDrawing = true;
    ParentInvalidate();
}

void CLineDrawer::OnPaint()
{
    if (!m_bIsOK)
    {
        return;
    }

    size_t nSize = m_PointQueue.size();
    if ( nSize != 2 )
    {
        return;
    }

    CPaintDC dc(this);
    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);

    CGdiObject *pOldObject= dc.SelectStockObject(NULL_BRUSH);
    dc.Ellipse(
        BeginPoint.x-Point_Radii,
        BeginPoint.y-Point_Radii,
        BeginPoint.x+Point_Radii,
        BeginPoint.y+Point_Radii );
    dc.Ellipse(
        EndPoint.x-Point_Radii,
        EndPoint.y-Point_Radii,
        EndPoint.x+Point_Radii,
        EndPoint.y+Point_Radii );
    DrawCenterPoint(&dc);
     dc.SelectObject(pOldObject);
}

// 不知道叫点积还是差积了，数学表达式如下：
// | a.x , b.x |
// | a.y , b.y | 
#define XMulti(a,b) (a.x*b.y-a.y*b.x)

// 得到线与直线的关系，输入线的一个开始点和结束点，然后一个测试点
// 如果结果大于0则为逆时针，反之顺时针
// 那么返回值大于0，则线的左边
#define GetPointRLineState(bg,ed,test) (XMulti(bg,ed)+XMulti(ed,test)+XMulti(test,bg))


//
// ************* CArrowLineDrawer *****************
//
IMPLEMENT_DYNAMIC(CArrowLineDrawer, CWnd)

BEGIN_MESSAGE_MAP(CArrowLineDrawer, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()


void CArrowLineDrawer::OnPaint()
{
    if (!m_bIsOK)
    {
        return;
    }

    size_t nSize = m_PointQueue.size();
    if ( nSize != 2 )
    {
        return;
    }

    CPaintDC dc(this);
    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);

    dc.Ellipse(
        BeginPoint.x-Point_Radii,
        BeginPoint.y-Point_Radii,
        BeginPoint.x+Point_Radii,
        BeginPoint.y+Point_Radii );
    dc.Ellipse(
        EndPoint.x-Point_Radii,
        EndPoint.y-Point_Radii,
        EndPoint.x+Point_Radii,
        EndPoint.y+Point_Radii );

    // 利用垂直和两点的距离算出两个点的坐标
    CPoint MedPoint((BeginPoint.x + EndPoint.x)/2, (BeginPoint.y + EndPoint.y)/2);
    double m = Distance(MedPoint, BeginPoint);
    CPoint A[2];
    A[0].x = MedPoint.x + ArrowLineLen/m*(BeginPoint.y-MedPoint.y);
    A[0].y = MedPoint.y - ArrowLineLen/m*(BeginPoint.x-MedPoint.x);
    A[1].x = MedPoint.x - ArrowLineLen/m*(BeginPoint.y-MedPoint.y);
    A[1].y = MedPoint.y + ArrowLineLen/m*(BeginPoint.x-MedPoint.x);
    dc.MoveTo(A[0]);  
    dc.LineTo(A[1]);
   // dc.p
}



// End of file