/*H***************************************************************************
 File            : OtherUtil.cpp
 Subsystem       : 
 Function Name(s): COtherUtil
 Author          : YiQiu
 Date            : 2010-2-25  
 Time            : 14:49
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "OtherUtil.h"


#define TraslateToRate(p1, p2, w, h) \
    p1.x = float(p2.x)/w; \
    p1.y = float(p2.y)/h

BOOL GetUseInputLineInfor( 
    Windows::IDrawer* pDrawer,
    const CRect& rect,
    WPG_TripwireEventDescription& lineDes )
{
    if ( pDrawer->PointCount() != 2 )
    {
        return FALSE;
    }

    CPoint szBufPoint[2];
    pDrawer->GetUserInput(szBufPoint, 2);

    int nW = rect.Width(), nH = rect.Height();
    TraslateToRate(lineDes.startPoint, szBufPoint[0], nW, nH);
    TraslateToRate(lineDes.endPoint, szBufPoint[1], nW, nH);
    Windows::IDrawer::DrawCommond C = Windows::IDrawer::Line_Show_All;
    pDrawer->SendCommond(Windows::IDrawer::Get_Line_Dir, &C);
    switch (C)
    {
    case Windows::IDrawer::Line_Show_Left:
        lineDes.direction = RIGHT_TO_LEFT;
        break;
    case Windows::IDrawer::Line_Show_Right:
        lineDes.direction = LEFT_TO_RIGHT;
        break;
    default:
        lineDes.direction = ANY_DIRECTION;
        break;
    }
    
    return TRUE;
}

BOOL GetUseInputRectangleInfor(
    Windows::IDrawer* pDrawer, 
    const CRect& rect,
    WPG_PolygonF& polygon )
{
    if ( pDrawer->PointCount() != 4 )
    {
        return FALSE;
    }

    CPoint szBufPoint[4];
    pDrawer->GetUserInput(szBufPoint, 4);

    int nW = rect.Width(), nH = rect.Height();
    for ( int i = 0; i<4; ++i)
    {
        TraslateToRate(polygon.points[i], szBufPoint[i], nW, nH);
    }
    polygon.numPoints = 4;
    return TRUE;
}

BOOL GetUseInputPolygonInfor(
    Windows::IDrawer* pDrawer,
    const CRect& rect,
    WPG_PolygonF& polygon )
{
    size_t nPointCount = pDrawer->PointCount();
    if ( nPointCount == 0 ||
         nPointCount > WPG_MAX_NUM_OF_POINTS)
    {
        return FALSE;
    }

    CPoint szBufPoint[WPG_MAX_NUM_OF_POINTS];
    pDrawer->GetUserInput(szBufPoint, WPG_MAX_NUM_OF_POINTS);

    int nW = rect.Width(), nH = rect.Height();
    for ( size_t i = 0; i<nPointCount; ++i)
    {
        TraslateToRate(polygon.points[i], szBufPoint[i], nW, nH);
    }
    polygon.numPoints = nPointCount;
    return TRUE;
}



// End of file