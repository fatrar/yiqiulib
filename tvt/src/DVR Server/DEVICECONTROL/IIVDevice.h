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


struct IIVDeviceBase
{
    // 这个通道是否在用智能
    virtual BOOL IsUse(int nChannelID)=0;

    // 对这个通道使用智能
    virtual BOOL Use(int nChannelID, bool bState)=0;

    // bState=true显示目标和路径，否则隐藏
    virtual BOOL ShowObjTrace(bool bState)=0;

    // 得到目标和路径是否正在显示
    virtual BOOL GetObjTraceState(bool& bState)=0;

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


// Alarm 回调输出数据，下面bool代表对应项是否输出
struct AlarmOutTable
{
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
    bool resvr2:1;
    bool resvr3:1;
};

// 现有智能的八种类型
enum IVRuleType
{
    IV_Invade,              // 入侵
    IV_Leave_Disappear,     // 消失/离开
    IV_LeftBehind,          // 遗留
    IV_Loiters,             // 徘徊
    IV_Statistic,           // 流量统计
    IV_Vehicle_Retrograde,  // 车辆逆行
    IV_Illegal_Parking,     // 非法停车
    IV_Stage_Change,        // 场景变换
};

// 第三个为通道，第四个为时间戳，第五个为回调传过来的用户参数
typedef BOOL (*AlarmCallBackFn)(const AlarmOutTable&,IVRuleType,int,const FILETIME&,void* pParm);

struct IIVDeviceSetter
{
    // 设置Alarm发生时的给上层的回调,pParm为用户输入的参数，回调会传出
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm)=0;

    // 设置智能数据发送的回调，由IVLiveFactory得到这个指针
    virtual void SetIVDataCallBack(const IIVDataSender* pIVDataSender)=0;
};


namespace DeviceFactory
{
    IIVDEVICE_API IIVDeviceBase* GetIVDeviceBase(void);
    IIVDEVICE_API IIVDeviceSetter* GeIVDeviceSetter(void);
    IIVDEVICE_API IIVStatistic* GetIVStatistic(void);
};




#endif  // End of file



