/*H***************************************************************************
File            : BaseIVViewer.h
Subsystem       : 
Function Name(s): CBaseIVViewer

Description     : 
Author          : Yiqiu
Date            : 2010/01/18
Revision        : 

History
-------


Copyright (c) Shenzhen Sunline Tech Co.,Ltd.
***************************************************************************H*/
#ifndef _BASEIVVIEWER_H_2010_1
#define _BASEIVVIEWER_H_2010_1





class CBaseIVViewer :
    public IIVViewer
{
public:
    CBaseIVViewer(void);
    virtual ~CBaseIVViewer(void);

public:
    virtual TargetQueue* GetIVData(int nChannelID) = 0;

    // bState=true显示目标和路径，否则隐藏
    virtual BOOL ShowObjTrace(int nChannelID, bool bState);

    // 得到目标和路径是否正在显示
    virtual BOOL GetObjTraceState(int nChannelID, bool& bState);

    virtual void SetDataShowState(int nChannelID, int nState);

protected:
    int m_nState;
    BOOL m_bShow;
};






#endif