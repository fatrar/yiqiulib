
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "Darw.h"
#include "ChildView.h"

#include "LineDrawer.h"
#include "RectangleDrawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	pDrawer = new CRectangleDrawer();
}

CChildView::~CChildView()
{
	delete pDrawer;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	//ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

   /* CRect Rect;
    GetClientRect(&Rect);*/
   // pDrawer->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, Rect, this, 10246);
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	//pDrawer->OnPaint(&dc);
	// 不要为绘制消息而调用 CWnd::OnPaint()
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
   /* CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	pDrawer->OnMouseDown(GetDC(), point, rect);*/
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//pDrawer->OnMouseUp(GetDC(), point);
	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
 /*   if ( !(nFlags&MK_LBUTTON) )
    {
        return;
    }

	pDrawer->OnMouseMove(GetDC(), point);*/
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    // TODO: Add your specialized code here and/or call the base class

    CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);


    return TRUE;
}

BOOL CChildView::OnCreateAggregates()
{
    // TODO: Add your specialized code here and/or call the base class
    CRect Rect;
    GetClientRect(&Rect);
    pDrawer->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, Rect, this, 10246);
    pDrawer->ShowWindow(SW_SHOW);
    return TRUE;
}
