/**H**************************************************************************
 File            : IVPlaybackViewer.h
 Subsystem       : 
 Function Name(s): CIVPlaybackViewer
 Author          : YiQiu
 Date            : 2010-4-14  
 Time            : 15:38
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVPLAYBACKVIEWER_H_2010_4
#define _IVPLAYBACKVIEWER_H_2010_4
#pragma once



class CIVPlaybackViewer :
    public CBaseIVViewer<IIVPlayBackViewer>,
    public Singleton<CIVPlaybackViewer>
{
public:
    CIVPlaybackViewer(void);
    ~CIVPlaybackViewer(void);

public:
    static IIVDataBuf* s_pIVDataBuf;

    // CBaseIVViewer
public:
    virtual BaseTargetQueue* GetIVData(
        int nChannelID,
        const FILETIME& time);

    virtual void RefrehPoint(
        ChannelPoint& PointBuf,
        const BaseTargetQueue* DataQueue );

    // IIVPlayBackViewer
public:
    virtual void SetPlayBackMode(PlayBack_Mode Mode);

protected:
    PlayBack_Mode m_PlayBackMode;
    BOOL m_NeedClearBuf;
};










#endif  // _IVPLAYBACKVIEWER_H_2010_


// End of file



