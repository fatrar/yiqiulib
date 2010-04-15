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
public:
    CLineDrawer(CWnd* pWnd);
    virtual ~CLineDrawer(void){};

protected:
    virtual BOOL OnMouseMove(UINT nFlags, CPoint& point);
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint& point);
    virtual BOOL OnLButtonDown(UINT nFlags, CPoint& point);
    virtual void OnPaint(CDC& dc,BOOL bSelect=FALSE);
};

class CArrowLineDrawer:
    public CLineDrawer
{
public:
    CArrowLineDrawer(CWnd* pWnd);
    virtual ~CArrowLineDrawer(void){};
protected:
    virtual void OnPaint(CDC& dc,BOOL bSelect=FALSE);
    virtual void SendCommond(DrawCommond c, void* p1=NULL, void* p2=NULL);

protected:
    virtual void DrawArrow(
        CDC* pdc, 
        const CPoint& p, 
        size_t d,
        double o,
        bool bUp = true,
        DWORD dwCount = MAXDWORD );


protected:
    enum 
    {
        ArrowLineLen = 50,
        ArrowHeadLen = 10,
    };

    DWORD m_dwDrawCommond; 
    DWORD m_szdwNumber[2];
};

class CArrowLineDrawerEx:
    public CArrowLineDrawer
{
public:
    CArrowLineDrawerEx(CWnd* pWnd)
        : CArrowLineDrawer(pWnd){ }
    virtual ~CArrowLineDrawerEx(void){};

protected:
    virtual void SendCommond(DrawCommond c, void* p1=NULL, void* p2=NULL);

protected:
    virtual void DrawArrow(
        CDC* pdc, 
        const CPoint& p, 
        size_t d,
        double o,
        bool bUp = true,
        DWORD dwCount = MAXDWORD );


};



#endif  // _LINEDRAWER_H_2010_


// End of file


