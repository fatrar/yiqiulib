/*H***************************************************************************
 File            : OtherUtil.h
 Subsystem       : 
 Function Name(s): COtherUtil
 Author          : YiQiu
 Date            : 2010-2-25  
 Time            : 14:48
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _OTHERUTIL_H_2010_2
#define _OTHERUTIL_H_2010_2






extern BOOL GetUseInputLineInfor(
    Windows::IDrawer* pDrawer, 
    const CRect& rect,
    WPG_TripwireEventDescription& lineDes);

extern BOOL GetUseInputRectangleInfor(
    Windows::IDrawer* pDrawer,
    const CRect& rect,
    WPG_PolygonF& polygon);

extern BOOL GetUseInputPolygonInfor(
    Windows::IDrawer* pDrawer,
    const CRect& rect,
    WPG_PolygonF& polygon);









#endif  // _OTHERUTIL_H_2010_


// End of file
