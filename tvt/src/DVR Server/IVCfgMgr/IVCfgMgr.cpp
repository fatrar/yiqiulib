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




CIVCfgMgr::CIVCfgMgr()
{
    bool bRc = m_Doc.LoadFile(_IV_Config_File_Path);
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
    return IVVistor( 
        TinyXmlUtil::CreateChildChildEle(
        m_pRootEle, 2, szBuf, _Rule) );
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
        m_pRootEle, szBuf );

    TiXmlElement* pRuleEle = TinyXmlUtil::CreateChildEle(
        pChannelEle, _Rule);
    if ( !_AddRule(pRuleEle, Rule) )
    {
        return End();
    }
    
    assert(_AddSchedule(pRuleEle, Sch));
    //TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _Schudule, Sch);
    TinyXmlUtil::SetChildBinaryTextData(pRuleEle, _AlarmOut, Alarm);
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
        m_pRootEle, szBuf );
    return pChannelEle->RemoveChild(pEle);
}

bool CIVCfgMgr::SetAutoRunChannel(
    const int szChannel[_MaxAutoChannel],
    size_t nCount )
{
    StringHelp::CMakeString<> strMake;
    for ( size_t i=0; i< min(nCount, _MaxAutoChannel); ++i )
        strMake << szChannel[i];
    m_pRootEle->SetAttribute(_AutoRunChannel, strMake.str().c_str());
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
        m_pRootEle, szBuf );
    pChannelEle->SetAttribute(_DataShowState, nState);
}

void CIVCfgMgr::GetDataShowState(int nChannelID,int& nState)
{
    GetChannelName(nChannelID, szBuf);
    TiXmlElement* pChannelEle = TinyXmlUtil::CreateChildEle(
        m_pRootEle, szBuf );
    TinyXmlUtil::GetAttributeData(
        pChannelEle, _DataShowState, nState, IV_Show_All);
}




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

