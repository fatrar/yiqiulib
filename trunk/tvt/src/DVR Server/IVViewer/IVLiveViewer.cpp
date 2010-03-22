/*H***************************************************************************
 File            : IVLiveViewer.cpp
 Subsystem       : 
 Function Name(s): CIVLiveViewer
 Author          : YiQiu
 Date            : 2010-1-14  
 Time            : 17:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "BaseIVViewer.h"
#include "IVLiveViewer.h"
#include "IVDataBuf.h"

IIVDataBuf* CIVLiveViewer::s_pIVDataBuf = CIVDataBuf::getInstancePtr();

CIVLiveViewer::CIVLiveViewer(void)
{
}

CIVLiveViewer::~CIVLiveViewer(void)
{
}

TargetQueue* CIVLiveViewer::GetIVData( 
    int nChannelID,
    const FILETIME& time )
{
    return s_pIVDataBuf->GetData(nChannelID, time);
}

IIVViewer* IVLiveFactory::GetLiveViewer(void)
{
    return CIVLiveViewer::getInstancePtr();
}

// End of file