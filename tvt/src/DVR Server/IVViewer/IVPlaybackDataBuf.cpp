/**CPP*************************************************************************
 File            : IVPlaybackDataBuf.cpp
 Subsystem       : 
 Function Name(s): CIVPlaybackDataBuf
 Author          : YiQiu
 Date            : 2010-4-14  
 Time            : 15:31
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVPlaybackDataBuf.h"




CIVPlaybackDataBuf::CIVPlaybackDataBuf(void)
{
    m_TargetMap = new ChannelTarget[IV_Playback_Max_Channel];
}

CIVPlaybackDataBuf::~CIVPlaybackDataBuf(void)
{
    delete[] m_TargetMap;
}

//
// ****************** IIVDataBuf ******************
// {
BaseTargetQueue* CIVPlaybackDataBuf::GetData( 
    int nChannelID,
    const FILETIME& time )
{
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    TGroupTarget* pGroupTarget = ChanTarget.Find(time);
    if ( pGroupTarget == NULL )
    {
        return NULL;
    }

    return pGroupTarget->m_TargetQueue;
}

// }
// IIVDataBuf

//
// ****************** IVDataFound ******************
//
BOOL CIVPlaybackDataBuf::Open( 
    int nChannelID,
    const char* pPath, 
    const FILETIME& time )
{
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    return ChanTarget.Open(pPath, time);
}

BOOL CIVPlaybackDataBuf::Close(
    int nChannelID,
    const FILETIME& time )
{
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    return ChanTarget.Close(time);
}
// }
// IVDataFound


// End of file