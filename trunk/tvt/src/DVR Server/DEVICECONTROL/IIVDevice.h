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
#pragma once
#pragma pack(push, 1)

#ifdef IIVDEVICE_EXPORTS
    #define IIVDEVICE_API extern "C" __declspec(dllexport)
#else    
    #define IIVDEVICE_API extern "C" __declspec(dllimport)
#endif

//#include "DeviceControl.h"
#include "IIVDataSender.h"

/**
*@note ����SDK�������ܵĵ��ýӿ�
*/
struct IIVDeviceBase
{
    /**
    *@note ���ͨ���Ƿ���������
    */
    virtual BOOL IsUse(int nChannelID)=0;

    /**
    *@note �����ͨ��ʹ������
    */ 
    virtual BOOL Use(int nChannelID, bool bState)=0;

    /**
    *@note �Ƿ��п��е������豸
    */ 
    virtual BOOL IsHaveFreeDevice(void)=0;

    /**
    *@note �Ƿ���ͳ�ƹ���,����ͨ���ţ�int nChannelID����ͬ��
    */ 
    virtual BOOL IsHaveStatisticRule(int nChannelID)=0;
};

//struct IIVStatistic
//{
//   
//};

enum
{
    Max_Schedule_day = 24,
    Default_Hold_Time = 5,

    Default_Loiters_duration = 10,
    Default_LeftBehind_duration = 10,
    Loiters_Max_Time = 600,     // 10min
    LeftBehind_Max_Time = 600,  // 10min

    Max_Rule_Name = 20,

    AlarmOutTable_Count = 10,

    Max_SnapShot_Pic_Size = 128 * 1024,  // һ��JPGͼ������С
    Max_SnapShot_Pic_Count = 8,         // һ����༸��ͼ
    //#define	VIDEO_BUFFER_MAX_FRAME_LENGTH		IDEO_BUFFER_MAX_FRAME_LENGTH		(64 * 1024)
};

// ʱ���ǰ�˳���ţ���С����
struct Scheduleday
{
    Scheduleday() : useNo(1)
    {
        ZeroMemory(&starttime, sizeof(starttime));
        ZeroMemory(&endtime, sizeof(endtime));
        starttime[0] = 0;
        endtime[0] = 24*60-1;     
    }
    int  useNo;//�Ѿ�ʹ�õĸ��������ų̵ĸ���
    unsigned short starttime[Max_Schedule_day];//�Է���Ϊ��λ��24Сʱ�ƿ�ʼʱ��
    unsigned short endtime[Max_Schedule_day];//�Է���Ϊ��λ��24Сʱ�ƽ���ʱ��
    //ÿһ���ų������±���ͬ����ʼ����ֹʱ������ʾ
};

struct ScheduleSettings
{
    Scheduleday s[7]; //��΢����š� ���������� Ȼ��1������
};

static const ScheduleSettings g_DefaultScheduleSettings;

// Alarm �ص�������ݣ�����bool�����Ӧ���Ƿ����
union AlarmOutTable
{
    AlarmOutTable():nTable(0xffff){}
    struct {
        bool AlarmRecord:1;
        bool FullScreen:1;
        bool Relay:1;
        bool Sensor:1;
        bool EMail:1;
        bool EMap:1;
        bool Buzzer:1; 
        bool SnapShot:1;
        bool PlaySound:1;
        bool TelphoneCall:1;   
        bool resvr1:1;
        bool resvr2:1;
        bool resvr3:1;
        bool resvr4:1;
        bool resvr5:1;
        bool resvr6:1;
    } Table;
    unsigned short nTable;
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
    IV_RuleID( const unsigned char (&p)[16] )
    {
        memcpy(szRuleId, p, 16);
    }
    IV_RuleID(){}
    struct RULE_ID
    {
        void Init(IVRuleType type)
        {
            static DWORD s_dwID = 0;
            ID = s_dwID++;
            if ( type == IV_Statistic )
            {
                s_dwID++;
            }
            nType = type;
            SYSTEMTIME syt;
            GetLocalTime(&syt);
            SystemTimeToFileTime(&syt, &SetTime);
        }

        DWORD ID:4;          // 0x00
        IVRuleType nType:4;  // 0x04
        FILETIME SetTime;    // 0x08
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

    void operator=(const unsigned char (&p)[16])
    {
        memcpy(szRuleId, p, 16);
    }

    /**
    *@note �ж�����RuleID�Ƿ���ͳ�ƣ����Ƿ���ͳ�Ʒֳ�������������֮һ
    *@return 0 or 1 �����Ƿֳ�������ĵ�һ���͵ڶ�����-1 ������ͬһ������
    *@remarks Ҫ����ͳ�ƵĹ���������
    * ����a��ͳ�ƹ�������Ҫ�ж�b�Ƿ�a���������ײ��ID��
    * int nRc = a ^ b;
    */
    int operator^(const unsigned char (&p)[16])
    {
        const IV_RuleID& left = (const IV_RuleID&)p;
        if ( this->RuleID.nType != IV_Statistic ||
             this->RuleID.nType != left.RuleID.nType ||
             memcmp(&this->RuleID.SetTime, &left.RuleID.SetTime, sizeof(FILETIME))!=0 )
        {
            return -1;
        }

        if ( this->RuleID.ID == left.RuleID.ID )
        {
            return 0;
        }
        if ( this->RuleID.ID == left.RuleID.ID + 1 )
        {
            return 1;
        }

        return -1;
    }
};

//DWORD IV_RuleID::RULE_ID::s_dwID = 0;

// ������Ϊͨ�������ĸ�Ϊʱ����������Ϊ�ص����������û�����
typedef BOOL (*AlarmCallBackFn)(const AlarmOutTable&,IVRuleType,int,const FILETIME*,void* pParm);

/**
*@note �����ص��ӿ�
*/
struct IIVAlarmCallBack
{
    virtual void OnAlarmCallBack(
        const AlarmOutTable& table,
        IVRuleType type,
        int nChannelID,
        const FILETIME* pTime)=0;
};

/**
*@note ������ͼ�ص��ӿ�
*/
struct ISnapShotSender
{
    virtual void OnSnapShotSend(
        int nChannelID, 
        DWORD dwRuleID,
        FILETIME* pTime,
        BYTE* pData,
        size_t nLen) = 0;
};

struct IIVDeviceSetter
{
    /**
    *@note ����Alarm����ʱ�ĸ��ϲ�Ļص�,pParmΪ�û�����Ĳ������ص��ᴫ��
    */
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm)=0;

    /**
    *@note �����������ݷ��͵Ļص�����IVLiveFactory�õ����ָ��
    */
    virtual void SetIVDataCallBack(
        IIVDataSender* pIVDataSender,
        IIVStatisticFresher* pIVStatisticFresher)=0;

    /**
    *@note ���ñ�����ͼ�ص�
    */
    virtual void SetSnapShotCallBack(
        ISnapShotSender* pSnapShotSender)=0;
};


// ����ṹ����DeviceControl�ж��壬����Ϊ�˲�����ճ��һ��
// �������壬���ú��ֹ�ض��塣����DeviceControl����Ӧ���úܶ�����ͷ�ļ���
// �ؼ����滹ʹ�õ�MFC���࣬���°���DeviceControl������MFC�Ĺ���
#ifndef _Define_FrameBufStruct_2010
#define _Define_FrameBufStruct_2010
typedef struct _FrameBufStruct
{
    long		ChannelIndex;//�ܵ�ͨ����
    DWORD		BufferPara;//ɾ��Buffer����,�������Ƶ������Index��BufIndex�������������16��ʾ�Ƿ�ΪPC�Դ�����
    BYTE     	*pBuf;//��֡���ݵ�Buffer��ַ
    long		BufLen;//��֡���ݵ�Buffer����
    ULONGLONG	localTime;// �ɼ�ʱ�ľ���ʱ�䵥λΪ100nm
    ULONGLONG	FrameTime;//�ɼ������ʱ��
    DWORD      	nStreamID;//������
    long      	nVLostFlag;//�ź�״̬
    long		bIsKeyFrame;//������Ϊѹ������ʱ���Ƿ�Ϊ�ؼ�֡
    int		width;	//<41XD1-17>
    int		height;	//<41XD1-17>
} FRAMEBUFSTRUCT;
#endif

struct IVideoSend
{
    virtual BOOL OnVideoSend(FRAMEBUFSTRUCT *bufStruct) = 0;
};

struct IIVSimulationAlarmCallBack :
    public IIVStatisticFresher
{
    virtual void OnAlarmCallBack(
        IVRuleType type,
        int nChannelID,
        const FILETIME* pTime)=0;

    ///**
    //*@note i=0  dir A, i=1 dir B
    //*/
    //virtual void OnStatisticAdd(int i)=0;
};

struct IIVSimulation
{
    virtual BOOL StartSimulation(
        int nChannelID, 
        IIVSimulationAlarmCallBack* p,
        const WPG_Rule& Rule) = 0;
    virtual BOOL StopSimulation(int nChannelID)=0;
};

// �忨���ֻ�����������еĹ���
struct IIVDeviceBase2 :
    public IIVDeviceBase,
    public IIVSimulation
{
    virtual BOOL Add(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings)=0;

    virtual BOOL Remove(
        int nChannelID,
        const IV_RuleID& RuleID)=0;

    virtual BOOL EnableRule(
        int nChannelID,
        const IV_RuleID& RuleID,
        BOOL bEnable =TRUE)=0;

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

    virtual void RegisterLiveDataCallBack(
        int nChannelID,
        IVideoSend* pVideoSend) =0;

    virtual void UnRegisterLiveDataCallBack(
        int nChannelID, 
        IVideoSend* pVideoSend)=0;

    virtual void ReleaseLiveBuf(FRAMEBUFSTRUCT* p)=0;

    virtual void GetDeviceInfo(
        int* pnDeviceNum,
        int* pnChannelNumByDevice )=0;

    virtual void GetIVDeviceInfo(
        int* pnIVChannelNumByDevice,
        int* pnMaxRuleNumByIVChannel )=0;
};


namespace DeviceFactory
{
#ifdef IIVDEVICE_EXPORTS
    IIVDEVICE_API IIVDeviceBase* GetIVDeviceBase(void);
    IIVDEVICE_API IIVDeviceBase2* GetIVDeviceBase2(void);
    IIVDEVICE_API IIVDeviceSetter* GeIVDeviceSetter(void);
    //IIVDEVICE_API IIVStatistic* GetIVStatistic(void);
#else
    // �������ֻ��Ϊ�����ߵ��÷����Ҿ�������������ߵĸĶ�
    // �Ӷ�����������߽ӿڵı䶯��ֻ��Ҫ���������±��뼴�ɣ�������Ҫ�Ķ�����
    enum
    {
        GetIVDeviceBase_Index = 0,
        GetIVDeviceBase2_Index = 1,
        GeIVDeviceSetter_Index = 2,
        GetIVStatistic_Index = 3,
    };

    static const char* g_szIVDeviceFuncName[] = 
    {
        "GetIVDeviceBase",
        "GetIVDeviceBase2",
        "GeIVDeviceSetter",
        //"GetIVStatistic",
    };

    typedef IIVDeviceSetter* (*GetIVDeviceBaseFn)();
    typedef IIVDeviceBase2* (*GetIVDeviceBase2Fn)();
    typedef IIVDeviceSetter* (*GeIVDeviceSetterFn)();
    //typedef IIVStatistic* (*GetIVStatisticFn)();
#endif
    
};


#pragma pack(pop)

#endif  // End of file



