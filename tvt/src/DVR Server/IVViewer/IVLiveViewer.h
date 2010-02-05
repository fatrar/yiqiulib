/*H***************************************************************************
 File            : IVLiveViewer.h
 Subsystem       : 
 Function Name(s): CIVLiveViewer
 Author          : YiQiu
 Date            : 2010-1-14  
 Time            : 17:49
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVLIVEVIEWER_H_2010_
#define _IVLIVEVIEWER_H_2010_




class CIVLiveViewer :
    public CBaseIVViewer,
    public Singleton<CIVLiveViewer>
{
public:
    CIVLiveViewer(void);
    virtual ~CIVLiveViewer(void);

    // IIVViewer
public:
    virtual TargetQueue* GetIVData(
        int nChannelID,
        const FILETIME& time);

public:
    static IIVDataBuf* s_pIVDataBuf;

private:
   
};





#endif  // End of file




