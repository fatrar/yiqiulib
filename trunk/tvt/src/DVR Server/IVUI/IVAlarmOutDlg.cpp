/**CPP*************************************************************************
 File            : IVAlarmOutDlg.cpp
 Subsystem       : 
 Function Name(s): CIVAlarmOutDlg
 Author          : YiQiu
 Date            : 2010-3-8  
 Time            : 17:04
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVUI.h"
#include "IVAlarmOutDlg.h"
 
static const TCHAR* s_szComboString[] = 
{
    _T("1"),   // 0
    _T("3"),   // 1
    _T("5"),   // 2
    _T("10"),  // 3
    _T("15"),  // 4
    _T("20"),  // 5
    _T("30"),  // 6
};

static const int s_szComboInt[] = 
{
// 0   1   2  3   4   5   6
   1,  3,  5, 10, 15, 20, 30
};

// CIVAlarmOutDlg dialog

IMPLEMENT_DYNAMIC(CIVAlarmOutDlg, CDialog)

CIVAlarmOutDlg::CIVAlarmOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVAlarmOutDlg::IDD, pParent)
    , m_nCurrentChan(0)
    , m_pCurentAlarmSet(NULL)
    , m_ClickItem(NULL)
{
    CIVCfgDoc::RegisterRuleTrigger(this);
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
    DDX_Control(pDX, IDC_AlarmOut_Group, m_TreeGroup);
    DDX_Control(pDX, IDC_NOUSE_HOLD, m_AlarmNoHoldBt);
    DDX_Control(pDX, IDC_USE_HOLD, m_AlarmHoldBt);
    DDX_Control(pDX, IDC_AlarmHold_Group, m_AlarmHoldGroup);
    DDX_Control(pDX, IDC_Choose_Alarm_Group, m_ChooseAlarmGroup);
    DDX_Control(pDX, IDC_Apply_BT, m_ApplyBT);
}


BEGIN_MESSAGE_MAP(CIVAlarmOutDlg, CDialog)
    ON_NOTIFY(NM_RCLICK, IDC_ALARMOUT_CAMERA_TREE, &CIVAlarmOutDlg::OnNMRclickAlarmoutCameraTree)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_NOUSE_HOLD, &CIVAlarmOutDlg::OnBnClickedNouseHold)
    ON_BN_CLICKED(IDC_USE_HOLD, &CIVAlarmOutDlg::OnBnClickedUseHold)
    ON_NOTIFY(NM_CLICK, IDC_ALARMOUT_CAMERA_TREE, &CIVAlarmOutDlg::OnNMClickAlarmoutCameraTree)
    ON_BN_CLICKED(IDC_Apply_BT, &CIVAlarmOutDlg::OnBnClickedApplyBt)
    ON_COMMAND(ID_Alarm_FULL, &CIVAlarmOutDlg::OnAlarmFull)
    ON_COMMAND(ID_Alarm_EMPTY, &CIVAlarmOutDlg::OnAlarmEmpty)
    ON_COMMAND(ID_Alarm_COPY, &CIVAlarmOutDlg::OnAlarmCopy)
    ON_COMMAND(ID_Alarm_PASTE, &CIVAlarmOutDlg::OnAlarmPaste)
    ON_COMMAND(ID_Alarm_USETOALL, &CIVAlarmOutDlg::OnAlarmUsetoall)
END_MESSAGE_MAP()


// CIVAlarmOutDlg message handlers

BOOL CIVAlarmOutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CString strTmp;
    strTmp.LoadString(g_hmodule, IDS_Alarm_Out_Group);
    m_TreeGroup.SetWindowText(strTmp);

    strTmp.LoadString(g_hmodule, IDS_Alarm_Hold_Group);
    m_AlarmHoldGroup.SetWindowText(strTmp);
    strTmp.LoadString(g_hmodule, IDS_Alarm_Not_Hold);
    m_AlarmNoHoldBt.SetWindowText(strTmp);
    strTmp.LoadString(g_hmodule, IDS_Alarm_Hold_Des);
    m_AlarmHoldBt.SetWindowText(strTmp);

    // AlarmOut Hold Time Combobox
    m_AlarmOutHoldTimeComb.ResetContent();
    for (int i=0; i<sizeof(s_szComboString)/sizeof(TCHAR*); ++i)
    {
        m_AlarmOutHoldTimeComb.InsertString(i, s_szComboString[i]);
    }
    m_AlarmOutHoldTimeComb.SetCurSel(0);

    strTmp.LoadString(IDS_Alarm_Choose_Group);
    m_ChooseAlarmGroup.SetWindowText(strTmp);
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        strTmp.LoadString(g_hmodule, Alarm_Out_String_Start+i);
        m_AlarmCheck[i].SetWindowText(strTmp);
        m_AlarmCheck[i].SetCheck(BST_CHECKED);
    }
    
    strTmp.LoadString(g_hmodule, IDS_Apply);
    m_ApplyBT.SetWindowText(strTmp);

    Enable(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVAlarmOutDlg::Init(CWnd* pWnd, const CRect& Rect)
{
    int nHeight = Rect.Height();
    int nWidth = Rect.Width();
    //
    // 1. Init Himself and CameraTree
    //
    Create(IDD, pWnd);
    MoveWindow(Rect);
    InitCameraTree(m_CameraTree, this, m_TreeGroup, 0, nHeight);
    
    //
    // 2. Init Other Child Ctrl
    //    
    int nAlarmOutGroupWidth = nWidth-CameraCtrl_Width-2*Alarm_Out_X_Offset;
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
        nWidth-2*Alarm_Out_X_Offset-Combo_Width, 
        nAlarmOutGroupY+2*Alarm_Out_Y_Offset+BT_Height,
        Combo_Width,
        10*BT_Height);

    // Init Alarm Group
    int nChooseAlarmGroupY = Alarm_Out_Y_Offset*2 + Alarm_Hold_Height;
    int nChooseAlarmGroupHeight = nHeight - nChooseAlarmGroupY - Alarm_Out_Y_Offset;
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

    m_ApplyBT.MoveWindow(
        nWidth-ApplyBT_X_ROffset,
        nHeight-ApplyBT_Y_ROffset,
        ApplyBT_Width,
        ApplyBT_Height);
    return TRUE;
}

void CIVAlarmOutDlg::OnNMRclickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    PopUpCameraMemu(m_CameraTree, 1, this, this);
}

void CIVAlarmOutDlg::OnNMClickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
}


void CIVAlarmOutDlg::OnInitCameraTree(
    int nChannelID,
    HTREEITEM Item )
{
    CIVCfgDoc::OnInitCameraTree(nChannelID,Item);
}

void CIVAlarmOutDlg::OnUpdateMemu( 
    CMenu* pMenu,
    WhichMemu Which,
    int nChannelID,
    const void* pData,
    HTREEITEM Item )
{
    m_ClickItem = Item;
    switch (Which)
    {
    case IV_Tree_Root:
        break;
    case IV_Tree_Camera:
        m_nCurrentChan = nChannelID;
        break;
    case IV_Tree_Rule:
        m_nCurrentChan = nChannelID;
        break;
    default:
        break;
    }
}

void CIVAlarmOutDlg::OnClickCameraTree(
    WhichMemu Which,
    int nChannelID,
    const void* pData, 
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
    m_ApplyBT.EnableWindow(bEnable);
}


void CIVAlarmOutDlg::OnBnClickedApplyBt()
{
    CollectUserSet();
    if ( !IsModify() )
    {
        return;
    }


}

void CIVAlarmOutDlg::CollectUserSet()
{
    if ( BST_CHECKED == m_AlarmNoHoldBt.GetCheck())
    {
        m_TmpAlarmSet.nHoldTime = 0;
    }
    else
    {
       int nSel = m_AlarmOutHoldTimeComb.GetCurSel();
       m_TmpAlarmSet.nHoldTime = s_szComboInt[nSel];
    }

    /**
    *@note 算法描述：
    * 首先通过右移可用Alarm的数目,再左移回来m，从而将需要设置的位清除为0
    * 然后在for循环中对对应位赋值
    */
    WORD& nTable = m_TmpAlarmSet.table.nTable;
    nTable >>= Alarm_Check_Num;
    nTable <<= Alarm_Check_Num;
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        nTable |= (m_AlarmCheck[i].GetCheck() << i);
    }
}

BOOL CIVAlarmOutDlg::IsModify()
{
    if ( m_pCurentAlarmSet == NULL )
    {
        ASSERT(FALSE);
        return FALSE;
    }

    return 0 == memcmp(
        &m_TmpAlarmSet,
        m_pCurentAlarmSet,
        sizeof(AlarmOutSettings));
}

void CIVAlarmOutDlg::OnRuleRemove( int nChannelID, const char* pIdentityID )
{
    if ( m_ClickItem == OnDeleteCameraTreeItem(
             m_CameraTree,nChannelID,(const void*)pIdentityID) )
    {
        Enable(FALSE);
    }
}

void CIVAlarmOutDlg::OnRuleAdd( int nChannelID, const char* pIdentityID )
{
    OnAddCameraTreeItem(
        m_CameraTree,
        nChannelID,
        (const void*)pIdentityID);
}

void CIVAlarmOutDlg::OnUseIV( int nChannelID, BOOL bEnbale )
{

}

//
// ************************ Menu *****************************
// {

void CIVAlarmOutDlg::OnAlarmFull()
{
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        m_AlarmCheck[i].SetCheck(BST_CHECKED);
    }
}

void CIVAlarmOutDlg::OnAlarmEmpty()
{
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        m_AlarmCheck[i].SetCheck(BST_UNCHECKED);
    }
}

void CIVAlarmOutDlg::OnAlarmCopy()
{
    // TODO: Add your command handler code here
}

void CIVAlarmOutDlg::OnAlarmPaste()
{
    // TODO: Add your command handler code here
}

void CIVAlarmOutDlg::OnAlarmUsetoall()
{
    // TODO: Add your command handler code here
}

// }
// Menu


// End of file


