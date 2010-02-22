/*H***************************************************************************
 File            : LineDrawer.h
 Subsystem       : 
 Function Name(s): CLineDrawer
 Author          : YiQiu
 Date            : 2010-2-9  
 Time            : 13:56
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _LINEDRAWER_H_2010_2
#define _LINEDRAWER_H_2010_2
#include "Drawer.h"



class CLineDrawer :
    public CDrawer
{
    DECLARE_DYNAMIC(CLineDrawer)
public:
    CLineDrawer(void);
    virtual ~CLineDrawer(void);

protected:
    DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
};


class CArrowLineDrawer:
    public CLineDrawer
{
public:
    CArrowLineDrawer();
    DECLARE_DYNAMIC(CArrowLineDrawer)
protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

    virtual void SendCommond(DrawCommond c, void* p1, void* p2);
protected:
    enum 
    {
        ArrowLineLen = 50,
        ArrowHeadLen = 10,
    };

    DWORD m_dwDrawCommond;
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



#endif  // _LINEDRAWER_H_2010_


// End of file


