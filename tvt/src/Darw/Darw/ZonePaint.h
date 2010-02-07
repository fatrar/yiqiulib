/*H***************************************************************************
File            : ZonePaint.h
Subsystem       : 
Function Name(s): CZonePaint

Description     : 
Author          : Yiqiu
Date            : 2010/02/07
Revision        : 

History
-------


Copyright (c) xx Tech Co.,Ltd.
***************************************************************************H*/
#ifndef _ZONEPAINT_H_2010_2
#define _ZONEPAINT_H_2010_2


// CZonePaint

class CZonePaint : public CWnd
{
	DECLARE_DYNAMIC(CZonePaint)

public:
	CZonePaint();
	virtual ~CZonePaint();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
};




#endif
