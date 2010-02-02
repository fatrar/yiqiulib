/*H***************************************************************************
 File            : IIVDevice.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 10:27
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVDEVICE_H_2010_
#define _IIVDEVICE_H_2010_


#ifdef IIVDEVICE_EXPORTS
    #define IIVDEVICE_API extern "C" __declspec(dllexport)
#else    
    #define IIVDEVICE_API extern "C" __declspec(dllimport)
#endif

#include "IIVDataSender.h"

struct IIVDeviceBase
{
    // ���ͨ���Ƿ���������
    virtual BOOL IsUse(int nChannelID)=0;

    // �����ͨ��ʹ������
    virtual BOOL Use(int nChannelID, bool bState)=0;

    // �Ƿ��п��е������豸
    virtual BOOL IsHaveFreeDevice(void)=0;
};

struct IIVStatistic
{
    // �Ƿ���ͳ�ƹ���,����ͨ���ţ�int nChannelID����ͬ��
    virtual BOOL IsHaveStatisticRule(int nChannelID)=0;

    // ����ͳ��
    virtual BOOL ResetStatistic(int nChannelID)=0;

    // bFlag=true��ʼͳ�ƣ�flaseֹͣ
    virtual BOOL StartStatistic(int nChannelID, bool bFlag)=0;

    // �õ�ͳ��״̬
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag)=0;
};

enum
{
    Max_Schedule_day = 24,
    Default_Hold_Time = 5,
};

// ʱ���ǰ�˳���ţ���С����
struct Scheduleday
{
    Scheduleday() : useNo(1)
    {
        starttime[0] = 0;
        endtime[0] = 24*60-1;
    }
    int  useNo;//�Ѿ�ʹ�õĸ��������ų̵ĸ���
    int  starttime[Max_Schedule_day];//�Է���Ϊ��λ��24Сʱ�ƿ�ʼʱ��
    int	 endtime[Max_Schedule_day];//�Է���Ϊ��λ��24Сʱ�ƽ���ʱ��
    //ÿһ���ų������±���ͬ����ʼ����ֹʱ������ʾ
};

struct ScheduleSettings
{
    Scheduleday s[7]; //��΢����š� ���������� Ȼ��1������
};

static const ScheduleSettings g_DefaultScheduleSettings;

// Alarm �ص�������ݣ�����bool�����Ӧ���Ƿ����
struct AlarmOutTable
{
    AlarmOutTable(){memset(this, 0xffffffff, sizeof(AlarmOutTable));}
    bool EMail:1;
    bool EMap:1;
    bool TelphoneCall:1;
    bool SnapShot:1;
    bool Sensor:1;
    bool Relay:1;
    bool Buzzer:1;
    bool FullScreen:1;
    bool PlaySound:1;
    bool AlarmRecord:1;
    bool resvr1:1;
    bool resvr2:1;
    bool resvr3:1;
    bool resvr4:1;
    bool resvr5:1;
    bool resvr6:1;
};

struct AlarmOutSettings
{
    AlarmOutSettings():nHoldTime(Default_Hold_Time){};
    int nHoldTime;
    AlarmOutTable table;
};

static const AlarmOutSettings g_DefaultAlarmOutSettings;

// �������ܵİ�������
enum IVRuleType
{
    IV_UnKnown = -1, 
    IV_Invade,              // ����
    IV_Leave_Disappear,     // ��ʧ/�뿪
    IV_LeftBehind,          // ����
    IV_Loiters,             // �ǻ�
    IV_Statistic,           // ����ͳ��
    IV_Vehicle_Retrograde,  // ��������
    IV_Illegal_Parking,     // �Ƿ�ͣ��
    IV_Stage_Change,        // �����任
};

enum IVDataShowState
{
    IV_Show_Object = 0x1,
    IV_Show_Trace  = 0x2,

    IV_Show_All = IV_Show_Object|IV_Show_Trace,
};


union IV_RuleID
{
    struct RULE_ID
    {
        static DWORD s_dwID;
        void Init(IVRuleType type)
        {
            ID = s_dwID++;
            nType = type;
            SYSTEMTIME syt;
            GetLocalTime(&syt);
            SystemTimeToFileTime(&syt, &SetTime);
        }

        DWORD ID;          // 0x00
        FILETIME SetTime;  // 0x04
        BYTE nType;        // 0xC
        BYTE resvr[3];     // 0xD
    } RuleID;

    unsigned char szRuleId[16];

    bool operator == (const IV_RuleID& a) const
    {
        return memcmp(szRuleId, a.szRuleId, sizeof(szRuleId))==0;
    }

    bool operator < (const IV_RuleID& a) const
    {
        return memcmp(szRuleId, a.szRuleId, sizeof(szRuleId))<0;
    }

    bool operator > (const IV_RuleID& a) const
    {
        return memcmp(szRuleId, a.szRuleId, sizeof(szRuleId))>0;
    }
};

DWORD IV_RuleID::RULE_ID::s_dwID = 0;

// ������Ϊͨ�������ĸ�Ϊʱ����������Ϊ�ص����������û�����
typedef BOOL (*AlarmCallBackFn)(const AlarmOutTable&,IVRuleType,int,const FILETIME*,void* pParm);

struct IIVDeviceSetter
{
    // ����Alarm����ʱ�ĸ��ϲ�Ļص�,pParmΪ�û�����Ĳ������ص��ᴫ��
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm)=0;

    // �����������ݷ��͵Ļص�����IVLiveFactory�õ����ָ��
    virtual void SetIVDataCallBack(IIVDataSender* pIVDataSender)=0;
};

struct ISnapShotSender
{
    virtual void OnSnapShotSend(
        int nChannelID, 
        DWORD dwRuleID,
        BYTE* pData,
        size_t nLen) = 0;
};

// �忨���ֻ�����������еĹ���
struct IIVDeviceBase2 :
    public IIVDeviceBase
{
    virtual BOOL Add(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings)=0;

    virtual BOOL Remove(
        int nChannelID,
        const IV_RuleID& RuleID)=0;

    virtual BOOL ModifyRule(
        int nChannelID,
        const WPG_Rule& Rule)=0;

    virtual BOOL ModifySchedule(
        int nChannelID,
        const IV_RuleID& RuleID,
        const ScheduleSettings& Sch )=0;

    virtual BOOL ModifyAlarmOut(
        int nChannelID,
        const IV_RuleID& RuleID,
        const AlarmOutSettings& Alarm )=0;
};


namespace DeviceFactory
{
    IIVDEVICE_API IIVDeviceBase* GetIVDeviceBase(void);
    IIVDEVICE_API IIVDeviceSetter* GeIVDeviceSetter(void);
    IIVDEVICE_API IIVStatistic* GetIVStatistic(void);
};




#endif  // End of file



