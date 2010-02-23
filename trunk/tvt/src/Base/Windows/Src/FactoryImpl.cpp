/*H***************************************************************************
 File            : FactoryImpl.cpp
 Subsystem       : 
 Function Name(s): CFactoryImpl
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 14:05
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "LineDrawer.h"
#include "PolygonDrawer.h"
#include "RectangleDrawer.h"



IDrawer* DrawerFactory::CreateDrawer( IDrawerGraphType t )
{
    switch (t)
    {
    case IDrawer_Rectangle: return new CRectangleDrawer();
    case IDrawer_Polygon:   return new CPolygonDrawer();
    case IDrawer_Line:      return new CLineDrawer();
    case IDrawer_ArrowLine: return new CArrowLineDrawer();
    default:
        return NULL;
    }
}

void DrawerFactory::DestoryDrawer( IDrawer* p ){ safeDelete(p); }



// End of file





