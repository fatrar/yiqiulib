/*H***************************************************************************
 File            : IVViewer.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 10:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVVIEWER_H_2010_
#define _IIVVIEWER_H_2010_

#include <Windows.h>
#include <Common.h>
#include "..\DEVICEControl\IIVDataSender.h"

#ifdef IVVIEWER_EXPORTS
    #define IVVIEWER_CLASS CLASS_EXPORT
    #define IVVIEWER_API API_EXPORT
#else    
    #define IVVIEWER_CLASS CLASS_IMPORT
    #define IVVIEWER_API API_IMPORT
    #ifndef IVVIEWER_LINK
        #define IVVIEWER_LINK
        #pragma comment(lib, "IVViewer.lib")
        #pragma message("Automatically linking with IVViewer.dll") 
    #endif
#endif

/**
*@note IV数据保存接口
*/
struct IIVDataSaver
{
    /**
    *@note 视频文件第一次写时回调，告诉智能这边文件保存的路径和开始的时间
    */
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    /**
    *@note 视频文件关闭时，调用
    */
    virtual BOOL Close(int nChannelID, const FILETIME& time)=0;

    /**
    *@note 删除视频文件回调，从而通知智能这边删除智能文件。
    */
    virtual BOOL DeleteIVFile(const char* pPath)=0;

    /**
    *@note 设置预录像时间
    */
    virtual BOOL TellPreAlarmTime(int time)=0;
};

/**
*@note IV数据显示接口
*/
struct IIVViewer
{
    enum
    {
        Show_Object = 0x1,
        Show_Trace  = 0x2,
        Show_Object_Trace = Show_Object|Show_Trace,
    };

    /**
    *@note 显示目标矩形框和路径时的回调，
    *      传入DC和正在播放的数据帧的时间
    */
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time)=0;

    /**
    *@note bState=true显示目标和路径，否则隐藏
    */
    virtual void SetObjTraceShowState(
        int nChannelID, bool bShow)=0;
    
    /**
    *@note 得到目标和路径是否正在显示
    */
    virtual void GetObjTraceShowState(
        int nChannelID, bool& bShow)=0;
    
    /**
    *@note 设置显示哪些，是目标还是轨迹
    */
    virtual void SetDataShowState(
        int nChannelID, int nState)=0;
   
    /**
    *@note 得到显示的是目标还是轨迹
    */
    virtual void GetDataShowState(
        int nChannelID, int& nState)=0;
};

/**
*@note IV数据显示接口
*/
struct IIVLiveViewer :
    public IIVViewer
{
    /**
    *@note 重置统计
    */
    virtual BOOL ResetStatistic(int nChannelID)=0;

    /**
    *@note bFlag=true开始统计，flase停止
    */
    virtual BOOL StartStatistic(int nChannelID, bool bFlag)=0;

    /**
    *@note 得到统计状态
    */ 
    virtual BOOL GetStatisticState(
        int nChannelID, bool& bFlag)=0;

    /**
    *@note 显示规则图形
    */
    virtual void PaintRule(
        int nChannelID,
        const HDC dc,
        const RECT& rect)=0;

    /**
    *@note 显示规则统计
    */
    virtual void PaintStatistic(
        int nChannelID,
        const HDC dc,
        const RECT& rect)=0;

    /**
    *@note 设置是否显示规则图形和统计
    */
    virtual void ShowRuleAndStatistic(
        int nChannelID,
        bool bShow)=0;
};

/**
*@note IV数据显示接口,
* 这个跟上一个区别在于配置者在更改设置和初始化时使用
* IIVLiveViewer只是给上层提供接口用于显示
*/
struct IIVLiveViewerEx:
    public IIVLiveViewer
{
    /**
    *@note 添加规则给live显示者
    */
    virtual void AddRule(
        int nChannelID,
        const WPG_Rule& Rule)=0;

    /**
    *@note 删除规则
    */
    virtual void RemoveRule(
        int nChannelID,
        BYTE (&ID)[16])=0;

    /**
    *@note 更新规则
    */
    virtual void ModifyRule(
        int nChannelID,
        const WPG_Rule& Rule)=0;

    /**
    *@note 清空该通道的所有规则
    */
    virtual void ClearAllRule(int nChannelID)=0;
};

/**
*@note 得到Live各接口对应的对象指针
*/
namespace IVLiveFactory
{
    IVVIEWER_API IIVLiveViewer* GetLiveViewer();
    IVVIEWER_API IIVLiveViewerEx* GetLiveViewerEx();
    IVVIEWER_API IIVDataSaver* GetDataSaver();
    IVVIEWER_API IIVDataSender* GetDataSender();
};

/**
*@note IV数据回放接口
*/
struct IVDataFound
{
    /**
    *@note 视频文件打开时回调，输入通道，视频文件路径，和时间戳
    */
    virtual int Open(int nChannelID, const char* pPath, const FILETIME& time)=0;

    /**
    *@note 视频文件关闭时回调
    */
    virtual int Close(int nChannelID, const FILETIME& time)=0;
};

/**
*@note 得到PlayBack各接口对应的对象指针
*/ 
namespace IVPlaybackFactory
{
    static IIVViewer* GetPlaybackViewer(void);
    static IVDataFound* GetDataFound(void);
};




#endif  // End of file



