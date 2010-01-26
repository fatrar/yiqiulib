// TestCard.cpp: implementation of the CTestCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QC.h"
#include "TestCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const DWORD g_dwSupportSizeType=DVRVIDEO_SIZE_320x240|DVRVIDEO_SIZE_640x480;

int _video_channel_number[SUPPORT_CARD_NUMBER] =
{
		4,
		8,
		16,
		16,
		16,
		16,
		16,
		0,
		16,
		0,
		4,
		4,
		8,
		4,
		16,
		4,	//djx 2008/1/5 3004_6805
		8,	//djx 2008/7/22 4108
		8,	//zld 2009/4/22 3308
		4,	//zld 2009/4/22 3304
		8,	//zld 2009/4/22 4408
		16,	//zld 2009/6/22 3016_6805
		8,	//zld 2009/6/22 3008_6805
        4,  //chenlong 09/08/29
        8,   //chenlong 09/08/29
        4
};

CString _card_id[SUPPORT_CARD_NUMBER] =
{
		ID3004,
		ID3008,
		ID3016,
		ID3116V1,
		ID3116V2,
		ID3216V2,
		ID3216V3,
		TDALARMNULL,
		ID3316,
		TDNULL,
		IDUSB_1,
		IDUSB_2,
		ID3108,
		ID4104,
		ID4116,
		ID3004V2,//djx 2008/1/5 3004_6805
		ID4108,	//djx 2008/7/22 4108
		ID3308,  //zld 2009/4/22 3308
		ID3304, //zld 2009/4/22 3304
		ID4408,  //zld 2009/4/22 4408
		ID3016V2,  //zld 2009/6/22 3016V2
		ID3008V2,  //zld 2009/6/23 3008V2
        ID3304S,  //chenlong 09/08/29
        ID3308S,   //chenlong 09/08/29
        ID4104i
};

CString _card_chip_set[5] =
{
		TW6800_VIDEO //3004,3008,3016
		BT878_VIDEO //3116(1.0/2.0)3216 (2.0/3.0)
		BT878_AUDIO 
		ALOG7416	
		ALOG480	
};

CString _display_name[SUPPORT_CARD_NUMBER] =
{
		("3004"),
		("3008"),
		("3016"),
		("3116V1"),
		("3116V2"),
		("3216V2"),
		("3216V3"),
		("Alarm Card"),
		("3316"),
		("NULL"),
		("USB"),
		("USB"),
		("TD3108"),
		("TD4104"),
		("TD4116"),
		("3004V2"),//djx 2008/1/5 3004_6805
		("TD4108"), //djx 2008/7/22 4108
		("TD3308"), //zld 2009/4/22 3308
		("TD3304"), //zld 2009/4/22 3304
		("TD4408"), //zld 2009/4/22 4408
		("TD3016V2"), //zld 2009/6/22 3016_6805
		("TD3008V2"), //zld 2009/6/23 3008_6805
        ("TD3304S"), //chenlong 2009/08/29
        ("TD3308S"), //chenlong 2009/08/29
        ("TD4104i")
};

CString _LaodLibrary_name[SUPPORT_CARD_NUMBER] = 
{
    (""),//3004
    (""),//3008
    (""),//3016
    (""),//3116V1
    (""),//3116V2
    (""),//3216V2
    (""),//3216V3
    (""),//Alarm Card
    ("DEV_SC_AL16.dll"),//3316
    (""),//NULL
    ("DEV_SC_CXA.dll"),//USB
    ("DEV_SC_CXA.dll"),//USB
    (""),//TD3108
    ("DEV_HC_DM642.dll"),//TD4104
    ("DEV_HC_DM642_16.dll"),//TD4116
    (""),//3004V2
    ("DEV_HC_DM642_8.dll"),//TD4108
    ("DEV_SC_MV08.dll"),//TD3308
    ("DEV_SC_MV04.dll"),//TD3304
    (""),//TD4408
    (""),//TD3016V2
    (""),//TD3008V2
    ("DEV_SC_MV04.dll"),//TD3304S
    ("DEV_SC_MV08.dll"), //TD3308S
    ("DEV_HC_DM642i.dll")//TD4104
};  //匹配加载的dll名 by chenlong

BOOL _test_table[SUPPORT_CARD_NUMBER][TEST_ITEM_NUMBER] =
{																							   //djx云台 音频
    //  0 ,      1,     2,       3,     4,      5,     6,       7,      8,      9,      10,	    11,     12
        TRUE,	 TRUE,	TRUE,    TRUE,  TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   FALSE,	FALSE,	FALSE, //3004;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,	FALSE,	FALSE, //3008;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,	FALSE,	FALSE, //3016;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,	FALSE,	FALSE, //3116V1;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,	FALSE,	FALSE, //3116V2;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  TRUE,   TRUE,   TRUE,   FALSE,	FALSE,	FALSE, //3216V2;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  TRUE,   TRUE,   TRUE,   FALSE,	FALSE,	FALSE, //3216V3;
        FALSE,   FALSE, FALSE,   FALSE, FALSE,  TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,	FALSE,	FALSE, //ALARM_CARD
        TRUE,	 TRUE,  TRUE,    TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  TRUE,   TRUE,   FALSE,	FALSE,	TRUE,  //3316;
        TRUE,	 TRUE,	TRUE,	 FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, //NULL
        TRUE,	 TRUE,  TRUE,    TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE, 	FALSE,  TRUE,  //USB_1
        TRUE,	 TRUE,  TRUE,    TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,	FALSE,  TRUE,  //USB_2
        TRUE,	 TRUE,  TRUE,    TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,	FALSE,  FALSE, //3108
        TRUE,	 TRUE,	TRUE,	 TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE,  //4104
        TRUE,	 TRUE,	TRUE,	 TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE,  //4116
        TRUE,	 TRUE,	TRUE,    TRUE,  TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   FALSE,	FALSE,	FALSE,  //3004V2;//djx 2008/1/5 3004_6805
        TRUE,	 TRUE,	TRUE,    TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE,  ////djx 2008/7/22 4108	
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   FALSE,  TRUE,   TRUE,   TRUE,   FALSE,	FALSE,	FALSE,  //zld 2009/4/22 3308	
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   FALSE,  TRUE,   TRUE,   FALSE,  FALSE,	FALSE,	FALSE,  //zld 2009/4/22 3304	
        TRUE,	 TRUE,	FALSE,	 TRUE,	FALSE,	FALSE,	FALSE,  FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE, ////zld 2009/4/22 4408
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,	FALSE,	FALSE, //zld 2009/6/22 3016V2;
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,	FALSE,	FALSE, //zld 2009/6/22 3008V2
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,	FALSE,	FALSE,  //chenlong 09/8/29 3304S
        TRUE,	 TRUE,  TRUE,	 TRUE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,	FALSE,	FALSE,   //chenlong 09/8/29 3308S
        TRUE,	 TRUE,	TRUE,	 TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE  //4104i
};

CTestCard::CTestCard()
{
	m_pVideoDriver = NULL;
	/*初始化m_pSignalStatus*/
    m_pSignalStatus = new DWORD[16];
    for (int i = 0; i < 16; i++)
    {
        m_pSignalStatus[i] = 0;
    }
}

CTestCard::~CTestCard()
{
    /*释放m_pSignalStatus*/
    if (m_pSignalStatus != NULL)
    {
        delete []m_pSignalStatus;
        m_pSignalStatus = NULL;
    }
    ::FreeLibrary(m_hAlarmCard);
}

_CARD_INFOR CTestCard::GetInstallCardInfor()
{
	return m_card_infor;
}

BOOL CTestCard::TestPciCardInitial()
{
	m_bPciInitial=CheckPciDvrCard();
	return m_bPciInitial;
}

BOOL  CTestCard::TestAlarmCardInitial()
{
	m_bAlarmInitial = CheckAlarmCard();
	return m_bAlarmInitial;
}

BOOL CTestCard::InitialDevice(BOOL *bPci, BOOL *bUsb, BOOL *bAlarm, 
							  CAPTURECALLBACK *pVideoCallBack, HWND hwnd)
{
	m_pVideoCallBack = pVideoCallBack;

	int devnum = 0;

	InitialVar();

	*bPci=TestPciCardInitial();
	//	*bUsb=FALSE;
	*bAlarm=TestAlarmCardInitial();
	
	if (*bPci) 
		devnum++;

	if (*bUsb)
		devnum++;

	if (*bAlarm) 
		devnum++;

	if (devnum>1)
	{
		AfxMessageBox("一次只能测试一个设备,请关机取出一块继续测试!");
		FreeDevice();
		exit(1);
	}

	if ((devnum == 0)&&(m_card_infor.card_type_id != DVR_CARD_TDNULL))
	{
		AfxMessageBox("没有发现任何待测试的设备!");
		FreeDevice();
		exit(1);
	}

	InitialDevNum();
	InitialTestInfor();
	m_bInitialed = TRUE;//
	if ((*bPci) && !b_dev_test)
	{
		
		//安装前检测一次
		BOOL rest = FALSE;
		BOOL instdrv = FALSE;
		GetRestartAndInstallDriver(&rest, &instdrv); 
		if (rest)
		{
			AfxMessageBox("需要重启电脑!");
			WritePrivateProfileString("QC", "NEED_RESET_COMPUTER", "0", m_ExePath + "Config.ini");
			exit(1);
		}
		
		if (instdrv)
		{
			//				AfxMessageBox("Install Driver");
			//				m_card_infor.resDriver=InstallDriver();
			m_bThisPCSInstallDriver = TRUE;
		}	
		else
		{
			//				AfxMessageBox("Not Install Driver");
			m_card_infor.resDriver = TRUE;
			m_bThisPCSInstallDriver = FALSE;
		}
		
		//安装后检测一次是否需要重启
		rest = FALSE;
		instdrv = FALSE;
		GetRestartAndInstallDriver(&rest, &instdrv);
		if (rest)
		{
			AfxMessageBox("需要重启电脑!");
			WritePrivateProfileString("QC", "NEED_RESET_COMPUTER", "0", m_ExePath + "Config.ini");
			//				FreeDevice();
			exit(1);
		}
	}
	
	if (*bPci)
	{
	    /*新增加了3304S,3308S by chenlong*/
		if (m_card_infor.card_type_id != DVR_CARD_TD3316
			&& m_card_infor.card_type_id != DVR_CARD_TD4104
			&& m_card_infor.card_type_id != DVR_CARD_TD4104i
			&& m_card_infor.card_type_id != DVR_CARD_TD4108	//djx 2008/7/22 4108
			&& m_card_infor.card_type_id != DVR_CARD_TD4116
			&& m_card_infor.card_type_id != DVR_CARD_TD3304//zld 2009/4/24 3304
			&& m_card_infor.card_type_id != DVR_CARD_TD3308//zld 2009/4/24 3308
            && m_card_infor.card_type_id != DVR_CARD_TD3304S//zld 2009/4/24 3304
            && m_card_infor.card_type_id != DVR_CARD_TD3308S//zld 2009/4/24 3308
			&& m_card_infor.card_type_id != DVR_CARD_TD4408)//zld 2009/4/24 4408

		{
			GetVideoDriverName();
			BOOL bresu = InitialVideoDevice(m_dwVideoFormat, pVideoCallBack);
			
			if(!bresu)
			{
				AfxMessageBox("InitialVideoDevice fail!");
			}
			m_card_infor.resPassWord = Password();
		}
	}

	if (*bUsb)
	{
	}
	
	//return bPci||bUsb||bAlarm;
	return *bPci||*bUsb||*bAlarm; //djx 2008/1/5 以上明显错误
}

BOOL CTestCard::CheckPciDvrCard()
{
	int devNum=0;
	BOOL bSubSystemID=FALSE;
	if (m_DeviceControl.GetDeviceID(BT878_VIDEO, &m_card_infor.resSubId, &m_card_infor.card_type_id))//BT878 Video
		devNum++;
	if(m_DeviceControl.GetDeviceID(TW6800_VIDEO, &m_card_infor.resSubId, &m_card_infor.card_type_id))//TW6800 Video
		devNum++;
	//djx 2008/1/5 3004_6805
	if(m_DeviceControl.GetDeviceID(TW6805_VIDEO, &m_card_infor.resSubId, &m_card_infor.card_type_id))//TW6805 Video
	devNum++;

	//zld 2009/6/22 3016_6805/3008_6805
	if(m_DeviceControl.GetDeviceID(TW6805_3016, &m_card_infor.resSubId, &m_card_infor.card_type_id))//TW6805 Video
	{
		devNum++;
		g_6800_6805_check = 1;//1:6805
	}
	
	//ADD 3316
	if(m_DeviceControl.GetDeviceID(ALOG480, &m_card_infor.resSubId, &m_card_infor.card_type_id))//TW6800 Video
		devNum++;
	//ADD 4104
	if(m_DeviceControl.GetDeviceID(ID4104, &m_card_infor.resSubId, &m_card_infor.card_type_id))
		devNum++;
	//ADD 4104i
	if(m_DeviceControl.GetDeviceID(ID4104i, &m_card_infor.resSubId, &m_card_infor.card_type_id))
		devNum++;

	if (devNum > 1)
	{
		AfxMessageBox("一次只能测试一个设备,请关机取出一块继续测试!");
		exit(0);
	}
	if (devNum ==0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTestCard::CheckAlarmCard()
{
	BOOL ret;
	ret = MyIniCard();
	if (ret)
	{
		m_card_infor.card_type_id=DVR_CARD_ALARM_CARD;
		m_card_infor.resSubId=TRUE;
		//djx 2007/12/07
		if (MyGetCardVer() == 0x15)
		{
			ShowWindow(GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_BUTTON_PTZ),TRUE);
			_test_table[m_card_infor.card_type_id][ID_TEST_ALARM_PTZ] = TRUE;
			ShowWindow(GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_TEST_PTZ),TRUE);
			ShowWindow(GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_TEST_PTZ_RES),TRUE);
		}
	}
	return ret;
}

BOOL CTestCard::InstallDriver()
{	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	CString proname="InstallDrv.exe =100";
	if(!CreateProcess( NULL,proname.GetBuffer (proname.GetLength ()),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi )) 
	{
		proname.ReleaseBuffer();
//		::MessageBox(NULL,"EnCodePro.exe","Warning",MB_OK|MB_ICONWARNING);
	}
	proname.ReleaseBuffer();

	WaitForSingleObject(pi.hProcess, INFINITE );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	WritePrivateProfileString("QC","NEED_RESET_COMPUTER",NeedResetComputer()?"1":"0",m_ExePath+"Config.ini");
	WritePrivateProfileString("QC","LAST_CARD_ID",m_card_infor.disname,m_ExePath+"Config.ini");

	return TRUE;
}

BOOL CTestCard::InitialVideoDevice(DWORD VideoFormat, CAPTURECALLBACK *pVideoCallBack)
{	
	DeviceFreeVideo();
	if (0 != m_dwVideoDeviceNum)
	{
		m_dwVideoFormat = VideoFormat;
		m_pVideoDriver = GetVideoDevDrvObject(m_card_infor.card_type_id);
		if(NULL == m_pVideoDriver)
		{
			//DeviceFreeVideo();
			SetLastError(ERROR_OUTOFMEMORY);
			return FALSE;
    	}

        // heliang -
	}
	return TRUE;
}

void CTestCard::GetVideoDriverName()
{
	switch(m_card_infor.card_type_id)
	{
        // heliang -
	default:
		m_strVideoDriverName="";
	}
}

void CTestCard::DeviceFreeVideo()
{

	if(m_pVideoDriver)
	{
		m_pVideoDriver->ExitDriver();
		m_pVideoDriver->CloseDriver();
		delete m_pVideoDriver;
        m_pVideoDriver = NULL;
	}
}

void CTestCard::InitialVar()
{
	m_bThisPCSInstallDriver = FALSE;
	m_bPciInitial = FALSE;
	m_bUsbInitial = FALSE;
	m_bAlarmInitial = FALSE;
	m_card_infor.card_id = "";
	m_card_infor.card_serial_number = "";
//	m_card_infor.card_type_id=DVR_CARD_TDNULL;
	m_card_infor.disname = "";
	m_card_infor.RecordTime = "";
	m_card_infor.RecordUser = "";
	m_card_infor.resAlarmPTZ = FALSE;
	m_card_infor.resAlarmIn = FALSE;
	m_card_infor.resAlarmOut = FALSE;
	m_card_infor.resAlarmRelay = FALSE;
	m_card_infor.resCallMonitor = FALSE;
	m_card_infor.resDriver = TRUE;
	m_card_infor.resPassWord = FALSE;
	m_card_infor.resReset = TRUE;
	m_card_infor.resResult = FALSE;
	m_card_infor.resSetSwitch = FALSE;
	m_card_infor.resSubId = FALSE;
	m_card_infor.video_channel_number = 0;
	m_card_infor.CardFpgaVer = 0; //add djx 2007/08/02
	m_card_infor.resAudio=FALSE;//djx 08/3/3
	m_bInitialed = FALSE;

	m_hAlarmCard = ::LoadLibrary("AlarmCard.dll");
	if (m_hAlarmCard == NULL)
	{
		TRACE("Can not find the AlarmCard.dll");
		return ;
	}
	MyQuitCard = (QuitCard)::GetProcAddress(m_hAlarmCard, "ALM_Quit");
	if (MyQuitCard == NULL)
	{
		TRACE("Can not find the MyQuitCard");
		return ;
	}

	MyAlarmIn = (AlarmIn )::GetProcAddress(m_hAlarmCard, "ALM_AlarmIn");
	if (MyAlarmIn == NULL)
	{
		TRACE("Can not find the MyAlarmIn");
		return ;
	}

	MyAlarmOut = (AlarmOut )::GetProcAddress(m_hAlarmCard, "ALM_AlarmOut");
	if (MyAlarmOut == NULL)
	{
		TRACE("Can not find the MyAlarmOut");
		return ;
	}

	MyIniCard = (IniCard )::GetProcAddress(m_hAlarmCard, "ALM_Initial");
	if (MyIniCard == NULL)
	{
		TRACE("Can not find the alarm card");
		return ;
	}

	//djx 2007/12/07
	MyCommOut=(CommOut )::GetProcAddress(m_hAlarmCard,"ALM_CommOut");
	if (MyCommOut==NULL)
	{
		AfxMessageBox("Can not find the ALM_CommOut");
		return ;
	}
	MyGetCardVer=(GetCardVer )::GetProcAddress(m_hAlarmCard,"ALM_GetCardVer");
	if (MyGetCardVer==NULL)
	{
		AfxMessageBox("Can not find the ALM_GetCardVer");
		return ;
	}
}

BOOL CTestCard::Password()
{
	if(!m_bInitialed)
	{
		SetLastError( DVRERR_DRIVERMUSTINIT);
		return FALSE;
	}
	ASSERT(NULL!= m_pVideoDriver);
	if(NULL == m_pVideoDriver) return FALSE;
	switch(m_card_infor.card_type_id)
	{
	case DVR_CARD_TD3004V2://djx 2008/1/5 3004_6805
	case DVR_CARD_TD3004:
	case DVR_CARD_TD3008:
	case DVR_CARD_TD3016:
	case DVR_CARD_TD3116V1:
	case DVR_CARD_TD3116V2:
	case DVR_CARD_TD3216V2:
	case DVR_CARD_TD3216V3:
	case DVR_CARD_TD3108:
	case DVR_CARD_TD3016V2:
	case DVR_CARD_TD3008V2: //zld 2009/6/23
		ASSERT(NULL!= m_pVideoDriver[0]);
		if(!m_pVideoDriver[0]->Password())
			return FALSE;
		break;
	case DVR_CARD_TD3316:
		return TRUE;
	default:
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			return FALSE;
		}
		break;
	}
	return TRUE;
}

BOOL CTestCard::VideoCaptureStart(DWORD dwBmpSizeType, BITMAPINFOHEADER RecBmpHeader)
{
	if(!m_bInitialed)
	{
		SetLastError( DVRERR_DRIVERMUSTINIT);
		return FALSE;
	}

    const DWORD g_dwSupportSizeType=DVRVIDEO_SIZE_320x240|DVRVIDEO_SIZE_640x480;
	if(!(g_dwSupportSizeType))//&dwBmpSizeType))//不支持尺寸类型
	{
		SetLastError(DVRERR_INVALIDPARA);
		return FALSE;
	}

    // heliang -
	if (m_card_infor.card_type_id == DVR_CARD_TD4104 ||
		m_card_infor.card_type_id == DVR_CARD_TD4104i ||
		m_card_infor.card_type_id == DVR_CARD_TD4108 || //djx 2008/7/22 4108
		m_card_infor.card_type_id == DVR_CARD_TD4116)
	{
	}
	else
	{
		if(NULL == m_pVideoDriver) 
			return FALSE;

		BOOL ret=m_pVideoDriver->StartCapture(dwBmpSizeType);
		if(!ret)
		{
			m_pVideoDriver->StopCapture();
			return FALSE;
		}
	}
	m_bVideoCaptureStart = TRUE;
	return TRUE;
}

void CTestCard::VideoCaptureStop()
{
	if(!m_bInitialed)
	{
		SetLastError(DVRERR_DRIVERMUSTINIT);
		return ;
	}

	if(!m_bVideoCaptureStart)  
		return ;

    m_pVideoDriver->StopCapture();
	m_bVideoCaptureStart = FALSE;
}

BOOL CTestCard::GetAlarmIn(DWORD * status)
{
	int i;
	switch(m_card_infor.card_type_id)
	{
        // heliang -
	case DVR_CARD_ALARM_CARD:
			MyAlarmIn(status);
			TRACE("%x,%x\n",*status>>16,*status&0xffff);
			break;
	default:
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			return TRUE;
		}
		break;
	}	
	return TRUE;
}

BOOL CTestCard::SetAlarmOut(DWORD *status)
{
	int i;

	switch(m_card_infor.card_type_id)
	{
//heliang-
	case DVR_CARD_ALARM_CARD:
			MyAlarmOut(*status);
			break;
	default:
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			return TRUE;
		}
		break;
	}	
	return TRUE;
}

BOOL CTestCard::FreeDevice()
{
	BOOL ret = TRUE;
	if (m_bPciInitial)
	{
		ret = ret && PciDeviceFree();
	}

	if (m_bAlarmInitial)
		ret = ret && AlarmDeviceFree();

return ret;
}

BOOL CTestCard::PciDeviceFree()
{
	if(m_pVideoDriver)
	{
		m_pVideoDriver->ExitDriver();
		m_pVideoDriver->CloseDriver();
		delete m_pVideoDriver;
		m_pVideoDriver=NULL;
	}

	return TRUE;
}

BOOL CTestCard::AlarmDeviceFree()
{
	MyQuitCard();
	return TRUE;
}

void CTestCard::InitialTestInfor()
{
	m_card_infor.disname = _display_name[m_card_infor.card_type_id];
	m_card_infor.card_id = _card_id[m_card_infor.card_type_id];
	m_card_infor.video_channel_number = _video_channel_number[m_card_infor.card_type_id];
}

BOOL CTestCard::NeedResetComputer()
{
	BOOL ret;
	char szBuffer[1];
	DWORD len;
	len=10;
	ret=GetPrivateProfileString("INSTALL","RESULT",NULL,szBuffer,len,m_ExePath+"Result.ini");//m_ExePath
	if (atoi( szBuffer )==2)
		return TRUE;
	else
		return FALSE;
}


void CTestCard::SetVideoFormat(DWORD m_VideoFormat)
{
	m_dwVideoFormat = m_VideoFormat;
}

void CTestCard::ChangeVideoFormat()
{
	if (m_card_infor.card_type_id != DVR_CARD_TD3316 
		&& m_card_infor.card_type_id != DVR_CARD_TDUSB_1
		&& m_card_infor.card_type_id != DVR_CARD_TDUSB_2
		&& m_card_infor.card_type_id != DVR_CARD_TD4104
		&& m_card_infor.card_type_id != DVR_CARD_TD4104i
		&& m_card_infor.card_type_id != DVR_CARD_TD4108 //djx 2008/7/22 4108
		&& m_card_infor.card_type_id != DVR_CARD_TD4116)
	{
		InitialVideoDevice(m_dwVideoFormat, m_pVideoCallBack);
	}
}
/*********************************************************
函数名称：GetInvalidateFlag(int deviceNum)
功能描述：判断是否刷新屏幕
输入参数：deviceNum  当前设备芯片数
输出参数：无
返回值： TRUE需要刷新 FALSE不需要刷新
修改日期   版本号    修改人    修改内容
09.07.07  5.0.6.0  chenlong  新建
**********************************************************/
BOOL CTestCard::GetInvalidateFlag(int deviceNum)
{
    return GetSignalStatus(deviceNum);
}

/*********************************************************
函数名称：GetSignalStatus(int deviceNum)
功能描述：获得当前信号状态并返回上层是否要刷新
输入参数：deviceNum  当前设备芯片数
输出参数：无
返回值： TRUE需要刷新 FALSE不需要刷新或者获取失败
修改日期   版本号    修改人    修改内容
09.07.07  5.0.6.0  chenlong  新建
**********************************************************/
BOOL CTestCard::GetSignalStatus(int deviceNum)
{
    DWORD *pSign = new DWORD[16];
    if (pSign == NULL)
    {
        return FALSE;
    }


    DWORD *pTmp = new DWORD[4];
    m_pVideoDriver->GetSignalStatus(pTmp, 4);
    for (int j1 = 0; j1 < 4; j1++)
    {
        pSign[4 + j1] = pTmp[3 - j1];
        //TRACE("pSign = %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", pSign[0], pSign[1], pSign[2], pSign[3], pSign[4], pSign[5], pSign[6], pSign[7], pSign[8], pSign[9], pSign[10], pSign[11], pSign[12], pSign[13], pSign[14], pSign[15]);
    }
    delete []pTmp;
    pTmp = NULL;

    for (int i2 = 0; i2 < 16; i2++)
    {
        if (m_pSignalStatus[i2] != pSign[i2] && m_pSignalStatus[i2] == 1)
        {
            for (int j2 = 0; j2 < 16; j2++)
            {
                m_pSignalStatus[j2] = pSign[j2];
            }
            delete []pSign;
            pSign = NULL;
            return TRUE;
        }
        else if (m_pSignalStatus[i2] != pSign[i2] && m_pSignalStatus[i2] == 0)
        {
            for (int j2 = 0; j2 < 16; j2++)
            {
                m_pSignalStatus[j2] = pSign[j2];
            }
            delete []pSign;
            pSign = NULL;
            return FALSE;
        }
    }
    delete []pSign;
    pSign = NULL;
    return FALSE;
}
