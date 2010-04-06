/**CPP*************************************************************************
 File            : IVLiveFactory.cpp
 Subsystem       : 
 Function Name(s): CIVLiveFactory
 Author          : YiQiu
 Date            : 2010-4-6  
 Time            : 16:00
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"



namespace IVLiveFactory
{



IIVLiveViewer* GetLiveViewer(void)
{
    return CIVLiveViewer::getInstancePtr();
}

IIVLiveViewerEx* GetLiveViewerEx(void)
{
    return CIVLiveViewer::getInstancePtr();
}

IIVDataSaver* GetDataSaver(void)
{
    return CIVDataBuf::getInstancePtr();
}

IIVDataSender* GetDataSender(void)
{
    return CIVDataBuf::getInstancePtr();
}

}

// End of file