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


//          _ 
//         /|\         
//          |   
// _________|__________
class CArrowLine:
	public CLineDrawer
{
public:
	CArrowLine();
	~CArrowLine();

	enum ArrowDirection { Dir_Left=0, Dir_Right, Dir_All};
public:
	DrawerErr OnCommand(DrawerCommand command, WPARAM wParam = 0, LPARAM lParam = 0);
	

private:
	ArrowDirection m_dir;
}



// End of file



