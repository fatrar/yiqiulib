// IVSchuduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSchuduleDlg.h"


// CIVSchuduleDlg dialog

IMPLEMENT_DYNAMIC(CIVSchuduleDlg, CDialog)

CIVSchuduleDlg::CIVSchuduleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSchuduleDlg::IDD, pParent)
    , m_nCurrentChan(0)
    , m_ClickItem(NULL)
{
     CScheduleCtrl::InitCursor();
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
    DDX_Control(pDX, IDC_Schudule_Group, m_SchuduleGroup);
    DDX_Control(pDX, IDC_Apply_BT, m_ApplyBT);
}


BEGIN_MESSAGE_MAP(CIVSchuduleDlg, CDialog)
    ON_NOTIFY(NM_RCLICK, IDC_SCHUDULE_CAMERA_TREE, &CIVSchuduleDlg::OnNMRclickSchuduleCameraTree)
    ON_BN_CLICKED(IDC_Add_CHECK, &CIVSchuduleDlg::OnBnClickedAddCheck)
    ON_BN_CLICKED(IDC_Erase_CHECK, &CIVSchuduleDlg::OnBnClickedEraseCheck)
    ON_WM_DESTROY()
    ON_NOTIFY(NM_CLICK, IDC_SCHUDULE_CAMERA_TREE, &CIVSchuduleDlg::OnNMClickSchuduleCameraTree)
    ON_BN_CLICKED(IDC_Apply_BT, &CIVSchuduleDlg::OnBnClickedApplyBt)
END_MESSAGE_MAP()


// CIVSchuduleDlg message handlers

BOOL CIVSchuduleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    Enable(FALSE);

    CString strTmp;
    strTmp.LoadString(g_hmodule, IDS_Schedule_Add);
    m_AddCheck.SetWindowText(strTmp);
    strTmp.LoadString(g_hmodule, IDS_Schedule_Erase);
    m_EraseCheck.SetWindowText(strTmp);
        
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVSchuduleDlg::Init( CWnd* pWnd, const CRect& Rect)
{
    int nHeight = Rect.Height();
    int nWidth = Rect.Width();
    //
    // 1. Init Himself and CameraTree
    //
    Create(CIVSchuduleDlg::IDD, pWnd);
    MoveWindow(Rect);
    InitCameraTree(m_CameraTree, this, m_SchuduleGroup, 0, nHeight);
    
    //
    // 2. Init ScheduleCtrl
    //
    int nSchCtrlWidth = nWidth-Week_Ctrl_X_Offset-CameraCtrl_Width;

    int nMed = nWidth - nSchCtrlWidth/2;
    m_AddCheck.MoveWindow(nMed-BT_X_Offset-BT_Width, BT_Start_Y, BT_Width, BT_Height);
    m_EraseCheck.MoveWindow(nMed+BT_X_Offset,BT_Start_Y,BT_Width, BT_Height);
    m_AddCheck.SetCheck(BST_CHECKED);

    int nAllSchHeight = nHeight-(BT_Start_Y*3+ BT_Height);
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
   
    return TRUE;
}

void CIVSchuduleDlg::OnNMRclickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    PopUpCameraMemu(m_CameraTree, 2, this, this);
}

void CIVSchuduleDlg::OnNMClickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    HTREEITEM hItem = GetTreeClickItem(m_CameraTree);
    if ( NULL == hItem )
    {
        return;
    }

    m_ClickItem = hItem;
    ItemAttribute* pInfo = (ItemAttribute*)m_CameraTree.GetItemData(hItem);
    switch ( pInfo->Info.Which )
    {
    case IUpdateMemu::Root:
    case IUpdateMemu::Camera:
        Enable(FALSE);
        break;
    case IUpdateMemu::Rule:
        Enable(TRUE);
        UpdateChannel(pInfo->Info.nChannelID);
        break;
    default:
        ASSERT(FALSE);
        Enable(FALSE);
        return;
    }
}

void CIVSchuduleDlg::OnInitCameraTree(
    int nChannelID,
    HTREEITEM Item )
{

}

void CIVSchuduleDlg::OnUpdateMemu(
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

void CIVSchuduleDlg::OnClickCameraTree( 
    WhichMemu Which,
    int nChannelID, 
    const void* pData, 
    HTREEITEM Item )
{

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
    UnitCameraTree(m_CameraTree);
    __super::OnDestroy();
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

void CIVSchuduleDlg::UpdateChannel( int nChannelID )
{
    
}

void CIVSchuduleDlg::OnBnClickedApplyBt()
{
   
}

void CIVSchuduleDlg::CollectUserSet()
{

}

void CIVSchuduleDlg::OnRuleRemove( int nChannelID, const char* pIdentityID )
{

}

void CIVSchuduleDlg::OnRuleAdd( int nChannelID, const char* pIdentityID )
{

}

void CIVSchuduleDlg::OnUseIV( int nChannelID, BOOL bEnbale )
{

}