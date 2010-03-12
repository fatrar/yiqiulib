/**CPP*************************************************************************
 File            : IVVistor.cpp
 Subsystem       : 
 Function Name(s): CIVVistor
 Author          : YiQiu
 Date            : 2010-3-10  
 Time            : 17:40
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVCfgMgr.h"



//
// **********************  IVVistor *************************     
// {
IIVCfgMgr::IVVistor::IVVistor(TiXmlElement* pEle): m_pEle(pEle) {}


bool IIVCfgMgr::IVVistor::operator == (const IIVCfgMgr::IVVistor& v) const
{
    return this->m_pEle == v.m_pEle;
}

bool IIVCfgMgr::IVVistor::operator != (const IIVCfgMgr::IVVistor& v) const
{
    return this->m_pEle != v.m_pEle;
}

IIVCfgMgr::IVVistor IIVCfgMgr::IVVistor::Next()
{
    assert(m_pEle);
    return IVVistor( m_pEle->NextSiblingElement() );
}

// Get
// {
const char* IIVCfgMgr::IVVistor::GetIdentityID()
{
    assert(m_pEle);
    return m_pEle->Attribute(_IdentityID);
}

const char* IIVCfgMgr::IVVistor::GetRuleName()
{
    assert(m_pEle);
    return m_pEle->Attribute(_RuleName);
}

bool IIVCfgMgr::IVVistor::IsAutoEnbale()
{
    assert(m_pEle);
    bool bEnbale = false;
    TinyXmlUtil::GetAttributeData(
        m_pEle, _RuleEnable, bEnbale, false);
    return bEnbale;
}

IVRuleType IIVCfgMgr::IVVistor::GeRuleType()
{
    assert(m_pEle);
    int nRuleType = 0;
    m_pEle->Attribute(_RuleType, &nRuleType); 
    return (IVRuleType)nRuleType;
}

bool IIVCfgMgr::IVVistor::GetRule(WPG_Rule& Rule)
{
    assert(m_pEle);
    return _GetRule(m_pEle, Rule);
}

bool IIVCfgMgr::IVVistor::GetSchedule(ScheduleSettings& Sch)
{
    assert(m_pEle);
    return _GetSchedule(m_pEle, Sch);
}

bool IIVCfgMgr::IVVistor::GetAlarmOut(AlarmOutSettings& Alarm)
{
    assert(m_pEle);
    return TinyXmlUtil::GetChildBinaryTextData(m_pEle, _AlarmOut, Alarm);
}

// }
// Get

// Modify
// {
bool IIVCfgMgr::IVVistor::ModifyRule(const WPG_Rule& Rule)
{
    assert(m_pEle);
    return _ModifyRule(m_pEle, Rule);
}

bool IIVCfgMgr::IVVistor::ModifySchedule(const ScheduleSettings& Sch)
{
    assert(m_pEle);
    return _ModifySchedule(m_pEle, Sch);
}

bool IIVCfgMgr::IVVistor::ModifyAlarmOut(const AlarmOutSettings& Alarm)
{
    assert(m_pEle);
    return TinyXmlUtil::SetChildBinaryTextData(m_pEle, _AlarmOut, Alarm);
}
// } 
// Modify











// End of file