/**CPP*************************************************************************
 File            : IVPlaybackViewer.cpp
 Subsystem       : 
 Function Name(s): CIVPlaybackViewer
 Author          : YiQiu
 Date            : 2010-4-14  
 Time            : 15:39
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVPlaybackViewer.h"

IIVDataBuf* CIVPlaybackViewer::s_pIVDataBuf = CIVPlaybackDataBuf::getInstancePtr();


CIVPlaybackViewer::CIVPlaybackViewer(void)
{
}

CIVPlaybackViewer::~CIVPlaybackViewer(void)
{
}

TargetQueue* CIVPlaybackViewer::GetIVData(
    int nChannelID,
    const FILETIME& time )
{
    return s_pIVDataBuf->GetData(nChannelID,time);
}





// End of file