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
    public CBaseIVViewer<IIVViewer>,
    public Singleton<CIVPlaybackViewer>
{
public:
    CIVPlaybackViewer(void);
    ~CIVPlaybackViewer(void);

    // CBaseIVViewer
public:
    virtual TargetQueue* GetIVData(
        int nChannelID,
        const FILETIME& time);

public:
};










#endif  // _IVPLAYBACKVIEWER_H_2010_


// End of file



