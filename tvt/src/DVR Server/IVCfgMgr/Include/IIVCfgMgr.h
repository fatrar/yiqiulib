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

#ifdef IVCFGMGR_EXPORTS
    #define IIVCFGMGR_CLASS CLASS_EXPORT
    #define IIVCFGMGR_API API_EXPORT
#else    
    #define IIVCFGMGR_CLASS CLASS_IMPORT
    #define IIVCFGMGR_API API_IMPORT
    #ifndef IIVCFGMGR_LINK
        #define IIVCFGMGR_LINK
        #pragma comment(lib, "IVCfgMgr.lib")
        #pragma message("Automatically linking with IVCfgMgr.dll") 
    #endif
#endif


struct IIVCfgMgr
{
    class IIVCFGMGR_CLASS IVVistor
    {
    public:
        explicit IVVistor(TiXmlElement* pEle = NULL);
        bool operator == (const IVVistor& v) const;
    public:
        IVVistor Next();

    public:
        // Get
        const char* GetIdentityID();
        const char* GetRuleName();
        bool IsAutoEnbale();
        IVRuleType GeRuleType();

        bool GetRule(WPG_Rule& Rule);
        bool GetSchedule(ScheduleSettings& Sch);
        bool GetAlarmOut(AlarmOutSettings& Alarm);

        // Modify
        bool ModifyRule(const WPG_Rule& Rule);   // Don`t modify type
        bool ModifySchedule(const ScheduleSettings& Sch);
        bool ModifyAlarmOut(const AlarmOutSettings& Alarm);

    private:
        TiXmlElement* m_pEle;
    };

    virtual IVVistor Begin(int nChannelID) = 0;
    virtual const IVVistor& End()=0;

    /**
    @purpose          : ���һ������
    @param nChannelID : ͨ����
    @param Rule       : �����趨�Ľṹ��
    @param Sch        : �����ų̵Ľṹ�壬Ĭ��ȫ����Ч
    @param Alarm      : ���������趨�Ľṹ��Ĭ��ȫ�У���hold 5��
    @return	IVVistor  : ������Ӻ�Ĺ�������ߡ���������End()�Ƚ��Ƿ�ɹ�
    */
    virtual IVVistor AddRule(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings)=0;

    /**
    @purpose : �����������ݵ�XML��ע�����е������޸ĺ�
               ֻ�е�������Żᱣ����Ч����Ȼֻ���޸��ڴ�
    @return	 : void
    */
    virtual bool Apply() = 0;
};


namespace IIVCfgMgrFactory
{
    IIVCFGMGR_API IIVCfgMgr* GetIIVCfgMgr();
}

#endif  // End of file



