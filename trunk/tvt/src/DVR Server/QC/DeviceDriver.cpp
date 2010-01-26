/************************************************************************
* ��Ȩ���� (C)2004, ������ͬΪ��Ƶ�������޹�˾��
* 
* �ļ����ƣ� DeviceDriver.cpp
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
* �������ƣ� BOOL InitializeDriver()
* ���������� ��ʼ��Driver,���ɲ�׽�¼�
* ��������� ��
* ��������� ��
* �� �� ֵ���ɹ�����TRUE�����򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
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
* �������ƣ� void ExitDriver()
* ���������� �˳�Driver,�رղ�׽�¼�
* ��������� ��
* ��������� ��
* �� �� ֵ�� ��
* �޸�����    �汾��     �޸���	     �޸�����
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
* �������ƣ� BOOL OpenDriver(CString strDrvName)
* ���������� ��Driver
* ��������� strDrvName Driver����
* ��������� ��
* �� �� ֵ�� �ɹ�����TRUE�����򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
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
* �������ƣ� void CloseDriver()
* ���������� �ر�Driver
* ��������� ��
* ��������� ��
* �� �� ֵ�� �ɹ�����TRUE�����򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
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
* �������ƣ� BOOL ControlDriver(DWORD dwCtrlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize)
* ���������� ����Driver
* ��������� dwCtrlCode ���ƴ���
*			 lpInBuf	����Buf
*			 nInBufSize ����Buf����
* ��������� lpOutBuf	���Buf
*			 nOutBufSize���Buf����
* �� �� ֵ�� �ɹ�����TRUE�����򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
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
