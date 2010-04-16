/**H**************************************************************************
 File            : IVCfgDoc.h
 Subsystem       : 
 Function Name(s): CIVCfgDoc
 Author          : YiQiu
 Date            : 2010-3-4  
 Time            : 17:47
 Description     : 
   该文件的CIVCfgDoc大量使用了模板使用抽象的功能，
   如果没有C++这么强大的泛型编程，估计CPP估计要多写一倍多的代码
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
    virtual void OnRuleRemove(
        int nChannelID,
        const char* pIdentityID,
        CString& strRuleName )=0;
    virtual void OnRuleAdd(
        int nChannelID,
        const char* pIdentityID,
        CString& strRuleName)=0;
    virtual void OnUseIV(int nChannelID, BOOL bEnbale) =0;
};

typedef void (IRuleTrigger::*OnRuleXXFn)(int,const char*,CString&);


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

    /**
    *@brief Unload data from Memory
    */
    static void Unit();

public:
    static int s_nDeviceNum;
    static int s_nMaxChannel;
    static int s_nIVChannelNumByDevice;
    static int s_nMaxRuleNumByIVChannel;

protected:
    /**
    *@brief Register Rule Add Or Remove Trigger
    *@param	pRuleTrigger Trigger Callback Ptr
    */
    static void RegisterRuleTrigger(
        IRuleTrigger* pRuleTrigger);  

    /**
    *@brief Init Camera Tree
    *@param	nChannelID  Channel ID
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    void OnInitCameraTree(
        int nChannelID, 
        HTREEITEM Item);

    static BOOL IsIVChannel(int nChannelID);

    const IV_RuleID* GetRuleID(HTREEITEM Item);

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
    static RuleSettingMap* m_pDoc; 
    static int* m_pShowState;
    static BOOL* m_pIsHaveStatistic;

    typedef deque<IRuleTrigger*> RuleTriggerList;
    static RuleTriggerList m_RuleTrigger;
    static set<int> m_UseChannel;

    

protected:
    CTreeCtrl m_CameraTree;
    CStatic m_TreeGroup;

    friend CIVRuleCfgDoc;
    friend CIVAlarmOutCfgDoc;
    friend CIVScheduleCfgDoc;

private:
    /** 我也写了一个用宏实现的方式，但因为宏不能调式，改用模板实现
    *@brief Get Rule,Schedule,AlarmOut Data
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    template<typename T, T RuleSettings::*P>
    T* GetIVRuleCfgXX(
        HTREEITEM Item);

    /** 
    *@brief Update Rule,Schedule,AlarmOut Data
    *@param	V  Rule or Schedule or AlarmOut
    *@param Item        Channel TreeCtrl Item HANDLE
    *@param IsRef       ...
    */
    template<typename T, typename Tfn, Tfn fn>
    void UpdateRuleCfgXX(
        const T& V,
        HTREEITEM Item,
        BOOL IsRef = TRUE);

    template<typename T>
    inline const char* GetRuleIDXX(
        const T& t,
        HTREEITEM Item,
        int& nChannelID );

    template<
        typename T, T RuleSettings::*P,
        typename Tfn1, Tfn1 fn1,
        typename Tfn2, Tfn2 fn2>
    void SetCfgToAllXX(const T& V);
};

class CIVRuleCfgDoc :
    protected CIVCfgDoc
{
protected:
    /**
    *@brief Get Rule
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    WPG_Rule* GetRule(
        HTREEITEM Item );

    /**
    *@brief Add Rule To Doc(memory And XML), And Add Tree Item, 
    *           if Set Trigger, Do Trigger Function
    *@param	nChannelID  Channel ID
    *@param Rule        WPG Rule 
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    void AddRule(
        const WPG_Rule& Rule,
        HTREEITEM Item);

    /**
    *@brief Remove Rule To Doc(memory And XML), And Remove Tree Item, 
    *          if Set Trigger, Do Trigger Function
    *@param Rule        WPG Rule 
    *@param Item        Rule TreeCtrl Item HANDLE
    */
    void RemoveRule(
        HTREEITEM Item);

    /**
    *@brief Update Rule To Doc(memory And XML), 
    *       And Update Device ,if That channel Is Run IV
    *@param Rule        WPG Rule 
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Rule point is pass to GetRule
    */
    void UpdateRule(
        const WPG_Rule& Rule, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);

    /**
    *@brief Enable Rule To Doc(memory And XML)
    *       And Update Device ,if That channel Is Run IV
    *@param Item        Rule TreeCtrl Item HANDLE
    */
    void EnableRule(HTREEITEM Item, BOOL bEnable =TRUE);

    /**
    *@brief XX channel Run or Stop IV
    *@param nChannelID Channel ID
    *@param bEnable    Run or Stop
    */
    void Use(int nChannelID, bool bEnable);

    /**
    *@brief Test XX channel is Run or Stop IV
    *@param nChannelID Channel ID
    */
    bool IsUse(int nChannelID); 

    int GetShowState(int nChannelID);
    void SetShowState(int nChannelID, int nState);

    BOOL IsRuleEnbale(HTREEITEM Item);

    /**
    *@brief Enable All Rule To Doc(memory And XML)
    *       And Update Device, if That channel Is Run IV
    *@param Item        Rule TreeCtrl Item HANDLE
    */
    void EnableAllRule(int nChannelID, bool bEnable);

    /**
    *@brief Test XX channel is Have Free Rule(Max 5 Rule by every Channel)
    *@param nChannelID Channel ID
    */
    BOOL IsCanAddRule(
        int nChannelID,
        IVRuleType RuleType);


private: 
    template<OnRuleXXFn T>
    inline void DoTriggerTFun(
        int nChannelID,
        const char* pIdentityID,
        CString& strRuleName );
};

class CIVAlarmOutCfgDoc :
    protected CIVCfgDoc
{
protected:
    AlarmOutSettings* GetAlarmOut(
        HTREEITEM Item );

    /**
    *@brief Update AlarmOut To Doc(memory And XML), And Add Tree Item
    *@param Alarm       AlarmOutSettings
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Alarm point is pass to GetAlarmOut
    */
    void UpdateAlarmOut(
        const AlarmOutSettings& Alarm, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);

    void SetCfgToAll(const AlarmOutSettings& Alarm);
};

class CIVScheduleCfgDoc :
    protected CIVCfgDoc
{
protected:
    /**
    *@brief Get Schedule
    *@param Item        Channel TreeCtrl Item HANDLE
    */
    ScheduleSettings* GetSchedule(
        HTREEITEM Item );

    /**
    *@brief Update AlarmOut To Doc(memory And XML), And Add Tree Item
    *@param Sch         ScheduleSettings 
    *@param Item        Rule TreeCtrl Item HANDLE
    *@param IsRef       Sch point is pass to GetSchedule
    */
    void UpdateSchedule(
        const ScheduleSettings& Sch, 
        HTREEITEM Item,
        BOOL IsRef = TRUE);


    void SetCfgToAll(const ScheduleSettings& Sch);
};




#endif  // _IVCFGDOC_H_2010_


// End of file


