// GameToolView.h : interface of the CGameToolView class
//


#pragma once


class CGameToolView : public CView
{
protected: // create from serialization only
	CGameToolView();
	DECLARE_DYNCREATE(CGameToolView)

// Attributes
public:
	CGameToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGameToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ShowMousePos(CPoint& point);

	void ReFreshCenter(const CPoint& point);
	void ReFreshPoint1(int nIndex, const CPoint& point);
	void ReFreshPoint2(int nIndex, const CPoint& point);

	void GetPoint1(CPoint (&szPoint)[4]);
	void GetPoint2(CPoint (&szPoint)[4]);

	BOOL IsDragCenter(const CPoint& point);
	BOOL IsDargPoint1(const CPoint& point);
	BOOL IsDargPoint2(const CPoint& point);
	
	void DrawCenter(CDC* pDC);
	void DrawPoint1(CDC* pDC);
	void DrawPoint2(CDC* pDC);

    void EarseBack();
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditSetwindow();
	afx_msg void OnEditZoomin();
	afx_msg void OnEditZoomout();
	afx_msg void OnEditClear32774();

	enum DargDir
	{
		NO_Darg = -1,
		Darg_Top,
		Darg_Right,
		Darg_Bottom,
		Darg_Left,
	};
private:
	BOOL m_bIsDrawing;
	BOOL m_bIsDrawGraph;
	BOOL m_bIsDragCenter;
	BOOL m_bIsDrag1;
	BOOL m_bIsDrag2;
	//DargDir m_nDragDir;
	int m_nDragIndex;

	double m_nO;
	CPoint m_Origin;
	int m_nWidth, m_nHeight;
	float m_nZoon;
	int m_nScale;

	CRect m_GraphRect;

	int m_x, m_y;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in GameToolView.cpp
inline CGameToolDoc* CGameToolView::GetDocument() const
   { return reinterpret_cast<CGameToolDoc*>(m_pDocument); }
#endif

