/*H***************************************************************************
 File            : IVCfgMgr.cpp
 Subsystem       : 
 Function Name(s): CIVCfgMgr
 Author          : YiQiu
 Date            : 2010-1-22  
 Time            : 17:36
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVCfgMgr.h"

#define _RootName       "IV"
#define _AutoRunChannel "AutoRun"

#define _ChannelName    "Ch"
#define _DataShowState  "ShowState"

#define _IdentityID     "ID"
#define _RuleName       "Name"
#define _RuleEnable     "Enable"
#define _RuleType       "Type"

#define _Rule           "Rule"
#define _Schudule       "Schudule"
#define _AlarmOut       "AlarmOut"


#define _IV_Config_File_Path  "C:\\IV_Settings.xml"

#define GetChannelName(i, szBuf) \
    char szBuf[32] = {0}; \
    sprintf(szBuf, "%s%d", _ChannelName, i);

CIVCfgMgr::CIVCfgMgr()
{
    bool bRc = m_Doc.LoadFile(_IV_Config_File_Path, TIXML_ENCODING_UTF8);
    if ( !bRc || 
         NULL == (m_pRootEle = m_Doc.RootElement()) )
    {
        m_pRootEle = new TiXmlElement(_RootName);
        m_Doc.LinkEndChild(m_pRootEle);

        assert( m_Doc.SaveFile() );
    }
}

CIVCfgMgr::~CIVCfgMgr()
{

}

IIVCfgMgr::IVVistor CIVCfgMgr::Begin( int nChannelID )
{
    assert(m_pRootEle);
    GetChannelName(nChannelID, szBuf);
    return IVVistor(m_pRootEle->FirstChildElement(szBuf));
}

const IIVCfgMgr::IVVistor& CIVCfgMgr::End()
{ 
    static const IVVistor v(NULL);
    return v; 
}

IIVCfgMgr::IVVistor CIVCfgMgr::Add(
    int nChannelID,
    const WPG_Rule& Rule,
    const ScheduleSettings& Sch,
    const AlarmOutSettings& Alarm )
{
    GetChannelName(nChannelID, szBuf);
    TiXmlElement* pChannelEle = TinyXmlUtil::CreateChildEle(
        &m_Doc, szBuf );


    TiXmlElement* pRuleEle = TinyXmlUtil::CreateChildEle(
        pChannelEle, _Rule);
    pRuleEle->SetAttribute(_RuleName, Rule.ruleName);
    TinyXmlUtil::SetAttributeData(pRuleEle, _IdentityID, Rule.ruleId, 16);

    TinyXmlUtil::SetChildElementTextData(pRuleEle, _Rule, Rule);
    TinyXmlUtil::SetChildElementTextData(pRuleEle, _Schudule, Sch);
    TinyXmlUtil::SetChildElementTextData(pRuleEle, _AlarmOut, Alarm);
    return IVVistor(pRuleEle);
}

bool CIVCfgMgr::Apply()
{
    return m_Doc.SaveFile();
}

bool CIVCfgMgr::Remove( 
    int nChannelID,
    const IVVistor& Vistor )
{
    assert(m_pRootEle);
    TiXmlElement* pEle = Ele(Vistor);
    if ( pEle == NULL )
    {
        return false;
    }

    GetChannelName(nChannelID, szBuf);
    TiXmlElement* pChannelEle = TinyXmlUtil::CreateChildEle(
        &m_Doc, szBuf );
    return pChannelEle->RemoveChild(pEle);
}

bool CIVCfgMgr::SetAutoRunChannel(
    const int szChannel[_MaxAutoChannel],
    size_t nCount )
{
    StringHelp::CMakeString<> strMake;
    for ( size_t i=0; i< min(nCount, _MaxAutoChannel); ++i )
        strMake << szChannel[i];
    m_pRootEle->SetAttribute(_AutoRunChannel, strMake.str());
    return true;
}

bool CIVCfgMgr::GetAutoRunChannel( 
    int szChannel[_MaxAutoChannel],
    size_t& nCount )
{
    const char* pTmpStr = m_pRootEle->Attribute(_AutoRunChannel);
    if ( pTmpStr == NULL )
    {
        nCount = 0;
        return true;
    }

    StringHelp::CParseString<> strParse(pTmpStr);
    nCount = min(strParse.Count(), _MaxAutoChannel);
    for ( size_t i=0; i< nCount; ++i )
    {
        strParse.At(i, szChannel[i]);
    }
    return true;
}

void CIVCfgMgr::SetDataShowState(int nChannelID, int nState)
{
    GetChannelName(nChannelID, szBuf);
    TiXmlElement* pChannelEle = TinyXmlUtil::CreateChildEle(
        &m_Doc, szBuf );
    pChannelEle->SetAttribute(_DataShowState, nState);
}

void CIVCfgMgr::GetDataShowState(int nChannelID,int& nState)
{
    GetChannelName(nChannelID, szBuf);
    TiXmlElement* pChannelEle = TinyXmlUtil::CreateChildEle(
        &m_Doc, szBuf );
    TinyXmlUtil::GetElementAttributeData(
        pChannelEle, _DataShowState, nState, IV_Show_All);
}

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
    TinyXmlUtil::GetElementAttributeData(
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
    return TinyXmlUtil::GetChildElementTextData(m_pEle, _Rule, Rule);
}

bool IIVCfgMgr::IVVistor::GetSchedule(ScheduleSettings& Sch)
{
    assert(m_pEle);
    return TinyXmlUtil::GetChildElementTextData(m_pEle, _Schudule, Sch);
}

bool IIVCfgMgr::IVVistor::GetAlarmOut(AlarmOutSettings& Alarm)
{
    assert(m_pEle);
    return TinyXmlUtil::GetChildElementTextData(m_pEle, _AlarmOut, Alarm);
}

// }
// Get

// Modify
// {
bool IIVCfgMgr::IVVistor::ModifyRule(const WPG_Rule& Rule)
{
    assert(m_pEle);
    m_pEle->SetAttribute(_RuleName, Rule.ruleName);
    m_pEle->SetAttribute(_RuleEnable, Rule.isEnabled);
    return TinyXmlUtil::SetChildElementTextData(m_pEle, _Rule, Rule);
}

bool IIVCfgMgr::IVVistor::ModifySchedule(const ScheduleSettings& Sch)
{
    assert(m_pEle);
    return TinyXmlUtil::SetChildElementTextData(m_pEle, _Schudule, Sch);
}

bool IIVCfgMgr::IVVistor::ModifyAlarmOut(const AlarmOutSettings& Alarm)
{
    assert(m_pEle);
    return TinyXmlUtil::SetChildElementTextData(m_pEle, _AlarmOut, Alarm);
}
// } 
// Modify

// }
// **********************  IVVistor *************************     
// 

namespace IIVCfgMgrFactory
{
    IIVCfgMgr* GetIIVCfgMgr()
    {
        return CIVCfgMgr::getInstancePtr();
    }
}


// End of file

