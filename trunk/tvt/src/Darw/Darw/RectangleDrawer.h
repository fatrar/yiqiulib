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
public:
	CRectangleDrawer(void);
	~CRectangleDrawer(void);
public:
	virtual void OnMouseDown(CDC* pdc, CPoint& point, CRect& LockRect);
	virtual void OnMouseMove(CDC* pdc, CPoint& point);
	virtual void OnMouseUp(CDC* pdc, CPoint& point);
	virtual void OnPaint(CDC* pdc);
public:
	virtual void Translate(MouseMsg msg, CPoint& point, int nID);
};
