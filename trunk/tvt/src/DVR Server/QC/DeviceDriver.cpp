/************************************************************************
* 版权所有 (C)2004, 深圳市同为视频技术有限公司。
* 
* 文件名称： DeviceDriver.cpp
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

#include "stdafx.h"
#include "DeviceDriver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceDriver::CDeviceDriver()
{
	m_hCaptureEvent = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;
}

CDeviceDriver::~CDeviceDriver()
{

}


/******************************************************************************
* 函数名称： BOOL InitializeDriver()
* 功能描述： 初始化Driver,生成捕捉事件
* 输入参数： 无
* 输出参数： 无
* 返 回 值：成功返回TRUE，否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceDriver::InitializeDriver()
{
	m_hCaptureEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(NULL == m_hCaptureEvent)
	{
		return FALSE;
	}

	return TRUE;
}


/******************************************************************************
* 函数名称： void ExitDriver()
* 功能描述： 退出Driver,关闭捕捉事件
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

void CDeviceDriver::ExitDriver()
{
	if(m_hCaptureEvent)
	{
		CloseHandle(m_hCaptureEvent);
	}
	m_hCaptureEvent= NULL;
}


/******************************************************************************
* 函数名称： BOOL OpenDriver(CString strDrvName)
* 功能描述： 打开Driver
* 输入参数： strDrvName Driver名字
* 输出参数： 无
* 返 回 值： 成功返回TRUE，否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceDriver::OpenDriver(CString strDrvName)
{
	m_hDriver = CreateFile(strDrvName, 
					GENERIC_READ  | GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					0,
					NULL );
	if (INVALID_HANDLE_VALUE == m_hDriver)
	{
		DWORD err = GetLastError();
		CString cstr;
		cstr.Format("CreateFile%d",err);
		AfxMessageBox(cstr);
		return FALSE;
	}

	return TRUE;
}


/******************************************************************************
* 函数名称： void CloseDriver()
* 功能描述： 关闭Driver
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回TRUE，否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

void CDeviceDriver::CloseDriver()
{
	if(INVALID_HANDLE_VALUE != m_hDriver)
	{
		CloseHandle(m_hDriver);
	}
	m_hDriver=INVALID_HANDLE_VALUE;
}


/******************************************************************************
* 函数名称： BOOL ControlDriver(DWORD dwCtrlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize)
* 功能描述： 控制Driver
* 输入参数： dwCtrlCode 控制代码
*			 lpInBuf	输入Buf
*			 nInBufSize 输入Buf长度
* 输出参数： lpOutBuf	输出Buf
*			 nOutBufSize输出Buf长度
* 返 回 值： 成功返回TRUE，否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceDriver::ControlDriver(DWORD dwCtrlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize)
{
	if (INVALID_HANDLE_VALUE == m_hDriver)
		return FALSE;
	DWORD retVal;

	m_ControlDriverSec.Lock();

	BOOL ret = DeviceIoControl(m_hDriver,
		dwCtrlCode,
		lpInBuf,
		nInBufSize,
		lpOutBuf,
		nOutBufSize,
		&retVal,
		NULL);

	m_ControlDriverSec.Unlock();

	return ret;
}

void CDeviceDriver::LockDelBuf()
{
	m_DelBufSec.Lock();
}

void CDeviceDriver::UnlockDelBuf()
{
	m_DelBufSec.Unlock();
}
