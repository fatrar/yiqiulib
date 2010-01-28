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


#define _ChannelName "Ch"


CIVCfgMgr::CIVCfgMgr()
{

}

CIVCfgMgr::~CIVCfgMgr()
{

}

IIVCfgMgr::IVVistor CIVCfgMgr::Begin( int nChannelID )
{
    char szBuf[32] = {0};
    sprintf(szBuf, "%s%d", _ChannelName, nChannelID);
    return IVVistor(m_Doc.FirstChildElement(szBuf));
}

IIVCfgMgr::IVVistor CIVCfgMgr::End(){ return IVVistor(NULL); }

bool CIVCfgMgr::AddIVRule( int nChannelID, const WPG_Rule& Rule )
{
    return true;
}

bool CIVCfgMgr::AddIVSchedule( int nChannelID, const ScheduleSettings& Sch )
{
    return true;
}

bool CIVCfgMgr::AddIVSAlarmOut( int nChannelID, const AlarmOutTable& table )
{
    return true;
}

bool CIVCfgMgr::ModifyIVRule( int nChannelID, const WPG_Rule& Rule )
{
    return true;
}

bool CIVCfgMgr::ModifyIVSchedule( int nChannelID, const ScheduleSettings& Sch )
{
    return true;
}

bool CIVCfgMgr::ModifyIVSAlarmOut( int nChannelID, const AlarmOutTable& table )
{
    return true;
}

IIVCfgMgr::IVVistor::IVVistor() : m_pEle(NULL) {}
IIVCfgMgr::IVVistor::IVVistor(TiXmlElement* pEle): m_pEle(pEle) {}


bool IIVCfgMgr::IVVistor::operator == (const IIVCfgMgr::IVVistor& v) const
{
    return this->m_pEle == v.m_pEle;
}

IIVCfgMgr::IVVistor IIVCfgMgr::IVVistor::Next()
{
    return IVVistor( m_pEle->NextSiblingElement() );
}

bool IIVCfgMgr::IVVistor::GetIVRule( int nChannelID, WPG_Rule& Rule )
{
    return true;
}

bool IIVCfgMgr::IVVistor::GetIVSchedule( int nChannelID, ScheduleSettings& Sch )
{
    return true;
}

bool IIVCfgMgr::IVVistor::GetIVSAlarmOut( int nChannelID, AlarmOutTable& table )
{
    return true;
}







// End of file

