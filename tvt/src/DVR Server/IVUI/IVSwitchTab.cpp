// IVSwitchTab.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSwitchTab.h"

enum
{
    // Tab Button
    BT_Width = 80,
    BT_Height = 40,
    BT_Start_X = 60,
    BT_Start_Y = 40,

    // Tab Static Group
    IV_TAB_Group_X = 20,
    IV_TAB_Group_Y = 10,
    IV_TAB_Height = BT_Height + 2*(BT_Start_Y-IV_TAB_Group_Y),
    
    // Function Dialog
    FunDlg_Start_X = 10,
    FunDlg_Start_Y = IV_TAB_Group_Y+IV_TAB_Height+5,
   /* FunDlg_Width,
    FunDlg_Height,*/

    BT_Start_ID = IDC_RULE,
};


// CIVSwitchTab dialog

IMPLEMENT_DYNAMIC(CIVSwitchTab, CDialog)

CIVSwitchTab::CIVSwitchTab(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSwitchTab::IDD, pParent)
{
    CIVCfgDoc::Init();
}

CIVSwitchTab::~CIVSwitchTab()
{
    CIVCfgDoc::Unit();
}

void CIVSwitchTab::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RULE, m_TabBt[0]);
    DDX_Control(pDX, IDC_ALARMOUT, m_TabBt[1]);
    DDX_Control(pDX, IDC_SCHUDULE, m_TabBt[2]);
    DDX_Control(pDX, IDC_IV_Tab_Group, m_IVTabGroup);
}


BEGIN_MESSAGE_MAP(CIVSwitchTab, CDialog)
    ON_COMMAND_RANGE(IDC_RULE, IDC_RULE+TAB_BT_NUM, &CIVSwitchTab::ClickTabBt)
    //ON_BN_CLICKED(IDC_RULE, &CIVSwitchTab::OnBnClickedRule)
    ON_WM_CLOSE()
END_MESSAGE_MAP()
// CIVSwitchTab message handlers


BOOL CIVSwitchTab::Init(CWnd* pWnd, const CRect& rect)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());

    Create(CIVSwitchTab::IDD, pWnd);

    m_IVTabGroup.MoveWindow(
        IV_TAB_Group_X,
        IV_TAB_Group_Y,
        rect.Width()-2*IV_TAB_Group_X,
        IV_TAB_Height );
    
    int nDistanceBt = (rect.Width() - 2*BT_Start_X-BT_Width)/(TAB_BT_NUM-1);	
    m_TabBt[0].SetCheck(BST_CHECKED);
    for (int i = 0; i<TAB_BT_NUM; ++i)
    {
        int nOffset = i*nDistanceBt;
        m_TabBt[i].MoveWindow(
            CRect(BT_Start_X+nOffset, BT_Start_Y, BT_Start_X+BT_Width+nOffset,BT_Start_Y+BT_Height));
    }

    CRect FuncRect(FunDlg_Start_X, FunDlg_Start_Y, rect.right-FunDlg_Start_X, rect.bottom-FunDlg_Start_X);
    m_IVSchuduleDlg.Init(this, FuncRect);
    m_IVSchuduleDlg.ShowWindow(SW_HIDE);

    m_IVAlarmOutDlg.Init(this, FuncRect);
    m_IVAlarmOutDlg.ShowWindow(SW_HIDE);

    m_IVSchuduleDlg.MoveWindow(&FuncRect, FALSE);
    //m_IVAlarmOutDlg.MoveWindow(&FuncRect, FALSE);

    m_IVRuleDlg.Init(this, FuncRect);
    m_IVRuleDlg.ShowWindow(SW_SHOW);
    //m_IVRuleDlg.MoveWindow(&FuncRect);

    MoveWindow(rect);
    ShowWindow(SW_SHOW);
    return TRUE;
}

void CIVSwitchTab::ClickTabBt( UINT ID/*, WPARAM w, LPARAM l*/ )
{
    int j = ID-BT_Start_ID;
    for (int i=0; i<TAB_BT_NUM; ++i)
    {
        if ( i != j )   
        {
             m_TabBt[i].SetCheck(BST_UNCHECKED);
        }
    }
    TRACE("%d", ID);	
    m_TabBt[j].SetCheck(BST_CHECKED);
    switch (j)
    {
    case 0:
        m_IVRuleDlg.ShowWindow(SW_SHOW);
        m_IVSchuduleDlg.ShowWindow(SW_HIDE);
        m_IVAlarmOutDlg.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_IVRuleDlg.ShowWindow(SW_HIDE);
        m_IVSchuduleDlg.ShowWindow(SW_HIDE);
        m_IVAlarmOutDlg.ShowWindow(SW_SHOW);
        break;
    case 2:
        m_IVRuleDlg.ShowWindow(SW_HIDE);
        m_IVSchuduleDlg.ShowWindow(SW_SHOW);
        m_IVAlarmOutDlg.ShowWindow(SW_HIDE);
        break;
    default:
        // log.. err
        break;
    }
}

BOOL CIVSwitchTab::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CIVSwitchTab::OnBnClickedRule()
{
    // TODO: Add your control notification handler code here
}


CIVSwitchTab* g_pIVSwitchTab = NULL;
IIVDeviceBase2* g_IIVDeviceBase2 =NULL;

#undef new
#define new new

CDialog* CreateIVConfigDlg(CWnd* pWnd, const CRect& rect)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    if ( g_pIVSwitchTab == NULL )
    {
        g_pIVSwitchTab = new CIVSwitchTab();
    }
    
    g_pIVSwitchTab->Init(pWnd, rect);
    return g_pIVSwitchTab;
}

void ReleaseIVConfigDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    safeDelete(g_pIVSwitchTab);   
}

void SetIVOpeator( IIVDeviceBase2* p )
{
    g_IIVDeviceBase2 = p;
}

void CIVSwitchTab::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    
    CDialog::OnClose();
}
