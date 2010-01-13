/********************************************************************
	created:	2010/01/09
	created:	9:1:2010   14:53
	filename: 	f:\tvt\src\DVR Server\DEV_HC_DM642i\CommonUtil.h
	file path:	f:\tvt\src\DVR Server\DEV_HC_DM642i
	file base:	CommonUtil
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/

/*H***************************************************************************
 File            : CommonUtil.h
 Subsystem       :  

Function Name(s): CCommonUtil
Author          : YiQiu
Date            : 2010-1-12  
Time            : 11:18
Description     : 

Revision        : 

History
-------


Copyright (c) xxxx Ltd.
***************************************************************************H*/


#ifndef COMMONUTIL_H_2010
#define COMMONUTIL_H_2010









#endif  // End of file

//

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
    bool resvr1:1;
    bool resvr2:1;
    bool resvr3:1;
};

// 现有智能的八种类型
enum IVRuleType
{
    // .. 省略
};

// 第三个为通道，第四个为时间戳，第五个为回调传过来的用户参数
typedef BOOL (*AlarmCallBack)(const AlarmOutTable&,IVRuleType,int,const FILETIME&,void* pParm);
struct IIVDeviceSetter
{
    // 设置Pre Alarm时间
    virtual void TellPreAlarmTime(int time)=0;

    // 当文件循环时调用，结果删除智能对应的数据文件
    virtual void DeleteIVFile(const char* pPath)=0;

    // 设置Alarm发生时的给上层的回调,pParm为用户输入的参数，回调会传出
    virtual void SetIVAlarmOutCallBack(AlarmCallBack AlarmCallBackFn, void* pParm)=0;
    
    // 设置智能数据发送的回调，由IVLiveFactory得到这个指针
    virtual void SetIVDataCallBack(const IIVDataSender* pIVDataSender)=0;
};

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

// 得到对应的对象指针
namespace DeviceFactory
{
    static IIVDeviceBase* GetIVDeviceBase(void);
    static IIVDeviceSetter* GeIVDeviceSetter(void);
    static IIVStatistic* GetIVStatistic(void);
};

struct IIVDataSender
{
    // 智能数据采集到后的回调
    virtual BOOL OnIVDataSend(int nChannelID, int Data)=0;
};

struct IIVDataSaver
{
    // 视频文件第一次写时回调，告诉智能这边文件保存的路径和开始的时间
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    // 视频文件关闭时，回调
    virtual BOOL Close(int nChannelID, int time)=0;
};

struct IIVViewer
{
    // 显示目标矩形框和路径时的回调，
    // 传入DC和正在播放的数据帧的时间
    virtual BOOL Paint(int nChannelID, const CDC& dc, const FILETIME& nTime)=0;
};

// 得到对应的对象指针
namespace IVLiveFactory
{
    static IIVViewer* GetViewer(void);
    static IIVDataSaver* GetDataSaver(void);
    static IIVDataSender* GetDataSender(void);
};

struct IVDataFound
{
    // 视频文件打开时回调，输入通道，视频文件路径，和时间戳
    virtual int Open(int nChannelID, const char* pPath, const FILETIME& time)=0;
    
    // 视频文件关闭时回调
    virtual int Close(int nChannelID, int time)=0;
};

// 得到对应的对象指针
namespace IVPlaybackFactory
{
    static IIVViewer* GetViewer(void);
    static IVDataFound* GetDataFound(void);
};