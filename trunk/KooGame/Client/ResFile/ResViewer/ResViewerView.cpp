// ResViewerView.cpp : implementation of the CResViewerView class
//

#include "stdafx.h"
#include "ResViewer.h"

#include "ResViewerDoc.h"
#include "ResViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResViewerView

IMPLEMENT_DYNCREATE(CResViewerView, CView)

BEGIN_MESSAGE_MAP(CResViewerView, CView)
END_MESSAGE_MAP()

// CResViewerView construction/destruction

CResViewerView::CResViewerView()
{
	// TODO: add construction code here

}

CResViewerView::~CResViewerView()
{
}

BOOL CResViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CResViewerView drawing

void CResViewerView::OnDraw(CDC* /*pDC*/)
{
	CResViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CResViewerView diagnostics

#ifdef _DEBUG
void CResViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CResViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CResViewerDoc* CResViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResViewerDoc)));
	return (CResViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CResViewerView message handlers
