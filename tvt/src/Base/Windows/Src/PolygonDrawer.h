/*H***************************************************************************
 File            : PolygonDrawer.h
 Subsystem       : 
 Function Name(s): CPolygonDrawer
 Author          : YiQiu
 Date            : 2010-2-8  
 Time            : 15:03
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _POLYGONDRAWER_H_2010_2
#define _POLYGONDRAWER_H_2010_2
#include "Drawer.h"


class CPolygonDrawer :
    public CDrawer
{
public:
    CPolygonDrawer(CWnd* pWnd);
    virtual ~CPolygonDrawer(void);

    virtual void SendCommond(DrawCommond c, void* p1=NULL, void* p2=NULL);

    virtual void Clear()
    {
        m_PointQueue.clear();
        m_PointQueue.push_back(CPoint());
        m_bIsOK=false;
        m_pWnd->RedrawWindow();
    }
protected:
    virtual BOOL OnMouseMove(UINT nFlags, CPoint& point);
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint& point);
    virtual BOOL OnLButtonDown(UINT nFlags, CPoint& point);
    virtual void OnPaint(CDC& dc, BOOL bSelect=FALSE);

protected:
    bool m_bFirst;
};


//    public CDrawer
//{
//    DECLARE_DYNAMIC(CPolygonDrawer)
//public:
//    CPolygonDrawer(void);
//    virtual ~CPolygonDrawer(void);
//
//    void SendCommond(DrawCommond c, void* p1=NULL, void* p2=NULL);
//
//protected:
//    DECLARE_MESSAGE_MAP()
//
//protected:
//    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//    afx_msg void OnPaint();
//
//protected:
//    bool m_bFirst;
//};




#endif  // _POLYGONDRAWER_H_2010_


// End of file



