// ResViewerView.h : interface of the CResViewerView class
//


#pragma once


class CResViewerView : public CView
{
protected: // create from serialization only
	CResViewerView();
	DECLARE_DYNCREATE(CResViewerView)

// Attributes
public:
	CResViewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CResViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ResViewerView.cpp
inline CResViewerDoc* CResViewerView::GetDocument() const
   { return reinterpret_cast<CResViewerDoc*>(m_pDocument); }
#endif

