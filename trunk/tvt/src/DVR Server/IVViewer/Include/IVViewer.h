/*H***************************************************************************
 File            : IVViewer.h
 Subsystem       : 
 Function Name(s): CIVViewer
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 10:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVVIEWER_H_2010_
#define _IVVIEWER_H_2010_

#include "IIVDataSender.h"


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
    virtual int Close(int nChannelID, const FILETIME& time)=0;
};

// 得到对应的对象指针
namespace IVPlaybackFactory
{
    static IIVViewer* GetViewer(void);
    static IVDataFound* GetDataFound(void);
};




#endif  // End of file



