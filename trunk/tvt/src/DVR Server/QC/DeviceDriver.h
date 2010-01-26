/************************************************************************
* ��Ȩ���� (C)2004, ������ͬΪ��Ƶ�������޹�˾��
* 
* �ļ����ƣ� DeviceDriver.h
* ����ժҪ�� ���ƿ��豸(Driver)�Ļ��࣬��Ҫ��һЩ��������������Ƶ
*			 Ҳ��������忨���͵Ĳ���
* ��ǰ�汾�� 1.0
* ��    �ߣ� ��ʿ��
* ������ڣ� 
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
* �޸ļ�¼2����
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
