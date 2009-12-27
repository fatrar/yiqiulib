/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:32
	filename: 	F:\code\Darw\Darw\IDrawer.h
	file path:	F:\code\Darw\Darw
	file base:	IDrawer
	file ext:	h
	author:		heliang
	
	purpose:	
*********************************************************************/


#ifndef IDRAWER_H__
#define IDRAWER_H__

#include "stdafx.h"
//#include <afxwin.h>
//#include <GdiplusTypes.h>


struct IDrawer
{
	virtual void OnMouseDown(CDC* pdc, CPoint& point, CRect& pLockRect) = 0;
	virtual void OnMouseMove(CDC* pdc, CPoint& point) = 0;
	virtual void OnMouseUp(CDC* pdc, CPoint& point) = 0;
	virtual void OnPaint(CDC* pdc) = 0;
};



//#define A(x) T_##x
//#define LoopDefine(0) MSG_0 
//#define LoopDefine(x) LoopDefine(x-1)+1


#endif // IDRAWER_H__