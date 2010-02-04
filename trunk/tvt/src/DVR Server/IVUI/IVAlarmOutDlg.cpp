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

BOOL CIVAlarmOutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVAlarmOutDlg::Init( CWnd* pWnd, const CRect& rect )
{
    Create(CIVAlarmOutDlg::IDD, pWnd);
    return TRUE;
}