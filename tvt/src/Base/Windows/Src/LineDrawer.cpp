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


//IMPLEMENT_DYNAMIC(CLineDrawer, CWnd)

CLineDrawer::CLineDrawer(CWnd* pWnd)
{
    m_pWnd = pWnd;
    m_nMaxPoint = 2;
    m_PointQueue.resize(2);
}

BOOL CLineDrawer::OnMouseMove(UINT nFlags, CPoint& point)
{
    if ( !(nFlags&MK_LBUTTON) )
    {
        return FALSE;
    }

    if ( m_bDragging && m_nDragIndex != -1 )
    {
        m_PointQueue[m_nDragIndex] = point;
        //Invalidate();
        ParentInvalidateEx();
        return TRUE;
    }

    if ( m_bDrawing )
    {
        m_PointQueue[1] = point;
        //Invalidate();
        ParentInvalidateEx();
        return TRUE;
    }

    if ( m_bDragCenter )
    {
        CenterPointMoveTo(point);
        //Invalidate();
        ParentInvalidateEx();
        return TRUE;
    }

    return FALSE;
}

BOOL CLineDrawer::OnLButtonUp(UINT nFlags, CPoint& point)
{
    if ( m_bDrawing ) 
    {
        m_bDrawing = false;
        UnLockCursor();
        return TRUE;
    }

    if ( m_bDragging )
    {
        UnLockCursor();
        m_bDragging = false;
        m_nDragIndex = -1;
        return TRUE;
    }

    if ( m_bDragCenter )
    {
        UnLockCursor();
        m_bDragCenter = false;
        return TRUE;
    }

    return FALSE;
}

BOOL CLineDrawer::OnLButtonDown(UINT nFlags, CPoint& point)
{
    if ( m_bDrawing )
    {
        // Log
        TRACE( _T("OnLButtonDown") );
        m_bDrawing = false;
        return FALSE;
    }

    if ( m_bIsOK )
    {
        CRect Rect;
        m_pWnd->GetClientRect(&Rect);
        m_pWnd->ClientToScreen(&Rect);
        if ( IsDargPoint(point) )
        { 
            LockCursor(Rect);
            m_bDragging = true;
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

    m_PointQueue[0] = m_PointQueue[1] = point;
    m_bIsOK = true;
    m_bDrawing = true;
    ParentInvalidateEx();
    return TRUE;
}

void CLineDrawer::OnPaint(CDC& dc, BOOL bSelect)
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

    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectObject(&m_Brush);

    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);
    
    if ( bSelect )
    {
        DrawSquare(&dc, BeginPoint, Point_Radii);
        DrawSquare(&dc, EndPoint, Point_Radii);
        DrawCenterPoint(&dc);
    }

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}


// ��֪���е�����ǲ���ˣ���ѧ���ʽ���£�
// | a.x , b.x |
// | a.y , b.y | 
#define XMulti(a,b) (a.x*b.y-a.y*b.x)

// �õ�����ֱ�ߵĹ�ϵ�������ߵ�һ����ʼ��ͽ����㣬Ȼ��һ�����Ե�
// ����������0��Ϊ��ʱ�룬��֮˳ʱ��
// ��ô����ֵ����0�����ߵ����
#define GetPointRLineState(bg,ed,test) (XMulti(bg,ed)+XMulti(ed,test)+XMulti(test,bg))
                                               
//
// ************* CArrowLineDrawer *****************
// {


void CArrowLineDrawer::DrawArrow(
    CDC* pdc,
    const CPoint& p,
    size_t d, 
    double o,
    bool bUp,
    DWORD dwCount )
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


CArrowLineDrawer::CArrowLineDrawer(CWnd* pWnd) 
    : m_dwDrawCommond(Line_Show_All)
    , CLineDrawer(pWnd) 
{
    m_szdwNumber[0] = m_szdwNumber[1] = 0;
}

void CArrowLineDrawer::OnPaint(CDC& dc, BOOL bSelect)
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

    CGdiObject *pOldPen = dc.SelectObject(&m_Pen);  
    CGdiObject *pOldBrush = dc.SelectObject(&m_Brush);
    
    CPoint& BeginPoint = m_PointQueue[0];
    CPoint& EndPoint = m_PointQueue[1];
    dc.MoveTo(BeginPoint);  
    dc.LineTo(EndPoint);

    int nOldMode = SetBkMode(dc, TRANSPARENT);
    dc.TextOut(BeginPoint.x, BeginPoint.y+Point_Radii, _T("A"), 1);
    dc.TextOut(EndPoint.x, EndPoint.y+Point_Radii, _T("B"), 1);
    SetBkMode(dc, nOldMode);

    if ( bSelect )
    {
        DrawSquare(&dc, BeginPoint, Point_Radii);
        DrawSquare(&dc, EndPoint, Point_Radii);  
        DrawCenterPoint(&dc); 
    }

    // ���ô�ֱ������ľ�����������������
    CPoint MedPoint((BeginPoint.x + EndPoint.x)/2, (BeginPoint.y + EndPoint.y)/2);
    double m = Distance(MedPoint, BeginPoint);
    CPoint A[2];
    A[0].x = MedPoint.x + long(ArrowLineLen/m*(BeginPoint.y-MedPoint.y));
    A[0].y = MedPoint.y - long(ArrowLineLen/m*(BeginPoint.x-MedPoint.x));
    A[1].x = MedPoint.x - long(ArrowLineLen/m*(BeginPoint.y-MedPoint.y));
    A[1].y = MedPoint.y + long(ArrowLineLen/m*(BeginPoint.x-MedPoint.x));

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
   
    // �������������������Ϊ�Ұ����ĽǶ�ת��Ϊ[0, pi]����ģ�
    // atanȡֵ��ΧΪ(-pi/2, pi/2)
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
        DrawArrow(&dc, A[0], ArrowHeadLen, o, bUp, m_szdwNumber[0]);
    }
    if ( m_dwDrawCommond & Line_Show_Left )
    {
        dc.MoveTo(MedPoint);  
        dc.LineTo(A[1]);
        DrawArrow(&dc, A[1], ArrowHeadLen, o, !bUp, m_szdwNumber[1]);
    }  

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
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
        m_pWnd->ShowWindow(SW_HIDE);
        m_pWnd->ShowWindow(SW_SHOW);
    	break;
    case Get_Line_Dir:
        *(long*)p1 = m_dwDrawCommond;
    	break;
    default:
    	break;
    }
}

//
// ************* CArrowLineDrawerEx *****************
// {
void CArrowLineDrawerEx::SendCommond(
    DrawCommond c, void* p1, void* p2)
{ 
    switch ( c )
    {
    case Line_Show_Left:
    case Line_Show_Right:
    case Line_Show_All:
        m_dwDrawCommond=c;
        m_pWnd->ShowWindow(SW_HIDE);
        m_pWnd->ShowWindow(SW_SHOW);
    	break;
    case Get_Line_Dir:
        *(long*)p1 = m_dwDrawCommond;
    	break;
    case Line_Left_Add:
        ++m_szdwNumber[0];
        break;
    case Line_Right_Add:
        ++m_szdwNumber[1];
        break;
    case Line_Reset_Add:
        m_szdwNumber[0] = 0;
        m_szdwNumber[1] = 1;
        break;
    default:
    	break;
    }
}

void CArrowLineDrawerEx::DrawArrow(
    CDC* pdc,
    const CPoint& p,
    size_t d, 
    double o,
    bool bUp,
    DWORD dwCount )
{
    CArrowLineDrawer::DrawArrow(
        pdc, p, d, o, bUp, dwCount );
    char szBuf[16] = {0};
    sprintf_s(szBuf, "%u", dwCount);
    ::TextOutA(
        pdc->m_hDC, 
        p.x,
        p.y+Point_Radii, 
        szBuf, strlen(szBuf));
}
// }
// CArrowLineDrawerEx


// End of file