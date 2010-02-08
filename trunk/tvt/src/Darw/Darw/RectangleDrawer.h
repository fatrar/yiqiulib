/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:43
	filename: 	f:\code\Darw\Darw\RectangleDrawer.h
	file path:	f:\code\Darw\Darw
	file base:	RectangleDrawer
	file ext:	h
	author:		heliang
	
	purpose:	
*********************************************************************/
#pragma once
#include "DarwCommon.h"

class CRectangleDrawer :
	public CDrawer
{
    DECLARE_DYNAMIC(CRectangleDrawer)
public:
	CRectangleDrawer(void);
	~CRectangleDrawer(void);
protected:
     DECLARE_MESSAGE_MAP()

public:
    size_t GetUserInput(CPoint (&szPoint)[Max_Point]);

public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();

protected:
	inline void ReFreshPoint(int nIndex, CPoint& point);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};







// End of file



