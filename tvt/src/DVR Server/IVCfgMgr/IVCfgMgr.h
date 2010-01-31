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
    public IIVCfgMgr ,
    public Singleton<CIVCfgMgr>
{
public:
    CIVCfgMgr();
    ~CIVCfgMgr();

    typedef IIVCfgMgr::IVVistor IVVistor;
public:
    virtual IVVistor Begin(int nChannelID);
    virtual const IVVistor& End();

    virtual IVVistor AddRule(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings);

    virtual bool Apply();

private:
    TiXmlDocument m_Doc;
    TiXmlElement* m_pRootEle;
};






#endif  // End of file



