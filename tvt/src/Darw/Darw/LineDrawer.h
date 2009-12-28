/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:40
	filename: 	f:\code\Darw\Darw\LineDrawer.h
	file path:	f:\code\Darw\Darw
	file base:	LineDrawer
	file ext:	h
	author:		heliang
	
	purpose:	
*********************************************************************/
#pragma once

#include "DarwCommon.h"


class CLineDrawer :
	public CDrawer
{
public:
	CLineDrawer(void);
	~CLineDrawer(void);

public:
	virtual void OnMouseDown(CDC* pdc, CPoint& point, CRect& LockRect);
	virtual void OnMouseMove(CDC* pdc, CPoint& point);
	virtual void OnMouseUp(CDC* pdc, CPoint& point);
	virtual void OnPaint(CDC* pdc);

private:	
	 
};







// End of file



