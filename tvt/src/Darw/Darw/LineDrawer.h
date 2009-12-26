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

class CPointButton;

class CLineDrawer :
	public CDrawer
{
public:
	CLineDrawer(void);
	~CLineDrawer(void);

public:
	virtual void OnMouseDown(CDC* pdc, CPoint& point, CRect* pLockRect=NULL);
	virtual void OnMouseMove(CDC* pdc, CPoint& point);
	virtual void OnMouseUp(CDC* pdc, CPoint& point);
	virtual void OnPaint(CDC* pdc);

public:
	void Translate(MouseMsg msg, CPoint& point, int nID);

private:
	void Release();
	void Create(CWnd* pWnd, CPoint& point);

private:
	CPointButton* m_Button[2];
	CPoint m_Point[2];
	bool m_bIsOK;
	bool m_IsDargPointBt;
	int m_nDargBtIndex;
};
