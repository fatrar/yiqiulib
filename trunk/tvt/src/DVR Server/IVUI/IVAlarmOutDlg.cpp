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
    , m_bIsCopy(FALSE)
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
    for (int i=0;i<Alarm_Max_Check_Num;++i)
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
    DDX_Control(pDX, IDC_Alarm_Sensor_Combo, m_SensorCombo);
}

void CIVAlarmOutDlg::OnDestroy()
{
    CameraTreeUtil::UnitCameraTree(m_CameraTree);
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

void CIVAlarmOutDlg::OnNMRclickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::PopUpCameraMemu(
        m_CameraTree,
        1, this, this);
}

void CIVAlarmOutDlg::OnNMClickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::SendClickCameraTreeMes(m_CameraTree, this);
}

void CIVAlarmOutDlg::OnBnClickedApplyBt()
{
    AlarmOutSettings TmpAlarmSet;
    CollectUserSet(TmpAlarmSet);
    if ( 0 == memcmp(
          &TmpAlarmSet, m_pCurentAlarmSet, 
           sizeof(AlarmOutSettings) ) )
    {
        return;
    }

    *m_pCurentAlarmSet = TmpAlarmSet;
    CIVAlarmOutCfgDoc::UpdateAlarmOut(
        *m_pCurentAlarmSet, m_ClickItem);
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
    for (int i=Alarm_Check_Num; i<Alarm_Max_Check_Num;++i)
    {
        m_AlarmCheck[i].ShowWindow(SW_HIDE);
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
    CameraTreeUtil::InitCameraTree(
        m_CameraTree, this,
        m_TreeGroup, 0, nHeight);
    
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
        /*10**/BT_Height);

    // Init Alarm Group
    int nChooseAlarmGroupY = Alarm_Out_Y_Offset*2 + Alarm_Hold_Height;
    int nChooseAlarmGroupHeight = nHeight - (nChooseAlarmGroupY+2*Alarm_Out_Y_Offset+ApplyBT_Height);
    m_ChooseAlarmGroup.MoveWindow(
        nAlarmOutGroupX,
        nChooseAlarmGroupY,
        nAlarmOutGroupWidth,
        nChooseAlarmGroupHeight );

    int nOutBtYOffset = (nChooseAlarmGroupHeight-6*Alarm_Out_Y_Offset)/3;
    int nOutBtYStart = nChooseAlarmGroupY+2*Alarm_Out_Y_Offset;
    int nOutWidth = (nAlarmOutGroupWidth-6*Alarm_Out_X_Offset)/2;
    int nOutButtonX = nAllButtonX+2*Alarm_Out_X_Offset;
    for (int i = 0; i< Alarm_Check_Num/2; ++i)
    {
        m_AlarmCheck[i*2].MoveWindow(
            nOutButtonX,
            nOutBtYStart+i*nOutBtYOffset,
            Alarm_Check_Width,
            BT_Height );
        m_AlarmCheck[i*2+1].MoveWindow(
            nOutButtonX + nOutWidth,
            nOutBtYStart+i*nOutBtYOffset,
            Alarm_Check_Width,
            BT_Height );
        if ( i*2+1 == IDS_AlarmOut_Relay-Alarm_Out_String_Start )
        {
            m_SensorCombo.MoveWindow(
                nOutButtonX + nOutWidth + Alarm_Check_Width+Alarm_Out_X_Offset,
                nOutBtYStart+i*nOutBtYOffset,
                Combo_Width*2,
                BT_Height);
        }
    }

    if ( !s_bTelphone )
    {
        m_AlarmCheck[IDS_AlarmOut_TelphoneCall-Alarm_Out_String_Start].EnableWindow(FALSE);
    }
    if ( s_dwRelayCount == 0 )
    {
        m_AlarmCheck[IDS_AlarmOut_Relay-Alarm_Out_String_Start].EnableWindow(FALSE);
        m_SensorCombo.EnableWindow(FALSE);
    }
    else
    {
        CString strTmp;
        for (int i=0; i<s_dwRelayCount; ++i)
        {
            strTmp.Format(_T("%d"), i);
            m_SensorCombo.InsertString(i, strTmp);
        }
    }
    
    m_ApplyBT.MoveWindow(
        nWidth-ApplyBT_X_ROffset,
        nHeight-ApplyBT_Y_ROffset,
        ApplyBT_Width,
        ApplyBT_Height);
    return TRUE;
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
    if ( m_ClickItem == Item )
    {
        return;
    }

    m_ClickItem = Item;
    m_nCurrentChan = nChannelID;
    switch (Which)
    {
    case IV_Tree_Root:
    case IV_Tree_Camera:    
        Enable(FALSE);
        break;
    case IV_Tree_Rule:
    {
        Enable(TRUE);
        UpdateAlarm();
        DWORD dwState =  m_bIsCopy ? Menu_Enbale : Menu_Disbale;
        pMenu->EnableMenuItem(ID_Alarm_PASTE, dwState);
        pMenu->EnableMenuItem(ID_Alarm_USETOALL, dwState);
        break;
    }  
    default:
        ASSERT(FALSE);
        Enable(FALSE);
        break;
    }
}

void CIVAlarmOutDlg::OnClickCameraTree(
    WhichMemu Which,
    int nChannelID,
    const void* pData, 
    HTREEITEM Item )
{
    if ( m_ClickItem == Item )
    {
        return;
    }

    m_ClickItem = Item;
    m_nCurrentChan = nChannelID;
    switch (Which)
    {
    case IV_Tree_Root:
    case IV_Tree_Camera:    
        Enable(FALSE);
        break;
    case IV_Tree_Rule:
        Enable(TRUE);
        UpdateAlarm();
        break;
    default:
        ASSERT(FALSE);
        Enable(FALSE);
        break;
    }
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
    m_SensorCombo.EnableWindow(bEnable);

    if ( bEnable )
    {
        if ( !s_bTelphone )
        {
            m_AlarmCheck[IDS_AlarmOut_TelphoneCall-Alarm_Out_String_Start].EnableWindow(FALSE);
        }
        if ( s_dwRelayCount == 0 )
        {
            m_AlarmCheck[IDS_AlarmOut_Relay-Alarm_Out_String_Start].EnableWindow(FALSE);
            m_SensorCombo.EnableWindow(FALSE);
        }
    }
}

void CIVAlarmOutDlg::UpdateAlarm()
{
    m_pCurentAlarmSet = CIVAlarmOutCfgDoc::GetAlarmOut(m_ClickItem);
    ASSERT(m_pCurentAlarmSet);
    UpdateUI(*m_pCurentAlarmSet);
}

void CIVAlarmOutDlg::CollectUserSet(AlarmOutSettings& TmpAlarmSet)
{
    if ( BST_CHECKED == m_AlarmNoHoldBt.GetCheck())
    {
        TmpAlarmSet.nHoldTime = 0;
    }
    else
    {
       int nSel = m_AlarmOutHoldTimeComb.GetCurSel();
       TmpAlarmSet.nHoldTime = s_szComboInt[nSel];
    }

    /**
    *@note 算法描述：
    * 首先通过右移可用Alarm的数目,再左移回来m，从而将需要设置的位清除为0
    * 然后在for循环中对对应位赋值
    */
    DWORD& nTable = TmpAlarmSet.table.nTable;
    nTable >>= Alarm_Check_Num;
    nTable <<= Alarm_Check_Num;
    for (int i=0; i<Alarm_Check_Num; ++i)
    {
        nTable |= (m_AlarmCheck[i].GetCheck() << i);
    }

    if ( m_AlarmCheck[IDS_AlarmOut_TelphoneCall-Alarm_Out_String_Start].GetCheck() == BST_CHECKED )
    {
        unsigned short& nRelay = TmpAlarmSet.table.Table.nRelay;
        nRelay = 0;
        int nCount = m_SensorCombo.GetCount();
        for ( int i = 0; i<nCount; ++i )
        {
            nRelay |= (m_SensorCombo.GetCheck(i) << i);
        }
    }
}

void CIVAlarmOutDlg::UpdateUI( const AlarmOutSettings& Alarm )
{
    /**
    *@note 1. Update Hold Group
    */
    if ( Alarm.nHoldTime == 0 )
    {
        OnBnClickedNouseHold();
    }
    else
    {
        OnBnClickedUseHold();
        BOOL bFound = FALSE;
        int i = 0;
        for (; i< sizeof(s_szComboInt)/sizeof(int); ++i)
        {
            if ( s_szComboInt[i] == Alarm.nHoldTime )
            {
                bFound = TRUE;
                break;
            }
        }
        if ( bFound )
        {
            m_AlarmOutHoldTimeComb.SetCurSel(i);
        }
        else
        {
            // [] heliang err +
            TRACE("UpdateUI Error Not Found Combox Index!");
            m_AlarmOutHoldTimeComb.SetCurSel(0);
        }
    }

    /**
    *@note 2. Update Alarm check Group
    */
    //TRACE("Start Alarm Table \n");
    DWORD nTable = Alarm.table.nTable;
    for (int i = 0; i < Alarm_Check_Num; ++i)
    {
        register DWORD nVlaue = ( nTable & (1<<i) ) >> i;
        //TRACE("%d", nVlaue);
        m_AlarmCheck[i].SetCheck( nVlaue );
    }
    //TRACE("\nEnd Alarm Table \n");
}

void CIVAlarmOutDlg::OnRuleRemove(
    int nChannelID,
    const char* pIdentityID,
    CString& strRuleName )
{
    if ( m_ClickItem == CameraTreeUtil::OnDeleteCameraTreeItem(
             m_CameraTree,nChannelID,(const void*)pIdentityID) )
    {
        Enable(FALSE);
    }
}

void CIVAlarmOutDlg::OnRuleAdd( 
    int nChannelID,
    const char* pIdentityID,
    CString& strRuleName )
{
    CameraTreeUtil::OnAddCameraTreeItem(
        m_CameraTree,
        nChannelID,
        (const void*)pIdentityID,
        strRuleName );
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
    m_bIsCopy = TRUE;
    CollectUserSet(m_CopyAlarmSet);
}

void CIVAlarmOutDlg::OnAlarmPaste()
{
    if ( !m_bIsCopy )
    {
        TRACE("CIVAlarmOutDlg::OnAlarmPaste\n");
        ASSERT(FALSE);
        return;
    }

    /** 
    *@Note 1. Compare Data Is Modify
    */
    if ( 0 == memcmp(
        &m_CopyAlarmSet, m_pCurentAlarmSet, 
        sizeof(AlarmOutSettings) ) )
    {
        return;
    }

    /** 
    *@Note 2. Update UI
    */
    UpdateUI(m_CopyAlarmSet);

    /**
    *@Note 3. Update Data
    */
    *m_pCurentAlarmSet = m_CopyAlarmSet;
    CIVAlarmOutCfgDoc::UpdateAlarmOut(
        *m_pCurentAlarmSet, m_ClickItem);

    /**
    *@Note 3. if Current Channel Is Use IV, Set To Device
    */
    if ( CIVCfgDoc::IsIVChannel(m_nCurrentChan) )
    {
        g_IIVDeviceBase2->ModifyAlarmOut(
            m_nCurrentChan,
            *CIVCfgDoc::GetRuleID(m_ClickItem),
            m_CopyAlarmSet);
    }
}

void CIVAlarmOutDlg::OnAlarmUsetoall()
{
    if ( !m_bIsCopy )
    {
        TRACE("CIVAlarmOutDlg::OnAlarmUsetoall\n");
        ASSERT(FALSE);
        return;
    }

    SetCfgToAll(m_CopyAlarmSet);
}



// }
// Menu


// End of file


