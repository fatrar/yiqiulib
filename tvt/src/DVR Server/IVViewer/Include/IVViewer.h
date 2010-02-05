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
#include "..\..\Base\Base\Include\Common.h"
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

struct IIVDataSaver
{
    // 视频文件第一次写时回调，告诉智能这边文件保存的路径和开始的时间
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    // 视频文件关闭时，回调
    virtual BOOL Close(int nChannelID, const FILETIME& time)=0;

    // 删除视频文件回调，从而通知智能这边删除智能文件。
    virtual BOOL DeleteIVFile(const char* pPath)=0;

    virtual BOOL TellPreAlarmTime(int time)=0;
};
       
struct IIVViewer
{
    // 显示目标矩形框和路径时的回调，
    // 传入DC和正在播放的数据帧的时间
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time)=0;

    // bState=true显示目标和路径，否则隐藏
    virtual BOOL ShowObjTrace(int nChannelID, bool bState)=0;

    // 得到目标和路径是否正在显示
    virtual BOOL GetObjTraceState(int nChannelID, bool& bState)=0;

    // 设置显示哪些，是目标还是轨迹
    virtual void SetDataShowState(int nChannelID, int nState)=0;
};

// 得到对应的对象指针
//namespace IVLiveFactory
//{
//    IVVIEWER_API /*static */IIVViewer* GetViewer(void);
//    IVVIEWER_API /*static*/ IIVDataSaver* GetDataSaver(void);
//    IVVIEWER_API /*static*/ IIVDataSender* GetDataSender(void);
//};
namespace IVLiveFactory
{
    IVVIEWER_API IIVViewer* GetLiveViewer();
    IVVIEWER_API IIVDataSaver* GetDataSaver();
    IVVIEWER_API IIVDataSender* GetDataSender();
};

struct IVDataFound
{
    // 视频文件打开时回调，输入通道，视频文件路径，和时间戳
    virtual int Open(int nChannelID, const char* pPath, const FILETIME& time)=0;

    // 视频文件关闭时回调
    virtual int Close(int nChannelID, const FILETIME& time)=0;
};

// 得到对应的对象指针
namespace IVPlaybackFactory
{
    static IIVViewer* GetPlaybackViewer(void);
    static IVDataFound* GetDataFound(void);
};




#endif  // End of file



