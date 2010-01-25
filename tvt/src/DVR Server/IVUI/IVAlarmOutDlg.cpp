// IVAlarmOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVAlarmOutDlg.h"


// CIVAlarmOutDlg dialog

IMPLEMENT_DYNAMIC(CIVAlarmOutDlg, CDialog)

CIVAlarmOutDlg::CIVAlarmOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVAlarmOutDlg::IDD, pParent)
{

}

CIVAlarmOutDlg::~CIVAlarmOutDlg()
{
}

void CIVAlarmOutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ALARM_HOLD_TIME, m_AlarmOutHoldTimeComb);
    for (int i=0;i<ALARM_CHECK_NUM;++i)
    {
        DDX_Control(pDX, IDC_CHECK1+i, m_AlarmCheck[i]);
    }
    DDX_Control(pDX, IDC_ALARMOUT_CAMERA_TREE, m_CameraTree);
}


BEGIN_MESSAGE_MAP(CIVAlarmOutDlg, CDialog)
END_MESSAGE_MAP()


// CIVAlarmOutDlg message handlers
