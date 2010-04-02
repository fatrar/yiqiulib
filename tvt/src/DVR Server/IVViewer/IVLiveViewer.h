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

#include "BaseIVViewer.h"


class CIVLiveViewer :
    public CBaseIVViewer<IIVLiveViewer>,
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
    // 重置统计
    virtual BOOL ResetStatistic(int nChannelID);

    // bFlag=true开始统计，flase停止
    virtual BOOL StartStatistic(int nChannelID, bool bFlag);

    // 得到统计状态
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag);


    virtual void AddRule(
        int nChannelID,
        WPG_Rule& Rule);

    virtual void RemoveRule(
        int nChannelID,
        WPG_Rule& Rule);

    virtual void ModifyRule(
        int nChannelID,
        WPG_Rule& Rule);

    virtual void ClearAllRule(int nChannelID);

    virtual void PaintRule(
        int nChannelID,
        const HDC dc,
        const RECT& rect);

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
            WPG_TripwireEventDescription LineInfo;
            WPG_PolygonF PolygonInfo;
        } PointInfo;
    }; 

    typedef map<IV_RuleID, GraphInfo*> RuleGraphMap;

protected:
    static bool TranslateRuleToGraphInfo(
        const WPG_Rule& Rule,
        GraphInfo*& Info);
    
    static void PaintPolygon(
        const HDC dc,
        const RECT& rect,
        WPG_PolygonF& Polygon);
    
    static void PaintArrowLine(
        const HDC dc,
        const RECT& rect,
        WPG_TripwireEventDescription& Line);

protected:
    RuleGraphMap m_AllRuleGraph[MAX_IV_Channel];
    CriticalSection m_cs[MAX_IV_Channel];
};





#endif  // End of file




