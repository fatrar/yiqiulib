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
#ifndef _IIVCFGMGR_H_2010_
#define _IIVCFGMGR_H_2010_


#include "..\DEVICEControl\WPG_EventOccurrence.h"
#include "..\DEVICEControl\IIVDevice.h"

class TiXmlElement;

struct IIVCfgMgr
{
    class IVVistor
    {
    public:
        IVVistor();
        IVVistor(TiXmlElement* pEle);
    public:
        bool operator == (const IVVistor& v) const;
    public:
        IVVistor Next();

        bool GetIVRule(int nChannelID, WPG_Rule& Rule);
        bool GetIVSchedule(int nChannelID, ScheduleSettings& Sch);
        bool GetIVSAlarmOut(int nChannelID, AlarmOutTable& table);
    private:
        TiXmlElement* m_pEle;
    };


    virtual IVVistor Begin(int nChannelID) = 0;
    virtual const IIVCfgMgr::IVVistor& End();

    virtual bool AddIVRule(int nChannelID, const WPG_Rule& Rule) = 0;
    virtual bool AddIVSchedule(int nChannelID, const ScheduleSettings& Sch) = 0;
    virtual bool AddIVSAlarmOut(int nChannelID, const AlarmOutTable& table) = 0;

    virtual bool ModifyIVRule(int nChannelID, const WPG_Rule& Rule) = 0;
    virtual bool ModifyIVSchedule(int nChannelID, const ScheduleSettings& Sch) = 0;
    virtual bool ModifyIVSAlarmOut(int nChannelID, const AlarmOutTable& table) = 0;
};


#endif  // End of file



