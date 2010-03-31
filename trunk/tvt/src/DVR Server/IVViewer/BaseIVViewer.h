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


static int g_PointList = 0;


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
    // ��ʾĿ����ο��·��ʱ�Ļص���
    // ����DC�����ڲ��ŵ�����֡��ʱ��
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time);

    // bState=true��ʾĿ���·������������
    virtual void SetObjTraceShowState(int nChannelID, bool bShow);

    virtual void GetObjTraceShowState(int nChannelID, bool& bShow);

    virtual void SetDataShowState(int nChannelID, int nState);

    virtual void GetDataShowState(int nChannelID, int& nState);

    // ����ͳ��
    virtual BOOL ResetStatistic(int nChannelID);

    // bFlag=true��ʼͳ�ƣ�flaseֹͣ
    virtual BOOL StartStatistic(int nChannelID, bool bFlag);

    // �õ�ͳ��״̬
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag);

protected:
    typedef deque<WPG_PointF> PointList;
    typedef map<size_t, PointList*> ChannelPoint;

    struct ViewerBuf
    {
        ~ViewerBuf()
        {
            g_PointList -= PointBuf.size();
            StlHelper::STLDeleteAssociate(PointBuf);
        }
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
    struct TShowState
    {
        TShowState():nState(Show_Object_Trace),bShow(true){}
        int nState;
        bool bShow;
    };
    
    //map<int, TShowState> m_ShowState;
    TShowState m_ShowState[MAX_IV_Channel];

    ViewerBuf* m_pViewerBuf;  
};






#endif