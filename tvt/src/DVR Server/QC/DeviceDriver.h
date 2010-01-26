/************************************************************************
* 版权所有 (C)2004, 深圳市同为视频技术有限公司。
* 
* 文件名称： DeviceDriver.h
* 内容摘要： 控制卡设备(Driver)的基类，主要做一些不区别声音和视频
*			 也不区别具体卡类型的操作
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

#if !defined(AFX_DEVICEDRIVER_H__E023BB5A_09B9_43AE_9DC8_E5F0FBAB1731__INCLUDED_)
#define AFX_DEVICEDRIVER_H__E023BB5A_09B9_43AE_9DC8_E5F0FBAB1731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxmt.h>
#include <vfw.h>
#include <winioctl.h>
#include ".\DeviceControl\DeviceControl.h"
#include "IoCtlV.h"



class CDeviceDriver  
{
public:
	CDeviceDriver();
	virtual ~CDeviceDriver();
	virtual void CloseDriver();
	virtual BOOL OpenDriver(CString strDrvName);
	virtual void ExitDriver();
	virtual BOOL InitializeDriver();
	virtual BOOL ControlDriver(DWORD dwCtrlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize);
	virtual long GetDeviceNumber() = 0;
protected:
	virtual void UnlockDelBuf();
	virtual void LockDelBuf();
	CCriticalSection m_DelBufSec;
	CCriticalSection m_ControlDriverSec;
	HANDLE m_hDriver;
	HANDLE m_hCaptureEvent;
};

#endif // !defined(AFX_DEVICEDRIVER_H__E023BB5A_09B9_43AE_9DC8_E5F0FBAB1731__INCLUDED_)
