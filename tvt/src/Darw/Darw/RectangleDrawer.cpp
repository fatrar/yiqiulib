/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:45
	filename: 	f:\code\Darw\Darw\RectangleDrawer.cpp
	file path:	f:\code\Darw\Darw
	file base:	RectangleDrawer
	file ext:	cpp
	author:		heliang
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "RectangleDrawer.h"

CRectangleDrawer::CRectangleDrawer(void)
{
}

CRectangleDrawer::~CRectangleDrawer(void)
{
}

void CRectangleDrawer::OnMouseDown(
    CDC* pdc,
    CPoint& point,
    CRect& LockRect/*=NULL*/ )
{

}

void CRectangleDrawer::OnMouseMove( CDC* pdc, CPoint& point )
{

}

void CRectangleDrawer::OnMouseUp( CDC* pdc, CPoint& point )
{

}

void CRectangleDrawer::OnPaint(CDC* pdc)
{

}

void CRectangleDrawer::Translate( MouseMsg msg, CPoint& point, int nID )
{
	switch (msg)
	{
	case MouseMsg_LButtonDown:
		break;
	case MouseMsg_LButtonUp:
		break;
	case MouseMsg_MouseMove:
		break;
	}
}