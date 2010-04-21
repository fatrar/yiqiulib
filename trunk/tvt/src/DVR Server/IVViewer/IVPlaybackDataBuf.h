/**H**************************************************************************
 File            : IVPlaybackDataBuf.h
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
***************************************************************************H*/
#ifndef _IVPLAYBACKDATABUF_H_2010_4
#define _IVPLAYBACKDATABUF_H_2010_4
#pragma once



class CIVPlaybackDataBuf :
    public IIVDataBuf,
    public IIVDataFound,
    public Singleton<CIVPlaybackDataBuf>
{
public:
    typedef PlayBackGroupTarget TGroupTarget;
    
public:
    CIVPlaybackDataBuf(void);
    ~CIVPlaybackDataBuf(void);

    // IIVDataBuf
public:
    virtual BaseTargetQueue* GetData(
        int nChannelID,
        const FILETIME& time );
    
    // IIVDataFound
public:
    virtual BOOL Open(
        int nChannelID,
        const char* pPath, 
        const FILETIME& time);

    virtual BOOL Close(
        int nChannelID,
        const FILETIME& time);

protected:
    enum
    {
        IV_Playback_Max_Channel = 64,
    };

    class ChannelTarget;

private:
    ChannelTarget* m_TargetMap;
};










#endif  // _IVPLAYBACKDATABUF_H_2010_


// End of file



