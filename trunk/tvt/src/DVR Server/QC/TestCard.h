// TestCard.h: interface for the CTestCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTCARD_H__664E6FA2_EB9C_4442_A60F_8B796FB2C43C__INCLUDED_)
#define AFX_TESTCARD_H__664E6FA2_EB9C_4442_A60F_8B796FB2C43C__INCLUDED_

#include "Define.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CSetupDi.h"
#include <afxtempl.h>

#include "vfw.h"
#include ".\devicecontrol\devicecontrol.h"
#include "AlarmDefine.h"


typedef CList<CString, CString> CardVerList;

//class CDeviceManager;

class CTestCard  
{
public:
	CString m_ExePath;
	BOOL m_bThisPCSInstallDriver;
    DWORD *m_pSignalStatus;  //保存通道信号状态

	CAPTURECALLBACK *m_pVideoCallBack;
	void ChangeVideoFormat();
	void SetVideoFormat(DWORD m_VideoFormat);
	void GetRestartAndInstallDriver(BOOL *needRestart,BOOL *needInstallDriver);
	BOOL NeedResetComputer();
	void InitialTestInfor();
	BOOL AlarmDeviceFree();
	BOOL PciDeviceFree();

	BOOL FreeDevice();
	BOOL m_bPciInitial;
	BOOL m_bUsbInitial;
	BOOL m_bAlarmInitial;
	//Alarm Card Var
	QuitCard MyQuitCard;
	AlarmIn MyAlarmIn;
	AlarmOut MyAlarmOut;
	IniCard MyIniCard;
	//djx 2007/12/07
	CommOut MyCommOut;
	GetCardVer MyGetCardVer;

	HINSTANCE m_hAlarmCard;

	BOOL SetAlarmOut(DWORD *status);
	BOOL GetAlarmIn(DWORD *status);
	void InitialDevNum();
	void ResetComputer();

	void VideoCaptureStop(void);

	BOOL m_bVideoCaptureStart;
	BOOL VideoCaptureStart(DWORD dwBmpSizeType,BITMAPINFOHEADER RecBmpHeader);
	BOOL m_bInitialed;
	BOOL Password();
	void InitialVar();
	DWORD m_dwVideoDeviceNum;
	void DeviceFreeVideo();
	CString m_strVideoDriverName;
	void GetVideoDriverName();
	CVideoDriver *m_pVideoDriver;
	DWORD m_dwVideoFormat;
	BOOL InitialVideoDevice(DWORD VideoFormat, CAPTURECALLBACK *pVideoCallBack);

	BOOL InstallDriver();
	CStringArray m_DisplayName;
	CStringArray m_CardId;
	CDeviceInfoList m_DeviceControl;
	BOOL TestAlarmCardInitial();

	BOOL TestPciCardInitial();
	BOOL CheckUsbCard();
	BOOL CheckPciDvrCard();
	BOOL CheckAlarmCard();
	BOOL InitialDevice( BOOL *bPci,BOOL *bUsb ,BOOL *bAlarm,CAPTURECALLBACK *pVideoCallBack ,HWND hwnd );
	CARD_INFOR GetInstallCardInfor();
    BOOL GetInvalidateFlag(int deviceNum); //by chenlong 
    BOOL GetSignalStatus(int deviceNum);   //by chenlong
	CTestCard();
	virtual ~CTestCard();
private:
	CARD_INFOR m_card_infor;
};

#endif // !defined(AFX_TESTCARD_H__664E6FA2_EB9C_4442_A60F_8B796FB2C43C__INCLUDED_)
