/********************************************************************
	created:	2009/12/26
	created:	26:12:2009   9:53
	filename: 	f:\code\Darw\Darw\DarwCommon.h
	file path:	f:\code\Darw\Darw
	file base:	DarwCommon
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/
#pragma once


#include "IDrawer.h"


enum MouseMsg
{
	MouseMsg_LButtonDown,
	MouseMsg_LButtonUp,
	MouseMsg_MouseMove,
};

struct IMsgTranslator
{
	virtual void Translate(MouseMsg msg, CPoint& point, int nID) = 0;
};


class CDrawer :
	public IDrawer,
	public IMsgTranslator
{
public:
	CDrawer():m_bDrawing(false){}
protected:
	bool m_bDrawing;
};


inline BOOL LockCursor(CRect& LockRect){ return ClipCursor(&LockRect); }

inline BOOL UnLockCursor()
{
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN); 
	return ClipCursor(&CRect(0,0,x,y));
}

enum
{
	Point_Button_Widht = 15,
	Point_Button_Height = 15,


	MSG_0 = 0x8100,
	MSG_1,
	// ..

};

inline CRect& CalculateRect(int x, int y, CRect& rect)
{
	int x1 = x - Point_Button_Widht/2; 
	int y1 = y - Point_Button_Height/2; 
	int x2 = x + Point_Button_Widht/2; 
	int y2 = y + Point_Button_Height/2; 
	rect.SetRect(x1, y1, x2, y2);
	return rect;
}

