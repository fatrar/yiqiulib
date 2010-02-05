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


Copyright (c) xx Tech Co.,Ltd.
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
    virtual TargetQueue* GetIVData(
        int nChannelID,
        const FILETIME& time) = 0;

    // IIVViewer
public:
    // 显示目标矩形框和路径时的回调，
    // 传入DC和正在播放的数据帧的时间
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time);

    // bState=true显示目标和路径，否则隐藏
    virtual BOOL ShowObjTrace(int nChannelID, bool bState);

    // 得到目标和路径是否正在显示
    virtual BOOL GetObjTraceState(int nChannelID, bool& bState);

    virtual void SetDataShowState(int nChannelID, int nState);

protected:
    typedef deque<WPG_PointF> PointList;
    typedef map<size_t, PointList*> ChannelPoint;

    struct ViewerBuf
    {
        WPG_Target TarBuf[TARGET_MAX_NUM];
        int nTarCount;
        FILETIME time;
        int ChannelID;

        ChannelPoint PointBuf;
    };

    void RefrehPoint(
        ChannelPoint& PointBuf,
        const TargetQueue* DataQueue,
        const FILETIME& time );

    void DrawTrace(
        const HDC dc,
        const ChannelPoint& PointBuf,
        const RECT& rect );

protected:
    int m_nState;
    BOOL m_bShow;

    //typedef map<int, ChannelPoint> AllChannelPoint;
   // AllChannelPoint m_PointRecord;

    ViewerBuf* m_pViewerBuf;  
};






#endif