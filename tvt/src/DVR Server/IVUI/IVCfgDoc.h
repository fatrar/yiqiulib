/**H**************************************************************************
 File            : IVCfgDoc.h
 Subsystem       : 
 Function Name(s): CIVCfgDoc
 Author          : YiQiu
 Date            : 2010-3-4  
 Time            : 17:47
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVCFGDOC_H_2010_3
#define _IVCFGDOC_H_2010_3
#pragma once


class CIVRuleCfgDoc;
class CIVAlarmOutCfgDoc;
class CIVScheduleCfgDoc;

struct IRuleTrigger
{
    virtual void OnRuleRemove(int nChannelID, const char* pIdentityID)=0;
    virtual void OnRuleAdd(int nChannelID, const char* pIdentityID)=0;
    virtual void OnUseIV(int nChannelID, BOOL bEnbale) =0;
};

typedef void (IRuleTrigger::*OnRuleXXFn)(int,const char*);

#define IVCfgDoc_Use_Map

/**
*@brief 暂时不考虑，Camera Tree使用智能的通道 采用不同标记
        根据现在的资料将某个Tree的节点改颜色比较复杂，要重写Treectrl
*/
class CIVCfgDoc
{
public:
    /**
    *@brief Load XML Data To Memory
    */
    static void Init();

protected:
    /**
    *@brief Register Rule Add Or Remove Trigger
    *@param	pRuleTrigger Trigger Callback Ptr
    */
    static void RegisterRuleTrigger(IRuleTrigger* pRuleTrigger);  

    /**
    *@brief Init Camera Tree
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    void OnInitCameraTree(
        int nChannelID, 
        HTREEITEM Item);

    /** 我也写了一个用宏实现的方式，但因为宏不能调式，改用模板实现
    *@brief Get Rule,Schedule,AlarmOut Data
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    template<typename T>
    T* GetIVRuleCfgXX(
        int nChannelID,
        HTREEITEM Item);

    /** 
    *@brief Update Rule,Schedule,AlarmOut Data
    *@param	nChannelID  Channel ID
    *@param	V  Rule or Schedule or AlarmOut
    *@param Item        Channel TreeCtrl Item HANDLE
    *@param IsRef       ...
    */
    template<typename T, typename Tfn, Tfn fn>
    void UpdateRuleCfgXX(
        int nChannelID, 
        const T& V,
        HTREEITEM Item,
        BOOL IsRef = TRUE);

    template<typename T>
    inline const char* GetRuleID(
        const T& t,
        HTREEITEM Item);

protected:
    struct RuleSettings
    {
        RuleSettings(){}
        RuleSettings(
            const WPG_Rule& _Rule,
            const AlarmOutSettings& _Alarm = g_DefaultAlarmOutSettings,
            const ScheduleSettings& _Sch = g_DefaultScheduleSettings )
            : Rule(_Rule)
            , Sch(_Sch)
            , Alarm(_Alarm) {}

        WPG_Rule Rule;
        ScheduleSettings Sch;
        AlarmOutSettings Alarm;
    };


private:
    typedef map<const char*, RuleSettings*> RuleSettingMap;

#ifdef IVCfgDoc_Use_Map
    typedef map<int, RuleSettingMap> AllRuleSettingMap;
    static AllRuleSettingMap m_Doc;
#else
    static RuleSettingMap m_Doc[Max_Channel]; 
#endif
    
    typedef deque<IRuleTrigger*> RuleTriggerList;
    static RuleTriggerList m_RuleTrigger;
    static set<int> m_UseChannel;

protected:
    CTreeCtrl m_CameraTree;
    CStatic m_TreeGroup;

    friend CIVRuleCfgDoc;
    friend CIVAlarmOutCfgDoc;
    friend CIVScheduleCfgDoc;
};



class CIVRuleCfgDoc :
    protected CIVCfgDoc
{
protected:
    /**
    *@brief Init Camera Tree
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    //void OnInitCameraTree(
    //    int nChannelID,
    //    HTREEITEM Item);


    WPG_Rule* GetRule(
        int nChannelID,
        HTREEITEM Item );

    /**
    *@brief Add Rule To Doc(memory And XML), And Add Tree Item, 
    *           if Set Trigger, Do Trigger Function
    *@param	nChannelID  Channel ID
    *@param Rule        WPG Rule 
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    void AddRule(
        int nChannelID, 
        const WPG_Rule& Rule,
        HTREEITEM Item);

    /**
    *@brief Remove Rule To Doc(memory And XML), And Add Tree Item, 
    *          if Set Trigger, Do Trigger Function
    *@param	nChannelID  Channel ID
    *@param Rule        WPG Rule 
    *@param Item        Rule TreeCtrl Item HANDLE
    */
    void RemoveRule(
        int nChannelID,
        HTREEITEM Item);

    /**
    *@brief Update Rule To Doc(memory And XML), And Add Tree Item
    *@param	nChannelID  Channel ID
    *@param Rule        WPG Rule 
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Rule point is pass to GetRule
    */
    void UpdateRule(
        int nChannelID,
        const WPG_Rule& Rule, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);
private:
    
    template<OnRuleXXFn T>
    inline void DoTriggerTFun(int nChannelID, const char* pIdentityID);
};

class CIVAlarmOutCfgDoc :
    protected CIVCfgDoc
{
protected:
    /**
    *@brief Init Camera Tree
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    //void OnInitCameraTree(
    //    int nChannelID,
    //    HTREEITEM Item);

    AlarmOutSettings* GetAlarmOut(
        int nChannelID,
        HTREEITEM Item );

    /**
    *@brief Update AlarmOut To Doc(memory And XML), And Add Tree Item
    *@param	nChannelID  Channel ID
    *@param Alarm       AlarmOutSettings
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Alarm point is pass to GetAlarmOut
    */
    void UpdateAlarmOut(
        int nChannelID,
        const AlarmOutSettings& Alarm, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);
};

class CIVScheduleCfgDoc :
    protected CIVCfgDoc
{
protected:
    /**
    *@brief Init Camera Tree
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    //void OnInitCameraTree(
    //    int nChannelID, 
    //    HTREEITEM Item);

    ScheduleSettings* GetSchedule(
        int nChannelID,
        HTREEITEM Item );

    /**
    *@brief Update AlarmOut To Doc(memory And XML), And Add Tree Item
    *@param	nChannelID  Channel ID
    *@param Sch         ScheduleSettings 
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Sch point is pass to GetSchedule
    */
    void UpdateSchedule(
        int nChannelID,
        const ScheduleSettings& Sch, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);
};




#endif  // _IVCFGDOC_H_2010_


// End of file


