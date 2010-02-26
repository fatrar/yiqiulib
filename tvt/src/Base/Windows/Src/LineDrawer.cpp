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
    m_nMaxPoint = 2;
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

    if ( m_bDragCenter )
    {
        CenterPointMoveTo(point);
        ParentInvalidate();
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
        m_nDragIndex = -1;
        return;
    }

    if ( m_bDragCenter )
    {
        UnLockCursor();
        m_bDragCenter = false;
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
        CRect Rect;
        GetClientRect(&Rect);
        ClientToScreen(&Rect);
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

    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectObject(&m_Brush);

    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);
    
    DrawCircle(&dc, BeginPoint, Point_Radii);
    DrawCircle(&dc, EndPoint, Point_Radii);
    DrawCenterPoint(&dc);

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
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
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void DrawArrow(CDC* pdc, const CPoint& p, size_t d, double o, bool bUp = true)
{
    CPoint p0, p1, p2;
    if ( bUp )
    {
        p0.x = p.x + long(d*cos(o));
        p0.y = p.y + long(d*sin(o));
    }
    else
    {
        p0.x = p.x - long(d*cos(o));
        p0.y = p.y - long(d*sin(o));
    }
 
    p1.x = p.x - long(d*sin(o));
    p1.y = p.y + long(d*cos(o));
    p2.x = p.x + long(d*sin(o));
    p2.y = p.y - long(d*cos(o));
    pdc->MoveTo(p);
    pdc->LineTo(p0);
    pdc->LineTo(p1);
    pdc->MoveTo(p0);
    pdc->LineTo(p2);
}

CArrowLineDrawer::CArrowLineDrawer() : m_dwDrawCommond(Line_Show_All) {}

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

    //ShowWindow(SW_HIDE);
    CPaintDC dc(this);

    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectObject(&m_Brush);

    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);
    dc.TextOut(BeginPoint.x, BeginPoint.y+Point_Radii, _T("A"), 1);
    dc.TextOut(EndPoint.x, EndPoint.y+Point_Radii, _T("B"), 1);

    DrawCircle(&dc, BeginPoint, Point_Radii);
    DrawCircle(&dc, EndPoint, Point_Radii);  
    DrawCenterPoint(&dc); 

    // 利用垂直和两点的距离算出两个点的坐标
    CPoint MedPoint((BeginPoint.x + EndPoint.x)/2, (BeginPoint.y + EndPoint.y)/2);
    double m = Distance(MedPoint, BeginPoint);
    CPoint A[2];
    A[0].x = MedPoint.x + long(ArrowLineLen/m*(BeginPoint.y-MedPoint.y));
    A[0].y = MedPoint.y - long(ArrowLineLen/m*(BeginPoint.x-MedPoint.x));
    A[1].x = MedPoint.x - long(ArrowLineLen/m*(BeginPoint.y-MedPoint.y));
    A[1].y = MedPoint.y + long(ArrowLineLen/m*(BeginPoint.x-MedPoint.x));
   
   // dc.p

    double o;
    long nXoffset = A[0].x-A[1].x;
    long nYoffset = A[0].y-A[1].y;
    if (nXoffset == 0)
    {
        o = nYoffset > 0 ? M_PI_2 : (-M_PI_2);
    }
    else
    {
        double tanValue = 1.0*(nYoffset)/nXoffset;
        o = atan(tanValue);
    }
   
    // 修正两种特殊情况，因为我把它的角度转换为[0, pi]区间的，
    // atan取值范围为(-pi/2, pi/2)
    if ( o < 0 )
    {
        o += M_PI;
    }
    else if ( o == 0 && nXoffset > 0 )
    {
        o = M_PI;
    }
 
    
    bool bUp = (A[0].y > A[1].y);
    //bool bUp = (A[0].x > A[1].x) ^ (A[0].y < A[1].y);
    //bool bUp = GetPointRLineState(BeginPoint, EndPoint, A[1]) > 0; 
    //TRACE("%d\n", bUp);
    if ( m_dwDrawCommond & Line_Show_Right )
    {
        dc.MoveTo(MedPoint);  
        dc.LineTo(A[0]);
        DrawArrow(&dc, A[0], ArrowHeadLen, o, bUp);
    }
    if ( m_dwDrawCommond & Line_Show_Left )
    {
        dc.MoveTo(MedPoint);  
        dc.LineTo(A[1]);
        DrawArrow(&dc, A[1], ArrowHeadLen, o, !bUp);
    }  

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
    //ShowWindow(SW_SHOW);
    //TRACE("Paint \n");
}

void CArrowLineDrawer::SendCommond(
    DrawCommond c, void* p1, void* p2)
{ 
    switch ( c )
    {
    case Line_Show_Left:
    case Line_Show_Right:
    case Line_Show_All:
        m_dwDrawCommond=c;
        ShowWindow(SW_HIDE);
        ShowWindow(SW_SHOW);
    	break;
    case Get_Line_Dir:
        *(long*)p1 = m_dwDrawCommond;
    	break;
    default:
    	break;
    }
}

BOOL CArrowLineDrawer::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    /*TRACE("ArrowLineDrawer::OnEraseBkgnd\n");
    CRect rect;
    BOOL bRc = GetUpdateRect(&rect, TRUE);*/
    //return TRUE;   
    return CWnd::OnEraseBkgnd(pDC);
}




// End of file