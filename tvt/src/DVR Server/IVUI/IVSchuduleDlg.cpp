/**CPP*************************************************************************
 File            : IVSchuduleDlg.cpp
 Subsystem       : 
 Function Name(s): CIVSchuduleDlg
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
#include "IVSchuduleDlg.h"


// CIVSchuduleDlg dialog

IMPLEMENT_DYNAMIC(CIVSchuduleDlg, CDialog)

CIVSchuduleDlg::CIVSchuduleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSchuduleDlg::IDD, pParent)
    , m_nCurrentChan(0)
    , m_ClickItem(NULL)
    , m_bIsCopy(FALSE)
    , m_pScheduleSettings(NULL)
{
    CScheduleCtrl::InitCursor();
    CIVCfgDoc::RegisterRuleTrigger(this);
}

CIVSchuduleDlg::~CIVSchuduleDlg()
{
}

void CIVSchuduleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SCHUDULE_CAMERA_TREE, m_CameraTree);
    DDX_Control(pDX, IDC_Add_CHECK, m_AddCheck);
    DDX_Control(pDX, IDC_Erase_CHECK, m_EraseCheck);
    DDX_Control(pDX, IDC_Schudule_Group, m_TreeGroup);
    DDX_Control(pDX, IDC_Apply_BT, m_ApplyBT);
}

void CIVSchuduleDlg::OnBnClickedAddCheck()
{
    if ( m_AddCheck.GetCheck() == BST_UNCHECKED )
    {
        m_AddCheck.SetCheck(BST_CHECKED);
        return;
    }

    m_AddCheck.SetCheck(BST_CHECKED);
    m_EraseCheck.SetCheck(BST_UNCHECKED);
    for ( int i = 0; i< Week_Day; ++i )
    {
        m_ScheduleCtrl[i].SetOperateMode(true);
    }  
}

void CIVSchuduleDlg::OnBnClickedEraseCheck()
{
    if ( m_EraseCheck.GetCheck() == BST_UNCHECKED )
    {
        m_EraseCheck.SetCheck(BST_CHECKED);
        return;
    }

    m_AddCheck.SetCheck(BST_UNCHECKED);
    m_EraseCheck.SetCheck(BST_CHECKED);
    for ( int i = 0; i< Week_Day; ++i )
    {
        m_ScheduleCtrl[i].SetOperateMode(false);
    }  
}

void CIVSchuduleDlg::OnDestroy()
{
    CameraTreeUtil::UnitCameraTree(m_CameraTree);
    __super::OnDestroy();
}

void CIVSchuduleDlg::OnNMRclickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::PopUpCameraMemu(
        m_CameraTree, 
        2, this, this);
}

void CIVSchuduleDlg::OnNMClickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::SendClickCameraTreeMes(m_CameraTree, this);
}

void CIVSchuduleDlg::OnBnClickedApplyBt()
{
    /** 
    *@Note 1. Check ScheduleCtrl Is Modify
    */
    BOOL bModified = FALSE;
    for (int i = 0; i< Week_Day; ++i)
    {
        if ( m_ScheduleCtrl[i].IsModified() )
        {
            bModified = TRUE;
            break;
        }
    }
    if ( !bModified )
    {
        return;
    }

    /** 
    *@Note 2. Compare Data Is Modify
    */
    ScheduleSettings TmpSchedule;
    CollectUserSet(TmpSchedule);
    if ( memcmp(
        m_pScheduleSettings,
        &TmpSchedule, 
        sizeof(ScheduleSettings)) == 0 )
    {
        return;
    }

    /** 
    *@Note 3. Set ScheduleCtrl is Not be Modify
    */
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].ResetModifyFlag();
    }
    
    /** 
    *@Note 4. Update Data
    */
    *m_pScheduleSettings = TmpSchedule;
    CIVScheduleCfgDoc::UpdateSchedule(
        *m_pScheduleSettings,
        m_ClickItem);
}

BEGIN_MESSAGE_MAP(CIVSchuduleDlg, CDialog)
    ON_NOTIFY(NM_RCLICK, IDC_SCHUDULE_CAMERA_TREE, &CIVSchuduleDlg::OnNMRclickSchuduleCameraTree)
    ON_BN_CLICKED(IDC_Add_CHECK, &CIVSchuduleDlg::OnBnClickedAddCheck)
    ON_BN_CLICKED(IDC_Erase_CHECK, &CIVSchuduleDlg::OnBnClickedEraseCheck)
    ON_WM_DESTROY()
    ON_NOTIFY(NM_CLICK, IDC_SCHUDULE_CAMERA_TREE, &CIVSchuduleDlg::OnNMClickSchuduleCameraTree)
    ON_BN_CLICKED(IDC_Apply_BT, &CIVSchuduleDlg::OnBnClickedApplyBt)
    ON_COMMAND(ID_SCHUDULE_FULL, &CIVSchuduleDlg::OnSchuduleFull)
    ON_COMMAND(ID_SCHUDULE_EMPTY, &CIVSchuduleDlg::OnSchuduleEmpty)
    ON_COMMAND(ID_SCHUDULE_COPY, &CIVSchuduleDlg::OnSchuduleCopy)
    ON_COMMAND(ID_SCHUDULE_PASTE, &CIVSchuduleDlg::OnSchudulePaste)
    ON_COMMAND(ID_SCHUDULE_USETOALL, &CIVSchuduleDlg::OnSchuduleUsetoall)
END_MESSAGE_MAP()


// CIVSchuduleDlg message handlers

BOOL CIVSchuduleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString strTmp;
    strTmp.LoadString(g_hmodule, IDS_Schedule_Add);
    m_AddCheck.SetWindowText(strTmp);
    strTmp.LoadString(g_hmodule, IDS_Schedule_Erase);
    m_EraseCheck.SetWindowText(strTmp);
   
    strTmp.LoadString(g_hmodule, IDS_Schedule_Tree_Group);
    m_TreeGroup.SetWindowText(strTmp);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVSchuduleDlg::Init( CWnd* pWnd, const CRect& Rect)
{
    int nHeight = Rect.Height();
    int nWidth = Rect.Width();
    
    /** 
    *@Note 1. Init Himself and CameraTree
    */
    Create(CIVSchuduleDlg::IDD, pWnd);
    MoveWindow(Rect);
    CameraTreeUtil::InitCameraTree(m_CameraTree, this, m_TreeGroup, 0, nHeight);
    
    /** 
    *@Note 2. Init ScheduleCtrl
    */
    int nSchCtrlWidth = nWidth-Week_Ctrl_X_Offset-CameraCtrl_Width;

    int nMed = nWidth - nSchCtrlWidth/2;
    m_AddCheck.MoveWindow(nMed-BT_X_Offset-BT_Width, BT_Start_Y, BT_Width, BT_Height);
    m_EraseCheck.MoveWindow(nMed+BT_X_Offset,BT_Start_Y,BT_Width, BT_Height);
    m_AddCheck.SetCheck(BST_CHECKED);

    int nAllSchHeight = nHeight-(BT_Start_Y*4+ 2*BT_Height);
    int nSchCtrlHeight = (nAllSchHeight-(Week_Day-1)*Week_Ctrl_Between_Off)/Week_Day;
    
    int nSchCtrlRight = nWidth-Week_Ctrl_X_Offset;
    int nYMove = Week_Ctrl_Between_Off+nSchCtrlHeight;
    CString strName;
    CRect TmpRect(
        CameraCtrl_Width+Week_Ctrl_X_Offset, 
        BT_Start_Y*2+ BT_Height,
        nSchCtrlRight,
        BT_Start_Y*2+ BT_Height+nSchCtrlHeight );
    for ( int i = 0; i< Week_Day; ++i )
    {
        m_ScheduleCtrl[i].Create(
            NULL, NULL, WS_CHILD|WS_VISIBLE, 
            TmpRect,
            this, Ctrl_ID_Start+i );
        strName.LoadString(Week_Ctrl_Name_ID+i);
        m_ScheduleCtrl[i].SetTitle(strName);
        
        TmpRect.top += nYMove;
        TmpRect.bottom += nYMove;
    }  

    m_ApplyBT.MoveWindow(
        nWidth-ApplyBT_X_ROffset,
        nHeight-ApplyBT_Y_ROffset,
        ApplyBT_Width,
        ApplyBT_Height);
   
    Enable(FALSE);
    return TRUE;
}

void CIVSchuduleDlg::OnInitCameraTree(
    int nChannelID,
    HTREEITEM Item )
{
    CIVCfgDoc::OnInitCameraTree(nChannelID,Item);
}

void CIVSchuduleDlg::OnUpdateMemu(
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
    switch ( Which )
    {
    case IV_Tree_Root:
    case IV_Tree_Camera:
        Enable(FALSE);
        break;
    case IV_Tree_Rule:
    {
        Enable(TRUE);
        UpdateSchudule();
        DWORD dwState =  m_bIsCopy ? Menu_Enbale : Menu_Disbale;
        pMenu->EnableMenuItem(ID_Alarm_PASTE, dwState);
        pMenu->EnableMenuItem(ID_Alarm_USETOALL, dwState);
        break;
    }
    default:
        ASSERT(FALSE);
        Enable(FALSE);
        return;
    }
}

void CIVSchuduleDlg::OnClickCameraTree(
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
    switch ( Which )
    {
    case IV_Tree_Root:
    case IV_Tree_Camera:
        Enable(FALSE);
        break;
    case IV_Tree_Rule:
        Enable(TRUE);
        UpdateSchudule();
        break;
    default:
        ASSERT(FALSE);
        Enable(FALSE);
        return;
    }
}

void CIVSchuduleDlg::Enable( BOOL bEnable /*= TRUE*/ )
{
    m_AddCheck.EnableWindow(bEnable);
    m_EraseCheck.EnableWindow(bEnable);
    for (int i=0;i<Week_Day;++i)
    {
        m_ScheduleCtrl[i].EnableWindow(bEnable);
    }
    m_ApplyBT.EnableWindow(bEnable);
}

void CIVSchuduleDlg::UpdateSchudule()
{
    m_pScheduleSettings = CIVScheduleCfgDoc::GetSchedule(m_ClickItem);
    ASSERT(m_pScheduleSettings);
    UpdateUI(*m_pScheduleSettings);
}

void CIVSchuduleDlg::UpdateUI( const ScheduleSettings& Sch )
{
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].SetTimeSec(&Sch.s[i]);
    }
}

void CIVSchuduleDlg::CollectUserSet(ScheduleSettings& TmpSchedule)
{
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].GetTimeSec(&TmpSchedule.s[i]);
    }
}

void CIVSchuduleDlg::OnRuleRemove( int nChannelID, const char* pIdentityID )
{
    if ( m_ClickItem == CameraTreeUtil::OnDeleteCameraTreeItem(
        m_CameraTree,nChannelID,(const void*)pIdentityID))
    {
        Enable(FALSE);
    }
}

void CIVSchuduleDlg::OnRuleAdd( int nChannelID, const char* pIdentityID )
{
    CameraTreeUtil::OnAddCameraTreeItem(
        m_CameraTree,
        nChannelID,
        (const void*)pIdentityID);
}

void CIVSchuduleDlg::OnUseIV( int nChannelID, BOOL bEnbale )
{

}

//
// ************************ Menu *****************************
// {

void CIVSchuduleDlg::OnSchuduleFull()
{
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].Full();
        m_ScheduleCtrl[i].RedrawWindow();
    }
}

void CIVSchuduleDlg::OnSchuduleEmpty()
{
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].Empty();
        m_ScheduleCtrl[i].RedrawWindow();
    }
}

void CIVSchuduleDlg::OnSchuduleCopy()
{
    CollectUserSet(m_CopySchedule);
    m_bIsCopy = TRUE;
}

void CIVSchuduleDlg::OnSchudulePaste()
{
    if ( !m_bIsCopy )
    {
        TRACE(_T("CIVSchuduleDlg::OnAlarmPaste\n"));
        ASSERT(FALSE);
        return;
    }

    /** 
    *@Note 1. Compare Data Is Modify
    */
    if ( memcmp(
        m_pScheduleSettings,
        &m_CopySchedule, 
        sizeof(ScheduleSettings)) == 0 )
    {
        return;
    }

    /** 
    *@Note 2. Update UI, and ScheduleCtrl is Not be Modify
    */
    UpdateUI(m_CopySchedule);
    for (int i = 0; i< Week_Day; ++i)
    {
        m_ScheduleCtrl[i].ResetModifyFlag();
    }

    /**
    *@Note 3. Update Data
    */
    *m_pScheduleSettings = m_CopySchedule;
    CIVScheduleCfgDoc::UpdateSchedule(
        *m_pScheduleSettings,
        m_ClickItem);

    /**
    *@Note 3. if Current Channel Is Use IV, Set To Device
    */
    if ( CIVCfgDoc::IsIVChannel(m_nCurrentChan) )
    {
        g_IIVDeviceBase2->ModifySchedule(
            m_nCurrentChan,
            *CIVCfgDoc::GetRuleID(m_ClickItem),
            m_CopySchedule);
    }
}

void CIVSchuduleDlg::OnSchuduleUsetoall()
{
    if ( !m_bIsCopy )
    {
        TRACE(_T("CIVAlarmOutDlg::OnSchuduleUsetoall\n"));
        ASSERT(FALSE);
        return;
    }

    SetCfgToAll(m_CopySchedule);
}


// }
// Menu




// End of file