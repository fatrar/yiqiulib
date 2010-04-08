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
    public CBaseIVViewer<IIVLiveViewerEx>,
    public Singleton<CIVLiveViewer>
{
public:
    CIVLiveViewer(void);
    virtual ~CIVLiveViewer(void);

    // CBaseIVViewer
public:
    virtual TargetQueue* GetIVData(
        int nChannelID,
        const FILETIME& time);

    // IIVLiveViewer
public:
    // ����ͳ��
    virtual BOOL ResetStatistic(int nChannelID);

    // bFlag=true��ʼͳ�ƣ�flaseֹͣ
    virtual BOOL StartStatistic(int nChannelID, bool bFlag);

    // �õ�ͳ��״̬
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag);

    virtual void PaintRule(
        int nChannelID,
        const HDC dc,
        const RECT& rect);

    virtual void ShowRuleAndStatistic(
        int nChannelID,
        bool bShow);

    // IIVLiveViewerEx
public:
    virtual void AddRule(
        int nChannelID,
        const WPG_Rule& Rule);

    virtual void RemoveRule(
        int nChannelID,
        BYTE (&ID)[16]);

    virtual void ModifyRule(
        int nChannelID,
        const WPG_Rule& Rule);

    virtual void ClearAllRule(int nChannelID);

public:
    static IIVDataBuf* s_pIVDataBuf;

protected:
    enum Graph_Type
    {
        IDrawer_Polygon,
        IDrawer_ArrowLine,
    };

    struct GraphInfo
    {
        Graph_Type Type;

        union
        {
            struct{
                WPG_TripwireEventDescription Info;
                BOOL IsStatistic;
            } LineInfo;
            WPG_PolygonF PolygonInfo;
        } PointInfo;
    }; 

    typedef map<IV_RuleID, GraphInfo*> RuleGraphMap;

public:
    enum
    {
        Point_Radii = 5,
        Font_Color = RGB(255,0,0),
    };

protected:
    static bool TranslateRuleToGraphInfo(
        const WPG_Rule& Rule,
        GraphInfo*& Info);
    
    void PaintPolygon(
        const HDC dc,
        const RECT& rect,
        WPG_PolygonF& Polygon);
    
    void PaintArrowLine(
        const HDC dc,
        const RECT& rect,
        WPG_TripwireEventDescription& Line,
        DWORD dwShowCount );

    struct StatisticData
    {
        StatisticData():dwCount(0),IsOk(TRUE){}
        DWORD dwCount;
        BOOL IsOk; 
    };
protected:
    RuleGraphMap m_AllRuleGraph[MAX_IV_Channel];
    bool m_IsShowRule[MAX_IV_Channel];
    StatisticData m_StatisticData[MAX_IV_Channel];
    CriticalSection m_cs[MAX_IV_Channel];
    //HFONT m_hFont;
};





#endif  // End of file




