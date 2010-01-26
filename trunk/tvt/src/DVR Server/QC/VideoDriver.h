/************************************************************************
* 版权所有 (C)2004, 深圳市同为视频技术有限公司。
* 
* 文件名称： VideoDriver.h
* 内容摘要： 控制卡设备(Driver)视频部分的基类，主要做一些不区别具体卡类型的视频操作
* 当前版本： 1.0
* 作    者： 陈士光
* 完成日期： 
* 
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#if !defined(AFX_VIDEODRIVER_H__311CCE19_FD9E_4736_9C1F_77E20E1B9840__INCLUDED_)
#define AFX_VIDEODRIVER_H__311CCE19_FD9E_4736_9C1F_77E20E1B9840__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DeviceDriver.h"

#define TD3016_SUB_TYPE_6800 0
#define TD3016_SUB_TYPE_6805 1
#define TD3008_SUB_TYPE_6800 2 //by chenlong
#define TD3008_SUB_TYPE_6805 3 //by chenlong

class CVideoDriver : public CDeviceDriver
{
public:
	int m_iTD30xxSubType;
	virtual int Check_6800_6805_Channel(int index);
    virtual void GetDefaultOrder(DWORD &ChNum,DWORD pChannelIndex[]) {};
	virtual BOOL SetChannelOrder(DWORD ChannelCount,DWORD *pChannelIndex);
    virtual BOOL SetChannelOrder3016(int nindex, DWORD ChannelCount, DWORD *pChannelIndex);
	virtual BOOL SetResetComputerTime(long SecTime);
	virtual BOOL ResetComputer(DWORD dwStart);
	virtual BOOL Password();
	virtual BOOL GetVideoPropertyValue(long ChIndex, VIDEO_PROPER &videoProper);
	virtual BOOL SetVideoPropertyValue(long ChIndex,VIDEO_PROPER videoProper);
	virtual BOOL GetSignalStatus(DWORD *pSignalStatus,long statusNum);
	virtual BOOL CallMonitor(long VideoIndex);
    virtual BOOL SetAlarmStatus(DWORD *pAlarmStatus,long statusNum){return FALSE;};
    virtual BOOL GetAlarmStatus(DWORD *pAlarmStatus,long statusNum){return FALSE; };
	virtual long GetDeviceNumber();
	virtual BOOL ReleaseBuffer(DWORD bufParam);
	virtual BOOL StopCapture();
	virtual BOOL StartCapture(DWORD sizeType );
	virtual void ExitDriver();
	virtual BOOL InitializeDriver(long DeviceIndex,DWORD VideoFormat,CAPTURECALLBACK *pVideoCallBack);
	CVideoDriver();
	virtual ~CVideoDriver();
	
protected:
	DWORD	m_dwChannelSwitch[30];
	DWORD	m_dwChannelNum;
	virtual BOOL Is6800() = 0;
	virtual BOOL SetC51AddressValue(long lAddress, long lValue);
	virtual BOOL SetC51BlockValue(long lAddress, ULONG *lValue, long nLen);
	CCriticalSection m_GPIOSecIn;//不能同时操作GPIO,包括报警输入输出、AutoReset等
	CAPTURECALLBACK	*m_pVideoCallBack;
	VIDEO_PROPER m_VideoProperty[30];
	DWORD	m_dwVideoFormat;
	long	m_nDeviceNum;
	long	m_nDeviceIndex;
	DWORD	m_dwFrameIndex;
	DWORD	m_dwFrameIndexRel;
	HANDLE	m_hVideoCaptureThread;//视频捕捉线程
	CCriticalSection m_GPIOSec;//不能同时操作GPIO,包括报警输入输出、AutoReset等
	BOOL	m_bVideoCapturing;//
	LPBYTE	m_pBufFlagAddr;
	virtual BOOL	GPIOWrite( long lBeg, long lEnd, long lVal );
	virtual BOOL    GPIORead( long lBeg, long lEnd, DWORD &lVal );
	virtual long	GetChannelIndex(long CardIndex, long ChIndexOfCard)=0;
private:
	BOOL GetOneFrameData();
	DWORD VideoCaptureFunc();
	static DWORD WINAPI VideoCaptureThread(LPVOID lpParameter);
};

#endif // !defined(AFX_VIDEODRIVER_H__311CCE19_FD9E_4736_9C1F_77E20E1B9840__INCLUDED_)
