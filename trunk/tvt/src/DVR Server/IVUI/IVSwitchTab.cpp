// IVSwitchTab.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSwitchTab.h"

enum
{
    // Tab Button
    BT_Width = 60,
    BT_Height = 30,
    BT_Start_X = 60,
    BT_Start_Y = 20,

    // Tab Static Group
    IV_TAB_Group_X = 20,
    IV_TAB_Group_Y = 10,
    IV_TAB_Height = BT_Height + 2*(BT_Start_Y-IV_TAB_Group_Y),
    
    // Function Dialog
    FunDlg_Start_X = 10,
    FunDlg_Start_Y = IV_TAB_Group_Y+IV_TAB_Height+5,
    FunDlg_Offset_Y = 10,
   /* FunDlg_Width,
    FunDlg_Height,*/

    BT_Start_ID = IDC_RULE,
};


// CIVSwitchTab dialog

IMPLEMENT_DYNAMIC(CIVSwitchTab, CDialog)

CIVSwitchTab::CIVSwitchTab(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSwitchTab::IDD, pParent)
{
    
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
    DDX_Control(pDX, IDC_IV_Tab_Func, m_FuncGroup);
}


BEGIN_MESSAGE_MAP(CIVSwitchTab, CDialog)
    ON_COMMAND_RANGE(IDC_RULE, IDC_RULE+TAB_BT_NUM, &CIVSwitchTab::ClickTabBt)
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
// CIVSwitchTab message handlers


BOOL CIVSwitchTab::Init(HWND hWnd, const CRect& rect)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CWnd* pWnd = CWnd::FromHandle(hWnd);
    Create(CIVSwitchTab::IDD, pWnd);
    ShowWindow(SW_HIDE);
    MoveWindow(rect);

    // Òþ²Øtab Group
    m_IVTabGroup.MoveWindow(
        IV_TAB_Group_X,
        IV_TAB_Group_Y,
        rect.Width()-2*IV_TAB_Group_X,
        IV_TAB_Height );
    m_IVTabGroup.ShowWindow(SW_HIDE);

    
    int nDistanceBt = (rect.Width() - 2*BT_Start_X-BT_Width)/(TAB_BT_NUM-1);	
    m_TabBt[0].SetCheck(BST_CHECKED);
    for (int i = 0; i<TAB_BT_NUM; ++i)
    {
        int nOffset = i*nDistanceBt;
        m_TabBt[i].MoveWindow(
            CRect(BT_Start_X+nOffset, BT_Start_Y, BT_Start_X+BT_Width+nOffset,BT_Start_Y+BT_Height));
    }

    CRect FuncRect(
        FunDlg_Start_X, 
        FunDlg_Start_Y, 
        rect.Width()-FunDlg_Start_X,
        rect.Height()-FunDlg_Offset_Y);
    m_IVSchuduleDlg.Init(this, FuncRect);
    m_IVSchuduleDlg.ShowWindow(SW_HIDE);

    m_IVAlarmOutDlg.Init(this, FuncRect);
    m_IVAlarmOutDlg.ShowWindow(SW_HIDE);

    m_IVRuleDlg.Init(this, FuncRect);
    m_IVRuleDlg.ShowWindow(SW_SHOW);
    //m_IVRuleDlg.MoveWindow(&FuncRect);

    // ÔÝÊ±²»Òª
    //FuncRect.left-=5;
    //FuncRect.right+=5;
    //FuncRect.top-=5;
    //FuncRect.bottom+=5;
    //m_FuncGroup.MoveWindow(FuncRect);
    m_FuncGroup.ShowWindow(SW_HIDE); 
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
    //TRACE(_T("%d"), ID);	
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
//AfxMessageBox(_T("aaaa"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CIVSwitchTab::OnClose()
{
    // TODO: Add your message handler code here and/or call default

    CDialog::OnClose();
}

void CIVSwitchTab::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    m_IVRuleDlg.OnShowWindow(bShow);
}


CIVSwitchTab* g_pIVSwitchTab = NULL;
IIVDeviceBase2* g_IIVDeviceBase2 =NULL;

#undef new
#define new new

namespace IVUIFactory
{
void InitIVConfig(
    IIVDeviceBase2* p,
    BOOL bTelphone,
    DWORD dwRelayCount)
{
    g_IIVDeviceBase2 = p;

    size_t nChannelNumByDevice;
    p->GetDeviceInfo(
        &CIVCfgDoc::s_nDeviceNum,
        &nChannelNumByDevice);
    CIVCfgDoc::s_nMaxChannel = CIVCfgDoc::s_nDeviceNum*nChannelNumByDevice;
    p->GetIVDeviceInfo(
        &CIVCfgDoc::s_nIVChannelNumByDevice, 
        &CIVCfgDoc::s_nMaxRuleNumByIVChannel);

    CIVCfgDoc::s_dwRelayCount = dwRelayCount;
    CIVCfgDoc::s_bTelphone = bTelphone;

    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CIVCfgDoc::Init();
}

void UnitIVConfig()
{
    CIVCfgDoc::Unit();
}

HWND CreateIVConfigDlg(HWND hWnd, const RECT& rect)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    if ( g_pIVSwitchTab == NULL )
    {
        g_pIVSwitchTab = new CIVSwitchTab();
    }

    g_pIVSwitchTab->Init(hWnd, rect);
    return g_pIVSwitchTab->m_hWnd;
}

void ReleaseIVConfigDlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    safeDelete(g_pIVSwitchTab);   
}

BOOL UseIV(int nChannelID, bool bEnable)
{
    return CIVRuleCfgDoc::Use(nChannelID, bEnable);
}

IVUI_API void SetChannelName( 
    int nChannelID,
    const char* pChannelName )
{
    return CIVCfgDoc::SetChannelName(
        nChannelID, pChannelName);
}

}

