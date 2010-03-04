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


struct IRuleTrigger
{
    virtual void OnRuleRemove(int nChannelID, const char* pIdentityID)=0;
    virtual void onRuleAdd(int nChannelID, const char* pIdentityID)=0;
};

/**
*@brief  
*/
class CIVCfgDoc
{
public:
    CIVCfgDoc(void);
    ~CIVCfgDoc(void);

    static void Init();

    /**
    *@brief Register Rule Add Or Remove Trigger
    *@param	  
    */
    static void RegisterRuleTrigger(IRuleTrigger* pRuleTrigger);  

protected:
    struct CurrentRuleSetting
    {
        CurrentRuleSetting(
            const WPG_Rule& _Rule,
            const ScheduleSettings& _Sch,
            const AlarmOutSettings& _Alarm)
            : Rule(_Rule)
            , Sch(_Sch)
            , Alarm(_Alarm) {}

        WPG_Rule Rule;
        ScheduleSettings Sch;
        AlarmOutSettings Alarm;
    };

    typedef map<IV_RuleID, CurrentRuleSetting*> RuleSettingMap;
    typedef map<int, RuleSettingMap> AllRuleSettingMap;
private:
    static AllRuleSettingMap m_Doc;
    static deque<IRuleTrigger*> m_RuleTrigger;
};








#endif  // _IVCFGDOC_H_2010_


// End of file


