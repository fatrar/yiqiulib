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

    // bState=true��ʾĿ���·������������
    virtual BOOL ShowObjTrace(int nChannelID, bool bState);

    // �õ�Ŀ���·���Ƿ�������ʾ
    virtual BOOL GetObjTraceState(int nChannelID, bool& bState);

    virtual void SetDataShowState(int nChannelID, int nState);

protected:
    int m_nState;
    BOOL m_bShow;
};






#endif