/**CPP*************************************************************************
 File            : IVViewerFactory.cpp
 Subsystem       : 
 Function Name(s): IVViewerFactory
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

IIVStatisticFresher* GetStatisticFresher()
{
    return CIVLiveViewer::getInstancePtr();
}

IIVDataSaver* GetDataSaver(void)
{
    return CIVLiveDataBuf::getInstancePtr();
}

IIVDataSender* GetDataSender(void)
{
    return CIVLiveDataBuf::getInstancePtr();
}

}

namespace IVPlaybackFactory
{

IIVViewer* GetPlaybackViewer(void)
{
    return CIVPlaybackViewer::getInstancePtr();
}

IIVDataFound* GetDataFound(void)
{
    return CIVPlaybackDataBuf::getInstancePtr();
}

}

// End of file