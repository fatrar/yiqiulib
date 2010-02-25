// IVAlarmOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVAlarmOutDlg.h"


// CIVAlarmOutDlg dialog

IMPLEMENT_DYNAMIC(CIVAlarmOutDlg, CDialog)

CIVAlarmOutDlg::CIVAlarmOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVAlarmOutDlg::IDD, pParent)
    , m_nCurrentChan(0)
    , m_ClickItem(NULL)
{

}

CIVAlarmOutDlg::~CIVAlarmOutDlg()
{
}

void CIVAlarmOutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ALARM_HOLD_TIME, m_AlarmOutHoldTimeComb);
    for (int i=0;i<Alarm_Check_Num;++i)
    {
        DDX_Control(pDX, IDC_CHECK1+i, m_AlarmCheck[i]);
    }
    DDX_Control(pDX, IDC_ALARMOUT_CAMERA_TREE, m_CameraTree);
    DDX_Control(pDX, IDC_AlarmOut_Group, m_AlarmOutGroup);
    DDX_Control(pDX, IDC_NOUSE_HOLD, m_AlarmNoHoldBt);
    DDX_Control(pDX, IDC_USE_HOLD, m_AlarmHoldBt);
    DDX_Control(pDX, IDC_AlarmHold_Group, m_AlarmHoldGroup);
    DDX_Control(pDX, IDC_Choose_Alarm_Group, m_ChooseAlarmGroup);
}


BEGIN_MESSAGE_MAP(CIVAlarmOutDlg, CDialog)
    ON_NOTIFY(NM_RCLICK, IDC_ALARMOUT_CAMERA_TREE, &CIVAlarmOutDlg::OnNMRclickAlarmoutCameraTree)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_NOUSE_HOLD, &CIVAlarmOutDlg::OnBnClickedNouseHold)
    ON_BN_CLICKED(IDC_USE_HOLD, &CIVAlarmOutDlg::OnBnClickedUseHold)
END_MESSAGE_MAP()


// CIVAlarmOutDlg message handlers

BOOL CIVAlarmOutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CString strTmp;
    strTmp.LoadString(IDS_Alarm_Out_Group);
    m_AlarmOutGroup.SetWindowText(strTmp);

    strTmp.LoadString(IDS_Alarm_Hold_Group);
    m_AlarmHoldGroup.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Alarm_Not_Hold);
    m_AlarmNoHoldBt.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Alarm_Hold_Des);
    m_AlarmHoldBt.SetWindowText(strTmp);
    m_AlarmOutHoldTimeComb.ResetContent();
    m_AlarmOutHoldTimeComb.InsertString(0, _T("1"));
    m_AlarmOutHoldTimeComb.InsertString(1, _T("3"));
    m_AlarmOutHoldTimeComb.InsertString(2, _T("5"));
    m_AlarmOutHoldTimeComb.InsertString(3, _T("10"));
    m_AlarmOutHoldTimeComb.InsertString(4, _T("15"));
    m_AlarmOutHoldTimeComb.InsertString(5, _T("20"));
    m_AlarmOutHoldTimeComb.InsertString(6, _T("30"));
    m_AlarmOutHoldTimeComb.SetCurSel(0);

    strTmp.LoadString(IDS_Alarm_Choose_Group);
    m_ChooseAlarmGroup.SetWindowText(strTmp);
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        strTmp.LoadString(Alarm_Out_String_Start+i);
        m_AlarmCheck[i].SetWindowText(strTmp);
        m_AlarmCheck[i].SetCheck(BST_CHECKED);
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVAlarmOutDlg::Init(CWnd* pWnd, const CRect& Rect)
{
    //
    // 1. Init Himself and CameraTree
    //
    Create(IDD, pWnd);
    MoveWindow(Rect);
    InitCameraTree(m_CameraTree, this, m_AlarmOutGroup, 0, Rect.Height());
    
    //
    // 2. Init Other Child Ctrl
    //
    int nAlarmOutGroupWidth = Rect.Width()-CameraCtrl_Width-2*Alarm_Out_X_Offset;
    int nAlarmOutGroupX = CameraCtrl_Width+Alarm_Out_X_Offset;
    int nAlarmOutGroupY = Alarm_Out_Y_Offset;
    int nAllButtonX = nAlarmOutGroupX + Alarm_Out_X_Offset;
    m_AlarmHoldGroup.MoveWindow(nAlarmOutGroupX, nAlarmOutGroupY, nAlarmOutGroupWidth, Alarm_Hold_Height);
    m_AlarmNoHoldBt.MoveWindow(
        nAllButtonX, 
        nAlarmOutGroupY+Alarm_Out_Y_Offset,
        nAlarmOutGroupWidth-2*Alarm_Out_X_Offset,
        BT_Height);
    m_AlarmHoldBt.MoveWindow(
        nAllButtonX, 
        nAlarmOutGroupY+2*Alarm_Out_Y_Offset+BT_Height,
        nAlarmOutGroupWidth-2*Alarm_Out_X_Offset-Combo_Width,
        BT_Height);
    m_AlarmOutHoldTimeComb.MoveWindow(
        Rect.Width()-2*Alarm_Out_X_Offset-Combo_Width, 
        nAlarmOutGroupY+2*Alarm_Out_Y_Offset+BT_Height,
        Combo_Width,
        10*BT_Height);

    int nChooseAlarmGroupY = Alarm_Out_Y_Offset*2 + Alarm_Hold_Height;
    int nChooseAlarmGroupHeight = Rect.Height() - nChooseAlarmGroupY - Alarm_Out_Y_Offset;
    m_ChooseAlarmGroup.MoveWindow(
        nAlarmOutGroupX,
        nChooseAlarmGroupY,
        nAlarmOutGroupWidth,
        nChooseAlarmGroupHeight );

    int nOutBtYOffset = (nChooseAlarmGroupHeight-6*Alarm_Out_Y_Offset)/4;
    int nOutBtYStart = nChooseAlarmGroupY+2*Alarm_Out_Y_Offset;
    int nOutWidth = (nAlarmOutGroupWidth-6*Alarm_Out_X_Offset)/2;
    int nOutButtonX = nAllButtonX+2*Alarm_Out_X_Offset;
    for (int i = 0; i< Alarm_Check_Num/2; ++i)
    {
        m_AlarmCheck[i*2].MoveWindow(
            nOutButtonX,
            nOutBtYStart+i*nOutBtYOffset,
            nOutWidth,
            BT_Height );
        m_AlarmCheck[i*2+1].MoveWindow(
            nOutButtonX + nOutWidth,
            nOutBtYStart+i*nOutBtYOffset,
            nOutWidth,
            BT_Height );
    }
    return TRUE;
}

void CIVAlarmOutDlg::OnNMRclickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    PopUpCameraMemu(m_CameraTree, 1, this, this);
}

void CIVAlarmOutDlg::OnUpdateMemu( 
    CMenu* pMenu,
    WhichMemu Which,
    int nChannelID,
    void* pData,
    HTREEITEM Item )
{
    m_ClickItem = Item;
    switch (Which)
    {
    case Root:
        break;
    case Camera:
        m_nCurrentChan = nChannelID;
        break;
    case Rule:
        m_nCurrentChan = nChannelID;
        break;
    default:
        break;
    }
}

void CIVAlarmOutDlg::OnInitCameraTree(
    int nChannelID,
    HTREEITEM Item )
{

}
void CIVAlarmOutDlg::OnDestroy()
{
    UnitCameraTree(m_CameraTree);
    __super::OnDestroy();   
}

void CIVAlarmOutDlg::OnBnClickedNouseHold()
{
    m_AlarmNoHoldBt.SetCheck(BST_CHECKED);
    m_AlarmHoldBt.SetCheck(BST_UNCHECKED);
    m_AlarmOutHoldTimeComb.EnableWindow(FALSE);
}

void CIVAlarmOutDlg::OnBnClickedUseHold()
{
    m_AlarmNoHoldBt.SetCheck(BST_UNCHECKED);
    m_AlarmHoldBt.SetCheck(BST_CHECKED);
    m_AlarmOutHoldTimeComb.EnableWindow();
}

void CIVAlarmOutDlg::Enable( BOOL bEnable /*= TRUE*/ )
{
    m_AlarmHoldGroup.EnableWindow(bEnable);
    m_AlarmNoHoldBt.EnableWindow(bEnable);
    m_AlarmHoldBt.EnableWindow(bEnable);
    m_AlarmOutHoldTimeComb.EnableWindow(bEnable);

    m_ChooseAlarmGroup.EnableWindow(bEnable);
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        m_AlarmCheck[i].EnableWindow(bEnable);
    }
}