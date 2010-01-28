/*H***************************************************************************
 File            : IVCfgMgr.h
 Subsystem       : 
 Function Name(s): CIVCfgMgr
 Author          : YiQiu
 Date            : 2010-1-22  
 Time            : 17:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVCFGMGR_H_2010_
#define _IVCFGMGR_H_2010_

#include ".\Include\IIVCfgMgr.h"

class CIVCfgMgr :
    public IIVCfgMgr
{
public:
    CIVCfgMgr();
    ~CIVCfgMgr();

public:
    virtual IIVCfgMgr::IVVistor Begin(int nChannelID);
    virtual const IIVCfgMgr::IVVistor& End();

    virtual bool AddIVRule(int nChannelID, const WPG_Rule& Rule);
    virtual bool AddIVSchedule(int nChannelID, const ScheduleSettings& Sch);
    virtual bool AddIVSAlarmOut(int nChannelID, const AlarmOutTable& table);

    virtual bool ModifyIVRule(int nChannelID, const WPG_Rule& Rule) = 0;
    virtual bool ModifyIVSchedule(int nChannelID, const ScheduleSettings& Sch);
    virtual bool ModifyIVSAlarmOut(int nChannelID, const AlarmOutTable& table);

private:
    TiXmlDocument m_Doc;     
};






#endif  // End of file



