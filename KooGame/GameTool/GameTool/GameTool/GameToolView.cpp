// GameToolView.cpp : implementation of the CGameToolView class
//

#include "stdafx.h"
#include "GameTool.h"

#include "GameToolDoc.h"
#include "GameToolView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum
{
	Default_Width = 240,
	Default_Height = 320,

	Default_X_Offset = 10,
	Default_Y_Offset = 10,

	Default_Scale = 10,
	Default_Scale_Len = 30,

	Default_Center_Point_X = 5,
	Default_Center_Point_Y = 5,
};

#define Default_Origin_X(z) (Default_X_Offset)
#define	Default_Origin_y(z) (Default_Y_Offset + Default_Height*(z))


#define Pow2(x) ((x)*(x)) 
inline double Distance(const CPoint& p1, const CPoint& p2)
{
	return sqrt( Pow2(double(p1.x-p2.x)) + Pow2(double(p1.y-p2.y)) );
}

template<size_t nSize>
static int IsDargPoint(
	const CPoint& point,
	const CPoint (&szTestPoint)[nSize])
{
#define SimpleDistance(x)  Distance((point), (x))

	int nIndex = 0;
	double nMin = SimpleDistance( szTestPoint[nIndex] );
	for (size_t i=1; i<nSize; ++i)
	{
		double nTmp = SimpleDistance( szTestPoint[i] );
		if ( nTmp < nMin )
		{
			nMin = nTmp;
			nIndex = i;
		}
	}

	if ( nMin > (double)Default_Center_Point_X )
	{
		return -1;
	}

	return nIndex;
}

// CGameToolView

IMPLEMENT_DYNCREATE(CGameToolView, CView)

BEGIN_MESSAGE_MAP(CGameToolView, CView)
	ON_COMMAND(ID_EDIT_SETWINDOW, &CGameToolView::OnEditSetwindow)
	ON_COMMAND(ID_EDIT_ZOOMIN, &CGameToolView::OnEditZoomin)
	ON_COMMAND(ID_EDIT_ZOOMOUT, &CGameToolView::OnEditZoomout)
	ON_COMMAND(ID_EDIT_CLEAR32774, &CGameToolView::OnEditClear32774)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CGameToolView construction/destruction

CGameToolView::CGameToolView()	
	: m_bIsDrawGraph(FALSE)
	, m_nO(0)
	, m_nWidth(Default_Width)
	, m_nHeight(Default_Height)
	, m_nZoon(2)
	, m_bIsDrawing(FALSE)
	, m_bIsDrag1(FALSE)
	, m_bIsDrag2(FALSE)
{
	// TODO: add construction code here
	m_Origin = CPoint(Default_Origin_X(m_nZoon),Default_Origin_y(m_nZoon));
}

CGameToolView::~CGameToolView()
{
}

BOOL CGameToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGameToolView drawing

void CGameToolView::OnDraw(CDC* pDC)
{
	CGameToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//CRect rect;
	//GetClientRect(&rect);
	//pDC->SetMapMode(MM_ANISOTROPIC);
	//pDC->SetViewportOrg(rect.left+Default_Origin_X,rect.bottom-Default_Origin_y);
	//pDC->SetViewportExt(rect.right,-rect.bottom);
	//pDC->SetWindowExt(1000,1000);
	//pDC->MoveTo(10,10);
	//pDC->LineTo(250,10);
	//pDC->MoveTo(10,10);
	//pDC->LineTo(10,950);

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)) ;
	CBrush *pOldBrush = pDC->SelectObject(pBrush);
	do 
	{
		pDC->Rectangle(
			Default_X_Offset,
			Default_Y_Offset, 
			Default_Width*m_nZoon+Default_X_Offset,
			Default_Height*m_nZoon+Default_Y_Offset);
		if ( !m_bIsDrawGraph )
		{
			break;
		}

		pDC->Ellipse(&m_GraphRect);
		
		DrawCenter(pDC);
		DrawPoint1(pDC);
		DrawPoint2(pDC);
	}
	while (0);
	
	pDC->SelectObject(pOldBrush);
}


// CGameToolView diagnostics

#ifdef _DEBUG
void CGameToolView::AssertValid() const
{
	CView::AssertValid();
}

void CGameToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameToolDoc* CGameToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameToolDoc)));
	return (CGameToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameToolView message handlers

void CGameToolView::OnEditSetwindow()
{
	// TODO: Add your command handler code here
}

void CGameToolView::OnEditZoomin()
{
	// TODO: Add your command handler code here
}

void CGameToolView::OnEditZoomout()
{
	// TODO: Add your command handler code here
}

void CGameToolView::OnEditClear32774()
{
	// TODO: Add your command handler code here
}

void CGameToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_bIsDrawGraph )
	{
		if ( IsDragCenter(point) )
		{
			m_bIsDragCenter = TRUE;
		}
		else if ( IsDargPoint1(point) )
		{
			m_bIsDrag1 = TRUE;
		}
		else if ( IsDargPoint2(point) )
		{
			m_bIsDrag2 = TRUE;
		}
	}
	else
	{
		m_bIsDrawGraph = m_bIsDrawing = TRUE;
		m_GraphRect.SetRect(point, point);
	}
	
	CView::OnLButtonDown(nFlags, point);
}

void CGameToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	do 
	{
		if ( !m_bIsDrawing )
		{
			if ( m_bIsDragCenter )
			{
				ReFreshCenter(point);
				m_bIsDragCenter = FALSE;
			}
			else if ( m_bIsDrag1 )
			{
				ReFreshPoint1(m_nDragIndex, point);
				m_bIsDrag1 = FALSE;
			}
			else if ( m_bIsDrag2 )
			{
				ReFreshPoint2(m_nDragIndex, point);
				m_bIsDrag2 = FALSE;
			}
			else
			{
				break;
			}
		}
		else
		{
			m_GraphRect.bottom = point.y;
			m_GraphRect.right = point.x;
			m_bIsDrawing = FALSE;
		}

		Invalidate();
	}
	while (0);
	CView::OnLButtonUp(nFlags, point);
}

void CGameToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	ShowMousePos(point);

	do 
	{
		if ( !(nFlags&MK_LBUTTON) )
		{
			break;
		}

		if ( !m_bIsDrawing )
		{
			if ( m_bIsDragCenter )
			{
				ReFreshCenter(point);
			}
			else if ( m_bIsDrag1 )
			{
				ReFreshPoint1(m_nDragIndex, point);
			}
			else if ( m_bIsDrag2 )
			{
				ReFreshPoint2(m_nDragIndex, point);
			}
			else
			{
				break;
			}
		}
		else
		{
			m_GraphRect.bottom = point.y;
			m_GraphRect.right = point.x;
		}

		InvalidateRect(&m_GraphRect);
	}
	while (0);
	
	CView::OnMouseMove(nFlags, point);
}

void CGameToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_x = cx;
	m_y = cy;
}


void CGameToolView::ShowMousePos( CPoint& point )
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd(); 
	CString strTmp; 
	strTmp.Format( _T("×ø±êX:%d "), int((point.x-m_Origin.x)/m_nZoon));
	pFrame->m_wndStatusBar.SetPaneText(0, strTmp); 
	strTmp.Format(  _T("×ø±êy:%d "), int((m_Origin.y-point.y)/m_nZoon)); 
	pFrame->m_wndStatusBar.SetPaneText(1, strTmp); 
}

int CGameToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rect;
	GetClientRect(&rect);
	return 0;
}

void CGameToolView::ReFreshCenter( const CPoint& point )
{
    EarseBack();
    CPoint CenterPoint = m_GraphRect.CenterPoint();
    CPoint p = point - CenterPoint;
    m_GraphRect.OffsetRect(p);
}

void CGameToolView::ReFreshPoint1(int nIndex, const CPoint& point)
{
    EarseBack();
	// Rect
	//  0    1
	//  3    2 
	switch (nIndex)
	{
	case 0:
		m_GraphRect.TopLeft() = point;
		break;
	case 1:
		m_GraphRect.TopLeft().y = point.y;
		m_GraphRect.BottomRight().x = point.x;
		break;
	case 2: 
		m_GraphRect.BottomRight() = point;
		break;
	case 3: 
		m_GraphRect.TopLeft().x = point.x;
		m_GraphRect.BottomRight().y = point.y;
		break;
	}
}

void CGameToolView::ReFreshPoint2(int nIndex, const CPoint& point)
{
    EarseBack();
	// Rect
	//  0    1
	//  3    2 
	switch (nIndex)
	{
	case Darg_Top:
		m_GraphRect.TopLeft().y = point.y;
		break;
	case Darg_Right:
		m_GraphRect.BottomRight().x = point.x;
		break;
	case Darg_Bottom: 
		m_GraphRect.BottomRight().y = point.y;
		break;
	case Darg_Left: 
		m_GraphRect.TopLeft().x = point.x;
		break;
	}
}

BOOL CGameToolView::IsDragCenter( const CPoint& point )
{
	CPoint CenterPoint = m_GraphRect.CenterPoint();
	CRect CenterRect(
		CenterPoint.x-Default_Center_Point_X, 
		CenterPoint.y-Default_Center_Point_Y,
		CenterPoint.x+Default_Center_Point_X, 
		CenterPoint.y+Default_Center_Point_Y);
	return CenterRect.PtInRect(point);
}

BOOL CGameToolView::IsDargPoint1( const CPoint& point )
{
	CPoint szTestPoint[4];
	GetPoint1( szTestPoint );
	m_nDragIndex = ::IsDargPoint(point, szTestPoint);
	if ( m_nDragIndex != -1 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGameToolView::IsDargPoint2( const CPoint& point )
{
	CPoint szTestPoint[4];
	GetPoint2( szTestPoint );
	m_nDragIndex = ::IsDargPoint(point, szTestPoint);
	if ( m_nDragIndex != -1 )
	{
		return TRUE;
	}

	return FALSE;
}


inline void DrawPoint(CDC* pDC, const CPoint& point)
{
	CRect CenterRect(
		point.x-Default_Center_Point_X, 
		point.y-Default_Center_Point_Y,
		point.x+Default_Center_Point_X, 
		point.y+Default_Center_Point_Y);
	pDC->Rectangle(&CenterRect);
}

void CGameToolView::DrawCenter( CDC* pDC )
{
	CPoint CenterPoint = m_GraphRect.CenterPoint();
	DrawPoint(pDC, CenterPoint);
}

void CGameToolView::DrawPoint1( CDC* pDC )
{
	CPoint szTestPoint[4];
	GetPoint1( szTestPoint );
	for (int i = 0; i<4; ++i)
	{
		TRACE("Index%i x=%d, y=%d\n", i, szTestPoint[i].x, szTestPoint[i].y);
		DrawPoint(pDC, szTestPoint[i]);
	}
}

void CGameToolView::DrawPoint2( CDC* pDC )
{
	CPoint szTestPoint[4];
	GetPoint2( szTestPoint );
	for (int i = 0; i<4; ++i)
	{
		DrawPoint(pDC, szTestPoint[i]);
	}
}

void CGameToolView::GetPoint1( CPoint (&szPoint)[4] )
{
	szPoint[0] = m_GraphRect.TopLeft();
	szPoint[1].SetPoint(m_GraphRect.right, m_GraphRect.top);
	szPoint[2] = m_GraphRect.BottomRight();
	szPoint[3].SetPoint(m_GraphRect.left, m_GraphRect.bottom);
}

void CGameToolView::GetPoint2( CPoint (&szPoint)[4] )
{
	int nX = (m_GraphRect.left + m_GraphRect.right)/2;
	int nY = (m_GraphRect.top + m_GraphRect.bottom)/2;
	szPoint[0] = CPoint(nX, m_GraphRect.top);
	szPoint[1] = CPoint(m_GraphRect.right, nY);
	szPoint[2] = CPoint(nX, m_GraphRect.bottom);
	szPoint[3] = CPoint(m_GraphRect.left, nY);
}

void CGameToolView::EarseBack()
{
    m_bIsDrawGraph = FALSE;
    CRect rect = m_GraphRect;
    rect.left -= Default_X_Offset;
    rect.right+= Default_X_Offset;
    rect.top -= Default_Y_Offset;
    rect.bottom += Default_Y_Offset;
    InvalidateRect(&rect);
    m_bIsDrawGraph = TRUE;
}

