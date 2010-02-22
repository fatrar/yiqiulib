/*H***************************************************************************
 File            : RectangleDrawer.h
 Subsystem       : 
 Function Name(s): CRectangleDrawer
 Author          : YiQiu
 Date            : 2010-2-8  
 Time            : 15:24
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _RECTANGLEDRAWER_H_2010_2
#define _RECTANGLEDRAWER_H_2010_2

#include "Drawer.h"



class CRectangleDrawer :
	public CDrawer
{
    DECLARE_DYNAMIC(CRectangleDrawer)
public:
	CRectangleDrawer(void);
	virtual ~CRectangleDrawer(void);
protected:
     DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	inline void ReFreshPoint(int nIndex, const CPoint& point);

    enum DargDir
    {
        NO_Darg = -1,
        Darg_Top,
        Darg_Right,
        Darg_Bottom,
        Darg_Left,
    };


    inline void ReFreshPoint2(DargDir nIndex, const CPoint& point);


    DargDir IsDargPoint2(const CPoint& point);

    DargDir m_nDragDir;
};





#endif  // _RECTANGLEDRAWER_H_2010_


// End of file





