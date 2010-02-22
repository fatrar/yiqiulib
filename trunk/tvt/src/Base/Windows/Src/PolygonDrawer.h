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

BEGIN_WINDOWS_NAMESPACE

class CPolygonDrawer :
    public CDrawer
{
    DECLARE_DYNAMIC(CPolygonDrawer)
public:
    CPolygonDrawer(void);
    virtual ~CPolygonDrawer(void);

    void SendCommond(DrawCommond c, void* p1, void* p2);

protected:
    DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();

protected:
    bool m_bFirst;
};



END_WINDOWS_NAMESPACE





#endif  // _POLYGONDRAWER_H_2010_


// End of file



