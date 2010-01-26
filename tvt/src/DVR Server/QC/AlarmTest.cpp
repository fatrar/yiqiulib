// AlarmTest.cpp : implementation file
//

#include "stdafx.h"
#include "QC.h"
#include "AlarmTest.h"
#include "qcdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmTest dialog


CAlarmTest::CAlarmTest(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmTest)
	m_parent=pParent;
	//}}AFX_DATA_INIT
}


void CAlarmTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmTest)
	DDX_Control(pDX, IDC_ALARM_RELAY_CAPTURE, m_relay_capture);
	DDX_Control(pDX, IDC_ALARM_OUT_CAPTURE, m_out_capture);
	DDX_Control(pDX, IDC_ALARM_IN_CAPTURE, m_in_capture);
	DDX_Control(pDX, IDC_AUTO_TEST, m_auto_test);
	DDX_Control(pDX, IDC_ALARM_OUT_RES, m_c_alarm_out_res);
	DDX_Control(pDX, IDC_ALARM_RELAY_RES, m_c_alarm_relay_res);
	DDX_Control(pDX, IDC_ALARM_IN_RES, m_c_alarm_in_res);
	DDX_Control(pDX, IDC_LAMP_ALARM_J, m_C_alarm_out_j);
	DDX_Control(pDX, IDC_LAMP_ALARM_OUT_4, m_C_alarm_out_4);
	DDX_Control(pDX, IDC_LAMP_ALARM_OUT_3, m_C_alarm_out_3);
	DDX_Control(pDX, IDC_LAMP_ALARM_OUT_2, m_C_alarm_out_2);
	DDX_Control(pDX, IDC_LAMP_ALARM_OUT_1, m_C_alarm_out_1);
	DDX_Control(pDX, IDC_ALARM_IN_1, m_C_alarm_in_1);
	DDX_Control(pDX, IDC_ALARM_IN_2, m_C_alarm_in_2);
	DDX_Control(pDX, IDC_ALARM_IN_3, m_C_alarm_in_3);
	DDX_Control(pDX, IDC_ALARM_IN_4, m_C_alarm_in_4);
	DDX_Control(pDX, IDC_ALARM_IN_5, m_C_alarm_in_5);
	DDX_Control(pDX, IDC_ALARM_IN_6, m_C_alarm_in_6);
	DDX_Control(pDX, IDC_ALARM_IN_7, m_C_alarm_in_7);
	DDX_Control(pDX, IDC_ALARM_IN_8, m_C_alarm_in_8);
	DDX_Control(pDX, IDC_ALARM_IN_9, m_C_alarm_in_9);
	DDX_Control(pDX, IDC_ALARM_IN_10, m_C_alarm_in_10);
	DDX_Control(pDX, IDC_ALARM_IN_11, m_C_alarm_in_11);
	DDX_Control(pDX, IDC_ALARM_IN_12, m_C_alarm_in_12);
	DDX_Control(pDX, IDC_ALARM_IN_13, m_C_alarm_in_13);
	DDX_Control(pDX, IDC_ALARM_IN_14, m_C_alarm_in_14);
	DDX_Control(pDX, IDC_ALARM_IN_15, m_C_alarm_in_15);
	DDX_Control(pDX, IDC_ALARM_IN_16, m_C_alarm_in_16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmTest, CDialog)
	//{{AFX_MSG_MAP(CAlarmTest)
	ON_BN_CLICKED(IDC_FINISH, OnFinish)
	ON_BN_CLICKED(IDC_ALARM_IN_RES, OnAlarmInRes)
	ON_BN_CLICKED(IDC_ALARM_OUT_RES, OnAlarmOutRes)
	ON_BN_CLICKED(IDC_ALARM_RELAY_RES, OnAlarmRelayRes)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AUTO_TEST, OnAutoTest)
	ON_BN_CLICKED(IDC_LAMP_ALARM_J, OnLampAlarmJ)
	ON_BN_CLICKED(IDC_LAMP_ALARM_OUT_1, OnLampAlarmOut1)
	ON_BN_CLICKED(IDC_LAMP_ALARM_OUT_2, OnLampAlarmOut2)
	ON_BN_CLICKED(IDC_LAMP_ALARM_OUT_4, OnLampAlarmOut4)
	ON_BN_CLICKED(IDC_LAMP_ALARM_OUT_3, OnLampAlarmOut3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmTest message handlers

void CAlarmTest::OnFinish() 
{
	// TODO: Add your control notification handler code here
		CQCDlg *pdlg=(CQCDlg *) m_parent;
		pdlg->m_Alarmtest=FALSE;
		pdlg->m_calarm_test.SetCheck(0);
		KillTimer(1);


//Refresh Infor 
		CString str;
		m_c_alarm_in_res.GetWindowText(str);
		pdlg->m_c_alarm_in_res.SetWindowText(str);
		if ("√"==str)
			pdlg->m_card_infor.resAlarmIn=TRUE;
		else
			pdlg->m_card_infor.resAlarmIn=FALSE;

		m_c_alarm_out_res.GetWindowText(str);
		pdlg->m_c_alarm_out_res.SetWindowText(str);
		if ("√"==str)
			pdlg->m_card_infor.resAlarmOut=TRUE;
		else
			pdlg->m_card_infor.resAlarmOut=FALSE;

		m_c_alarm_relay_res.GetWindowText(str);
		pdlg->m_c_alarm_relay_res.SetWindowText(str);
		if ("√"==str)
			pdlg->m_card_infor.resAlarmRelay=TRUE;
		else
			pdlg->m_card_infor.resAlarmRelay=FALSE;




		CDialog::OnOK();
}
BOOL CAlarmTest::OnInitDialog()
{
		CDialog::OnInitDialog();
		InitialVar();
		InitialControl();
		if ((alarmin>0)||(alarmout>0)||(alarmrelay>0))
		{
			//CRect rect(0,0,400,150);
			//MoveWindow(&rect);
			//::SetWindowPos(m_hWnd,NULL,346,585,1,1,1);
			SetTimer(1,100,NULL);
		}
		else
		{
			AfxMessageBox("此设备没有该项功能!");
			OnFinish();
		}
	return TRUE;
}

void CAlarmTest::OnAlarmInRes() 
{
	CString str;
	m_c_alarm_in_res.GetWindowText(str);
	
	if (str=="√")
	{
		m_c_alarm_in_res.SetWindowText("X");
//		m_card_infor.resAlarmIn=FALSE;
	}
	else if (str=="X"||str=="")
	{
		m_c_alarm_in_res.SetWindowText("√");
//		m_card_infor.resAlarmIn=TRUE;
		
	}	
}

void CAlarmTest::OnAlarmOutRes() 
{
	CString str;
	m_c_alarm_out_res.GetWindowText(str);
	if (str=="√")
	{
		m_c_alarm_out_res.SetWindowText("X");
//		m_card_infor.resAlarmOut=FALSE;
	}
	else if (str=="X"||str=="")
	{
		m_c_alarm_out_res.SetWindowText("√");
//		m_card_infor.resAlarmOut=TRUE;
		
	}	
}

void CAlarmTest::OnAlarmRelayRes() 
{
	CString str;
	m_c_alarm_relay_res.GetWindowText(str);
	if (str=="√")
	{
		m_c_alarm_relay_res.SetWindowText("X");
//		m_card_infor.resAlarmRelay =FALSE;
	}
	else if (str=="X"||str=="")
	{
		m_c_alarm_relay_res.SetWindowText("√");
//		m_card_infor.resAlarmRelay =TRUE;	
	}	
}
//ID_TEST_ALARM_IN-5;
//
int  CARD_ALARM_INFOR[SUPPORT_CARD_NUMBER][TEST_ALARM_TYPE_NUMBER]=
{
		4,	0,	1,//3004	
		0,	0,	0,//3008
		16,	4,	1,//3016
		16,	4,	1,//3116v1
	    16,	4,	1,//3116v2
		0,	0,	1,//3216v2
		0,	0,	1,//3216v3
		16,	4,	1,//alarm card
		0,  0,  0,
		//djx 2008/1/5 3004_6805 ----------
		0,	0,	0,
		0,	0,	0,
		0,	0,	0,
		0,	0,	0,
		0,	0,	0,
		0,	0,	0,
		4,	0,	1,
		0,  0,  0,
		8,  0,  1,//3308
		4,  0,  1, //3304
		0,  0,  0, //4408
		16,	4,	1, //zld 2009/6/22 3016V2
		0,	0,	0,//zld 3008
        4,  0,  0,//3304S by chenlong
        8,  0,  0,//3308S by chenlong
        0,	0,	0
		//----------------------
};

void CAlarmTest::InitialControl()
{
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	if (DVR_CARD_ALARM_CARD==pdlg->m_card_infor.card_type_id)
	{
		m_auto_test.ShowWindow(0);
	}
	alarmin=CARD_ALARM_INFOR[pdlg->m_card_infor.card_type_id][ID_TEST_ALARM_IN-5];
	alarmout=CARD_ALARM_INFOR[pdlg->m_card_infor.card_type_id][ID_TEST_ALARM_OUT-5];
	alarmrelay=CARD_ALARM_INFOR[pdlg->m_card_infor.card_type_id][ID_TEST_ALARM_RELAY-5];
	InitialIn(alarmin);
	InitialOut(alarmout);
	InitialRelay(!!alarmrelay);
}
void CAlarmTest::InitialIn(int num)
{
	int cnt=16;
	if ((cnt--)>num)
	{
		m_C_alarm_in_16.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_15.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_14.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_13.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_12.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_11.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_10.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_9.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_8.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_7.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_6.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_5.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_4.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_3.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_2.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_in_1.ShowWindow(FALSE);
		m_c_alarm_in_res.ShowWindow(FALSE);
		m_in_capture.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
}

void CAlarmTest::InitialRelay(BOOL have)
{
	if (!have)
	{
		m_C_alarm_out_j.ShowWindow(FALSE);
		m_c_alarm_relay_res.ShowWindow(FALSE);
		m_relay_capture.ShowWindow(FALSE);
	}
}
void CAlarmTest::InitialOut(int num)
{
//Out
	int cnt=4;
	if ((cnt--)>num)
	{
		m_C_alarm_out_4.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_out_3.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_out_2.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
	if ((cnt--)>num)
	{
		m_C_alarm_out_1.ShowWindow(FALSE);
		m_c_alarm_out_res.ShowWindow(FALSE);
		m_out_capture.ShowWindow(FALSE);
	}
	else
	{
		return ;
	}
}

void CAlarmTest::RefreshControlList(DWORD * AlarmIn,DWORD * AlarmOut)
{
		BOOL ret=FALSE;
		m_C_alarm_in_1.SetCheck(!AlarmIn[0]);
		m_C_alarm_in_2.SetCheck(!AlarmIn[1]);
		m_C_alarm_in_3.SetCheck(!AlarmIn[2]);
		m_C_alarm_in_4.SetCheck(!AlarmIn[3]);
		m_C_alarm_in_5.SetCheck(!AlarmIn[4]);
		m_C_alarm_in_6.SetCheck(!AlarmIn[5]);
		m_C_alarm_in_7.SetCheck(!AlarmIn[6]);
		m_C_alarm_in_8.SetCheck(!AlarmIn[7]);
		m_C_alarm_in_9.SetCheck(!AlarmIn[8]);
		m_C_alarm_in_10.SetCheck(!AlarmIn[9]);
		m_C_alarm_in_11.SetCheck(!AlarmIn[10]);
		m_C_alarm_in_12.SetCheck(!AlarmIn[11]);
		m_C_alarm_in_13.SetCheck(!AlarmIn[12]);
		m_C_alarm_in_14.SetCheck(!AlarmIn[13]);
		m_C_alarm_in_15.SetCheck(!AlarmIn[14]);
		m_C_alarm_in_16.SetCheck(!AlarmIn[15]);
		if(m_bAutoAlarmOut)
		{
            int i;
			for (i=0;i<alarmin;i++)
			{
				ret=ret||(!AlarmIn[i]);
			}
			for (i=0;i<alarmout;i++)
			{
				AlarmOut[i]=((!AlarmIn[i])||(!AlarmIn[4+i])||(!AlarmIn[8+i])||(!AlarmIn[12+i]));
			}
			m_C_alarm_out_1.SetCheck(AlarmOut[0]);
			m_C_alarm_out_2.SetCheck(AlarmOut[1]);
			m_C_alarm_out_3.SetCheck(AlarmOut[2]);
			m_C_alarm_out_4.SetCheck(AlarmOut[3]);
			m_C_alarm_out_j.SetCheck(ret);
		}
		CQCDlg *pdlg=(CQCDlg *) m_parent;
		//if (DVR_CARD_TD3004==pdlg->m_card_infor.card_type_id)
		//djx 2008/1/5 3004_6805
		if (DVR_CARD_TD3004==pdlg->m_card_infor.card_type_id || DVR_CARD_TD3004V2==pdlg->m_card_infor.card_type_id)
		{
			AlarmOut[0]=ret;
		}
}


void CAlarmTest::InitialVar()
{
	alarmrelay=0;
	alarmin=0;
	alarmout=0;
	m_bAutoAlarmOut=FALSE;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
    int i;
	for (i=0;i<TEST_ALARM_TYPE_NUMBER;i++)
	{
		if (CARD_ALARM_INFOR[pdlg->m_card_infor.card_type_id][i]>0)
			m_CheckAlarm[i]=TRUE;
		else
			m_CheckAlarm[i]=FALSE;
	}
	for (i=0;i<4;i++)
		m_AlarmOutStatus[i]=0;
}

void CAlarmTest::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	BOOL  ret=FALSE;
	KillTimer(1);
	if (DVR_CARD_ALARM_CARD==pdlg->m_card_infor.card_type_id)
	{
		pdlg->m_TestCard.GetAlarmIn(&m_in);
		if (m_in!=0xffffffff)
		{
			SetKeyBoard(m_in);
			MakeAlarmCardOutStatus(&m_out);
			pdlg->m_TestCard.SetAlarmOut(&m_out);	
		}
	}
// 	else if (DVR_CARD_TD3304==pdlg->m_card_infor.card_type_id)
// 	{
// 		DWORD pchlstatus[4]={0xffff,0xffff,0xffff,0xffff};
// 		if(pdlg->m_MyDSP != NULL)
// 		{
// 			pdlg->m_MyDSP->GetAllChannelStatus(CHSTATUS_GET_SENSOR,4,pchlstatus);
// 			
// 			
// 		}
// 		CString ss;
// 		for (int i =0;i<4;i++)
// 		{
// 			ss.Format("CAlarmTest::OnTimer -%d\n",pchlstatus[i]);
// 			TRACE(ss);
// 			if (pchlstatus[i] == 0)
// 			{
// 				SetKeyBoard((DWORD)(i+1));
// 			}
// 		}
// 	}
	else
	{
		
		else if (!pdlg->m_TestCard.GetAlarmIn(m_AlarmInStatus))
		{
			goto ll;
		}

		RefreshControlList(m_AlarmInStatus,m_AlarmOutStatus);
		if (m_bAutoAlarmOut)
		{
			if (DVR_CARD_TD3304==pdlg->m_card_infor.card_type_id)
			{
				if(pdlg->m_MyDSP != NULL)
				{
					pdlg->m_MyDSP->SetChannelStatus(CHSTATUS_SET_ALARM,0,(long)m_AlarmOutStatus[0]);
				}	
			}
			else if (DVR_CARD_TD3308 == pdlg->m_card_infor.card_type_id)
			{
				if(pdlg->m_MyDSP != NULL)
				{
					pdlg->m_MyDSP->SetChannelStatus(CHSTATUS_SET_ALARM,0,(long)m_AlarmOutStatus[0]);
				}	
			}
			else
			{
				TRACE("Out---yyy---%d\n",m_AlarmOutStatus[0]);
				ret=pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);
			}
			
		}
		if ((DVR_CARD_TD3216V2==pdlg->m_card_infor.card_type_id)||(DVR_CARD_TD3216V3==pdlg->m_card_infor.card_type_id))
			m_C_alarm_out_j.SetCheck(ret);
/*
		if ((pdlg->m_TestCard.GetAlarmIn(m_AlarmInStatus))&&m_bAutoAlarmOut)
		{
			TRACE("In ------%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",(int)m_AlarmInStatus[0],m_AlarmInStatus[1],m_AlarmInStatus[2],m_AlarmInStatus[3],m_AlarmInStatus[4],m_AlarmInStatus[5],m_AlarmInStatus[6],m_AlarmInStatus[7],m_AlarmInStatus[8],m_AlarmInStatus[9],m_AlarmInStatus[10],m_AlarmInStatus[11],m_AlarmInStatus[12],m_AlarmInStatus[13],m_AlarmInStatus[14],m_AlarmInStatus[15]);
			TRACE("Out------%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",(int)m_AlarmOutStatus[0],m_AlarmOutStatus[1],m_AlarmOutStatus[2],m_AlarmOutStatus[3],m_AlarmOutStatus[4],m_AlarmOutStatus[5],m_AlarmOutStatus[6],m_AlarmOutStatus[7],m_AlarmOutStatus[8],m_AlarmOutStatus[9],m_AlarmOutStatus[10],m_AlarmOutStatus[11],m_AlarmOutStatus[12],m_AlarmOutStatus[13],m_AlarmOutStatus[14],m_AlarmOutStatus[15]);
//			m_C_alarm_out_j.SetCheck(pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus));
			pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);
		}
*/
	}
ll:
	SetTimer(1,100,NULL);
	CDialog::OnTimer(nIDEvent);
}

void CAlarmTest::SetKeyBoard(DWORD in)
{
	CString tmp;
	long i4,i3,i2,i1;
	tmp.Format("%x",in);
	tmp=tmp.Right(4);
	i4=StrToData(tmp.Left(1),16);
	i3=StrToData(tmp.Mid(1,1),16);
	i2=StrToData(tmp.Mid(2,1),16);
	i1=StrToData(tmp.Right(1),16);
	switch (15-i1)
	{
	case 0:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 1:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 2:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 3:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 4:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 5:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 6:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 7:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(0);
		break;
	case 8: 
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 9: 
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 10:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 11:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(0);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 12:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 13: 
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(0);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(1);
		break;
	case 14:
		m_C_alarm_in_1.SetCheck(0);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(1);		
		break;
	case 15:
		m_C_alarm_in_1.SetCheck(1);
		m_C_alarm_in_2.SetCheck(1);
		m_C_alarm_in_3.SetCheck(1);
		m_C_alarm_in_4.SetCheck(1);	
	}

	switch (15-i2)
	{
	case 0:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 1:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 2:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 3:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 4:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 5:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 6:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 7:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(0);
		break;
	case 8: 
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 9: 
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 10:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 11:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(0);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 12:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 13: 
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(0);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(1);
		break;
	case 14:
		m_C_alarm_in_5.SetCheck(0);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(1);		
		break;
	case 15:
		m_C_alarm_in_5.SetCheck(1);
		m_C_alarm_in_6.SetCheck(1);
		m_C_alarm_in_7.SetCheck(1);
		m_C_alarm_in_8.SetCheck(1);	
	}

	switch (15-i3)
	{
	case 0:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 1:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 2:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 3:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 4:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 5:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 6:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 7:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(0);
		break;
	case 8: 
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 9: 
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 10:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 11:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(0);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 12:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 13: 
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(0);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(1);
		break;
	case 14:
		m_C_alarm_in_9.SetCheck(0);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(1);		
		break;
	case 15:
		m_C_alarm_in_9.SetCheck(1);
		m_C_alarm_in_10.SetCheck(1);
		m_C_alarm_in_11.SetCheck(1);
		m_C_alarm_in_12.SetCheck(1);	
	}

	switch (15-i4)
	{
	case 0:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 1:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 2:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 3:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 4:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 5:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 6:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 7:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(0);
		break;
	case 8: 
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 9: 
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 10:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 11:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(0);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 12:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 13: 
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(0);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(1);
		break;
	case 14:
		m_C_alarm_in_13.SetCheck(0);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(1);		
		break;
	case 15:
		m_C_alarm_in_13.SetCheck(1);
		m_C_alarm_in_14.SetCheck(1);
		m_C_alarm_in_15.SetCheck(1);
		m_C_alarm_in_16.SetCheck(1);	
	}
}

long CAlarmTest::StrToData(CString strData, int jz)
{
	int l=strData.GetLength();
	long IntValue=0;
	int bitValue;
	int i;
	for(i=0;i<l;i++)
	{
		if(isRightData(strData[i],jz))
		{
			if(isdigit(strData[i]))
				bitValue=strData[i]-0x30;
			else
			{
				switch(strData[i])
				{
				case 'a':
					bitValue=0x0a;
					break;
				case 'b':
					bitValue=0x0b;
					break;
				case 'c':
					bitValue=0x0c;
					break;
				case 'd':
					bitValue=0x0d;
					break;
				case 'e':
					bitValue=0x0e;
					break;
				case 'f':
					bitValue=0x0f;
					break;
				case 'A':
					bitValue=0x0a;
					break;
				case 'B':
					bitValue=0x0b;
					break;
				case 'C':
					bitValue=0x0c;
					break;
				case 'D':
					bitValue=0x0d;
					break;
				case 'E':
					bitValue=0x0e;
					break;
				case 'F':
					bitValue=0x0f;
					break;
				}
			}
			IntValue=bitValue*npf(jz,l-i-1)+IntValue;
		}
		else
			return -1;
	}
	return IntValue;
}

BOOL CAlarmTest::isRightData(char c, int jz)
{
	if((jz<2)||(jz>16))
		return FALSE;
	char ch[]="00112233445566778899aAbBcCdDeEfF";
	int i=0;
	for(i=0;i<jz;i++)
	{
		if(c==ch[i*2])return TRUE;
		if(c==ch[i*2+1])return TRUE;
	}
	return FALSE;
}

long CAlarmTest::npf(int d, int n)
{
	int i;
	long m=1;
	for(i=0;i<n;i++)
		m*=d;
	return m;
}

BOOL CAlarmTest::MakeAlarmCardOutStatus(DWORD *out)
{
	DWORD temp=0;
	int jdq=1;
	int lp1=0,lp2=0,lp3=0,lp4=0;
	jdq=rand()%2;lp1=rand()%2;lp2=rand()%2;lp3=rand()%2;lp4=rand()%2;
	m_C_alarm_out_1.SetCheck(lp1);
	m_C_alarm_out_2.SetCheck(lp1);
	m_C_alarm_out_3.SetCheck(lp2);
	m_C_alarm_out_4.SetCheck(lp3);
	m_C_alarm_out_j.SetCheck(jdq);
	if (jdq)
	{
		temp|=0x01<<16;
	}
	if (lp1)
	{
		temp|=0x01;
	}
	if (lp2)
	{
		temp|=0x01<<1;
	}
	if (lp3)
	{
		temp|=0x01<<2;
	}
	if (lp4)
	{
		temp|=0x01<<3;
	}	
	*out=temp;
	return TRUE;
}

BOOL CAlarmTest::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			//::TranslateMessage(pMsg);
			//::DispatchMessage(pMsg);
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAlarmTest::OnAutoTest() 
{
	// TODO: Add your control notification handler code here
	m_bAutoAlarmOut=m_auto_test.GetCheck();
}

void CAlarmTest::OnLampAlarmJ() 
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	ManueAlarmOut();
	if (DVR_CARD_TD3304==pdlg->m_card_infor.card_type_id ||//zld 2009/4/23 3304
		DVR_CARD_TD3308==pdlg->m_card_infor.card_type_id)//zld 2009/4/23 3308
	{
		if(pdlg->m_MyDSP != NULL)
		{
			pdlg->m_MyDSP->SetChannelStatus(CHSTATUS_SET_ALARM,0,(long)m_AlarmOutStatus[0]);
		}	
	}
	else
	{
		pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);	
	}
}

void CAlarmTest::OnLampAlarmOut1() 
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	ManueAlarmOut();
 
	pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);	
	
}

void CAlarmTest::OnLampAlarmOut2() 
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	ManueAlarmOut();
	pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);	
}

void CAlarmTest::OnLampAlarmOut3() 
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	ManueAlarmOut();
	pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);	
}

void CAlarmTest::OnLampAlarmOut4() 
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	ManueAlarmOut();
	pdlg->m_TestCard.SetAlarmOut(m_AlarmOutStatus);	
}


void CAlarmTest::ManueAlarmOut()
{
	if (m_bAutoAlarmOut) return;
	CQCDlg *pdlg=(CQCDlg *) m_parent;
	static BOOL j=FALSE;
	switch(pdlg->m_card_infor.card_type_id)
	{
	case DVR_CARD_TD3304://zld 2009/3/23 3304
	case DVR_CARD_TD3308:
		m_AlarmOutStatus[0]=1;
		TRACE("Out---XXX---%d\n",m_AlarmOutStatus[0]);
		break;
	case DVR_CARD_TD3004V2://djx 2008/1/5 3004_6805
	case DVR_CARD_TD3004:
		m_AlarmOutStatus[0]=j;
		TRACE("Out---XXX---%d\n",m_AlarmOutStatus[0]);
		break;
	case DVR_CARD_TD3008:
	case DVR_CARD_TD3016:
	case DVR_CARD_TD3016V2:
	case DVR_CARD_TD3008V2: //zld 2009/6/23
		m_AlarmOutStatus[0]=m_C_alarm_out_1.GetCheck();
		m_AlarmOutStatus[1]=m_C_alarm_out_2.GetCheck();
		m_AlarmOutStatus[2]=m_C_alarm_out_3.GetCheck();
		m_AlarmOutStatus[3]=m_C_alarm_out_4.GetCheck();
		m_AlarmOutStatus[4]=m_C_alarm_out_j.GetCheck();
		break;
	case DVR_CARD_TD3116V1:
	case DVR_CARD_TD3116V2:
		m_AlarmOutStatus[0]=m_C_alarm_out_1.GetCheck();
		m_AlarmOutStatus[1]=m_C_alarm_out_2.GetCheck();
		m_AlarmOutStatus[2]=m_C_alarm_out_3.GetCheck();
		m_AlarmOutStatus[3]=m_C_alarm_out_4.GetCheck();
		m_AlarmOutStatus[4]=j;
		m_C_alarm_out_j.SetCheck(m_AlarmOutStatus[0]||m_AlarmOutStatus[1]||m_AlarmOutStatus[2]||m_AlarmOutStatus[3]);
		break;
	case DVR_CARD_TD3216V2:
	case DVR_CARD_TD3216V3:
	case DVR_CARD_ALARM_CARD:
	default:
		break;
	}
	j=!j;
}
