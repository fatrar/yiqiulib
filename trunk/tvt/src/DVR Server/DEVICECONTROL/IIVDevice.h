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

//#include "DeviceControl.h"
#include "IIVDataSender.h"

struct IIVDeviceBase
{
    // 这个通道是否在用智能
    virtual BOOL IsUse(int nChannelID)=0;

    // 对这个通道使用智能
    virtual BOOL Use(int nChannelID, bool bState)=0;

    // 是否还有空闲的智能设备
    virtual BOOL IsHaveFreeDevice(void)=0;
};

struct IIVStatistic
{
    // 是否有统计规则,输入通道号（int nChannelID，下同）
    virtual BOOL IsHaveStatisticRule(int nChannelID)=0;

    // 重置统计
    virtual BOOL ResetStatistic(int nChannelID)=0;

    // bFlag=true开始统计，flase停止
    virtual BOOL StartStatistic(int nChannelID, bool bFlag)=0;

    // 得到统计状态
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag)=0;
};

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

    Max_Channel = 4,

    Max_SnapShot_Pic_Size = 128 * 1024,  // 一张JPG图的最大大小
    Max_SnapShot_Pic_Count = 6,         // 一次最多几张图
    //#define	VIDEO_BUFFER_MAX_FRAME_LENGTH		IDEO_BUFFER_MAX_FRAME_LENGTH		(64 * 1024)
};

// 时间是按顺序排，从小到大
struct Scheduleday
{
    Scheduleday() : useNo(1)
    {
        ZeroMemory(&starttime, sizeof(starttime));
        ZeroMemory(&endtime, sizeof(endtime));
        starttime[0] = 0;
        endtime[0] = 24*60-1;     
    }
    int  useNo;//已经使用的个数，即排程的个数
    unsigned short starttime[Max_Schedule_day];//以分钟为单位的24小时制开始时间
    unsigned short endtime[Max_Schedule_day];//以分钟为单位的24小时制结束时间
    //每一个排程项用下标相同的起始和终止时间来表示
};

struct ScheduleSettings
{
    Scheduleday s[7]; //按微软的排。 先是星期天 然后1，。。
};

static const ScheduleSettings g_DefaultScheduleSettings;

// Alarm 回调输出数据，下面bool代表对应项是否输出
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

// 现有智能的八种类型
enum IVRuleType
{
    IV_UnKnown = -1, 
    IV_Invade,              // 入侵
    IV_Leave_Disappear,     // 消失/离开
    IV_LeftBehind,          // 遗留
    IV_Loiters,             // 徘徊
    IV_Statistic,           // 流量统计
    IV_Vehicle_Retrograde,  // 车辆逆行
    IV_Illegal_Parking,     // 非法停车
    IV_Stage_Change,        // 场景变换
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
        void Init(IVRuleType type)
        {
            static DWORD s_dwID = 0;
            ID = s_dwID++;
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
};

//DWORD IV_RuleID::RULE_ID::s_dwID = 0;

// 第三个为通道，第四个为时间戳，第五个为回调传过来的用户参数
typedef BOOL (*AlarmCallBackFn)(const AlarmOutTable&,IVRuleType,int,const FILETIME*,void* pParm);

struct IIVAlarmCallBack
{
    virtual void OnAlarmCallBack(
        const AlarmOutTable& table,
        IVRuleType type,
        int nChannelID,
        const FILETIME* pTime)=0;
};

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
    // 设置Alarm发生时的给上层的回调,pParm为用户输入的参数，回调会传出
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm)=0;

    // 设置智能数据发送的回调，由IVLiveFactory得到这个指针
    virtual void SetIVDataCallBack(IIVDataSender* pIVDataSender)=0;

    virtual void SetSnapShotCallBack(ISnapShotSender* pSnapShotSender)=0;
};


// 这个结构体在DeviceControl有定义，但是为了不跟他粘在一起，
// 单独定义，并用宏防止重定义。包含DeviceControl必须应引用很多其他头文件，
// 关键里面还使用的MFC的类，导致包含DeviceControl必须是MFC的工程
#ifndef _Define_FrameBufStruct_2010
#define _Define_FrameBufStruct_2010
typedef struct _FrameBufStruct
{
    long		ChannelIndex;//总的通道号
    DWORD		BufferPara;//删除Buffer参数,如果是视频包括卡Index和BufIndex，如果是声音高16表示是否为PC自带声音
    BYTE     	*pBuf;//此帧数据的Buffer地址
    long		BufLen;//此帧数据的Buffer长度
    ULONGLONG	localTime;// 采集时的绝对时间单位为100nm
    ULONGLONG	FrameTime;//采集的相对时间
    DWORD      	nStreamID;//流类型
    long      	nVLostFlag;//信号状态
    long		bIsKeyFrame;//当数据为压缩数据时，是否为关键帧
    int		width;	//<41XD1-17>
    int		height;	//<41XD1-17>
} FRAMEBUFSTRUCT;
#endif

struct IVideoSend
{
    virtual BOOL OnVideoSend(FRAMEBUFSTRUCT *bufStruct) = 0;
};

struct IIVSimulationAlarmCallBack
{
    virtual void OnAlarmCallBack(
        IVRuleType type,
        int nChannelID,
        const FILETIME* pTime)=0;
};

struct IIVSimulation
{
    virtual void Start(
        int nChannelID, 
        IIVSimulationAlarmCallBack* p,
        const WPG_Rule& Rule) = 0;
    virtual void Stop(int nChannelID)=0;
};

// 板卡这边只关心正在运行的规则
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
};


namespace DeviceFactory
{
#ifdef IIVDEVICE_EXPORTS
    IIVDEVICE_API IIVDeviceBase* GetIVDeviceBase(void);
    IIVDEVICE_API IIVDeviceBase2* GetIVDeviceBase2(void);
    IIVDEVICE_API IIVDeviceSetter* GeIVDeviceSetter(void);
    IIVDEVICE_API IIVStatistic* GetIVStatistic(void);
#else
    // 下面设计只是为调用者调用方便且尽量不依赖我这边的改动
    // 从而做到，我这边接口的变动，只需要调用者重新编译即可，而不需要改动代码
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
        "GetIVStatistic",
    };

    typedef IIVDeviceSetter* (*GetIVDeviceBaseFn)();
    typedef IIVDeviceBase2* (*GetIVDeviceBase2Fn)();
    typedef IIVDeviceSetter* (*GeIVDeviceSetterFn)();
    typedef IIVStatistic* (*GetIVStatisticFn)();
#endif
    
};




#endif  // End of file



