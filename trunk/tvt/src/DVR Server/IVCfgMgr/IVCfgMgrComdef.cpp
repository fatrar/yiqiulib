/**CPP*************************************************************************
 File            : IVCfgMgrComdef.cpp
 Subsystem       : 
 Function Name(s): CIVCfgMgrComdef
 Author          : YiQiu
 Date            : 2010-3-10  
 Time            : 18:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVCfgMgrComdef.h"


template<>
void MyRule<WPG_TripwireEventDescription>::Init(
    const WPG_Rule& Rule)
{
    description = Rule.ruleDescription.description.tripwireEventDescription;
}

template<>
void MyRule<MyAOIEventDes>::Init(
    const WPG_Rule& Rule)
{
    const WPG_AOIEventDescription& aoiDes = Rule.ruleDescription.description.aoiEventDescription;
    
    /**
    *@note  1. Init actionType
    */
    const WPG_AOI_ACTION_TYPE& actionType = aoiDes.actionType;
    description.enters           = actionType.enters != 0;
    description.exits            = actionType.exits!= 0;				
    description.inside           = actionType.inside!= 0;			
    description.appears          = actionType.appears!= 0;			
    description.disappears       = actionType.disappears!= 0;		
    description.takenAway        = actionType.takenAway!= 0;			
    description.loitersEnable    = actionType.loiters.loitersEnable!= 0;	
    description.leftBehindEnable = actionType.leftBehind.leftBehindEnable!= 0;
    description.loitersTime      = actionType.loiters.duration;
    description.leftBehindTime   = actionType.leftBehind.duration;
    
    /**
    *@note  2. Init other
    */
    description.viewType = aoiDes.viewType;
    description.planeType = aoiDes.planeType;
    description.polygon = aoiDes.polygon;
}

template<>
void MyRule<WPG_SceneChangeEventDescription>::Init(
    const WPG_Rule& Rule)
{
    description = Rule.ruleDescription.description.sceneChangeEventDescription;
}



template<typename T>
void MyRule<T>::ToWPGRule(WPG_Rule& Rule )
{
    Rule.maxSizeChangeFilter.maxSizeRatio= maxSizeRatio;
    Rule.maxSizeChangeFilter.useFilter   = useSizeChangeFilter;
    Rule.useShapeDirectionFilter         = useShapeDirectionFilter;

    Rule.minimumFilter.useFilter     =  useMinFilter;
    Rule.minimumFilter.nearRectangle = nearRectangle[0];
    Rule.minimumFilter.farRectangle  = farRectangle[0];

    Rule.maximumFilter.useFilter     = useMaxFilter;
    Rule.maximumFilter.nearRectangle = nearRectangle[1];
    Rule.maximumFilter.farRectangle  = farRectangle[1];
}


template<>
WPG_Rule& operator<< <WPG_TripwireEventDescription>(
    WPG_Rule& WPGRule, 
    MyRule<WPG_TripwireEventDescription>& MyRuleT)
{
    MyRuleT.ToWPGRule(WPGRule);
    WPGRule.ruleDescription.description.tripwireEventDescription 
        = MyRuleT.description;
    return WPGRule;
}

template<>
WPG_Rule& operator<< <MyAOIEventDes>(
    WPG_Rule& WPGRule, 
    MyRule<MyAOIEventDes>& MyRuleT)
{
    /**
    *@note  1. Init WPGRule Common
    */
    MyRuleT.ToWPGRule(WPGRule);

    WPG_AOIEventDescription& aoiDes = WPGRule.ruleDescription.description.aoiEventDescription;

    /**
    *@note  2. Init actionType
    */
    WPG_AOI_ACTION_TYPE& actionType = aoiDes.actionType;
    MyAOIEventDes& My = MyRuleT.description;
    actionType.enters                     = My.enters;
    actionType.exits		   	          = My.exits;				
    actionType.inside			          = My.inside;			
    actionType.appears			          = My.appears;			
    actionType.disappears		          = My.disappears;		
    actionType.takenAway			      = My.takenAway;			
    actionType.loiters.loitersEnable	  = My.loitersEnable;	
    actionType.leftBehind.leftBehindEnable= My.leftBehindEnable;
    actionType.loiters.duration           = My.loitersTime;
    actionType.leftBehind.duration        = My.leftBehindTime;

    /**
    *@note  3. Init other
    */
    aoiDes.viewType = My.viewType;
    aoiDes.planeType = My.planeType;
    aoiDes.polygon = My.polygon;
    return WPGRule;
}

template<>
WPG_Rule& operator<< <WPG_SceneChangeEventDescription>(
    WPG_Rule& WPGRule, 
    MyRule<WPG_SceneChangeEventDescription>& MyRuleT)
{
    MyRuleT.ToWPGRule(WPGRule);
    WPGRule.ruleDescription.description.sceneChangeEventDescription 
        = MyRuleT.description;
    return WPGRule;
}



bool _AddRule( 
    TiXmlElement* pRuleEle,
    const WPG_Rule& Rule )
{
    WPG_EVENT_TYPE type = Rule.ruleDescription.type;
    if ( type == TRIPWIRE_EVENT )
    {
        MyRule<WPG_TripwireEventDescription> MyRuleTmp(Rule);
        TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp);
    }
    else if ( type == AOI_EVENT )
    {
        MyRule<MyAOIEventDes> MyRuleTmp(Rule);
        TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp);
    }
    else if ( type == SCENE_CHANGE_EVENT )
    {
        MyRule<WPG_SceneChangeEventDescription> MyRuleTmp(Rule);
        TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp);
    }
    else 
    {
        assert(false);
        return false;
    }

    TinyXmlUtil::SetBinaryAttributeData(pRuleEle, _IdentityID, Rule.ruleId, 16);
    pRuleEle->SetAttribute(_RuleName, Rule.ruleName);
    pRuleEle->SetAttribute(_RuleEnable, (int)Rule.isEnabled);
    pRuleEle->SetAttribute(_RuleType, (int)type);
        
    //TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _Rule, Rule);
    return true;
}

bool _AddSchedule(
    TiXmlElement* pRuleEle, 
    const ScheduleSettings& Sch )
{
    TiXmlElement* pSchEle = TinyXmlUtil::CreateChildEle(pRuleEle, _Schudule);
    TiXmlElement* pSchWeekEle = NULL;

    Scheduleday SchBuf;
    static const size_t nHead = sizeof(Sch.s[0].useNo);
    static const size_t nIter = sizeof(Sch.s[0].starttime[0]);
    for ( int i = 0; i< Week_Count; ++i )
    {
        const Scheduleday& TmpSch = Sch.s[i];
        pSchWeekEle = TinyXmlUtil::CreateChildEle(pSchEle, g_szWeek[i]);
        if ( TmpSch.useNo == Max_Schedule_day )
        {
            TinyXmlUtil::SetBinaryTextData(pSchWeekEle, TmpSch);
            continue;
        }

        /**
        *@note 1. fill Buffer
        */
        SchBuf.useNo = TmpSch.useNo;
        BYTE* pTmp = (BYTE*)&SchBuf + nHead;
        size_t nOffset = nIter*TmpSch.useNo;
        memcpy(pTmp, (void*)&TmpSch.starttime, nOffset );
        pTmp += nOffset;
        memcpy(pTmp, (void*)&TmpSch.endtime, nOffset );

        /**
        *@note 2. Set to XML
        */
        TinyXmlUtil::SetBinaryTextData(pSchWeekEle, &SchBuf, nHead+2*nIter*TmpSch.useNo);
    }
    
    return true;
}

bool _GetRule( 
    TiXmlElement* pRuleEle,
    WPG_Rule& Rule )
{
    /*
    *@note 1. Get Rule Base Info(common Info) 
    */
    size_t nIDLen = 16;
    TinyXmlUtil::GetBinaryAttributeData(pRuleEle, _IdentityID, Rule.ruleId, nIDLen);
    TinyXmlUtil::GetAttributeData(pRuleEle, _RuleName, Rule.ruleName, "Error");
    TinyXmlUtil::GetAttributeData(pRuleEle, _RuleEnable, (bool&)Rule.isEnabled);
    TinyXmlUtil::GetAttributeData(
        pRuleEle, _RuleType, (int&)Rule.ruleDescription.type, INVALID_EVENT_TYPE);

    /*
    *@note 2. Get Rule Other Info 
    */
    WPG_EVENT_TYPE type = Rule.ruleDescription.type;
    if ( type == TRIPWIRE_EVENT )
    {
        MyRule<WPG_TripwireEventDescription> MyRuleTmp;
        assert(TinyXmlUtil::GetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp));
        Rule << MyRuleTmp;
    }
    else if ( type == AOI_EVENT )
    {
        MyRule<MyAOIEventDes> MyRuleTmp(Rule);
        assert(TinyXmlUtil::GetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp));
        Rule << MyRuleTmp;
    }
    else if ( type == SCENE_CHANGE_EVENT )
    {
        MyRule<WPG_SceneChangeEventDescription> MyRuleTmp(Rule);
        assert(TinyXmlUtil::GetChildBinaryTextData(pRuleEle, _Rule, MyRuleTmp));
        Rule << MyRuleTmp;
    }
    else 
    {
        assert(false);
        return false;
    }
    return true;
}

bool _GetSchedule( 
    TiXmlElement* pRuleEle,
    ScheduleSettings& Sch )
{
    TiXmlElement* pSchEle = TinyXmlUtil::CreateChildEle(pRuleEle, _Schudule);
    TiXmlElement* pSchWeekEle = NULL;

    //Scheduleday SchBuf;
    static const size_t nHead = sizeof(Sch.s[0].useNo);
    static const size_t nIter = sizeof(Sch.s[0].starttime[0]);
    for ( int i = 0; i< Week_Count; ++i )
    {
        Scheduleday& TmpSch = Sch.s[i];

        /**
        *@note 1. Get Date to XML
        */
        pSchWeekEle = TinyXmlUtil::CreateChildEle(pSchEle, g_szWeek[i]);
        TinyXmlUtil::GetBinaryTextData(pSchWeekEle, TmpSch);

        /**
        *@note 2. Adjust Data
        */
        int nNum = TmpSch.useNo;
        if ( nNum == Max_Schedule_day )
        {
            continue;
        }

        for ( int i = 0, j = Max_Schedule_day-1;
              i<nNum; ++i, --j)
        {
            TmpSch.endtime[i] = TmpSch.starttime[nNum+i];
        }
        ZeroMemory(&TmpSch.starttime[nNum], nIter*(Max_Schedule_day-nNum));
    }

    return true;
}

// End of file