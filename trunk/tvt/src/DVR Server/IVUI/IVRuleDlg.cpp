// IVRuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVRuleDlg.h"


// CIVRuleDlg dialog

IMPLEMENT_DYNAMIC(CIVRuleDlg, CDialog)

CIVRuleDlg::CIVRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVRuleDlg::IDD, pParent)
{

}

CIVRuleDlg::~CIVRuleDlg()
{
}

void CIVRuleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RULE_CAMERA_TREE, m_CameraTree);
    DDX_Control(pDX, IDC_Rule_Group, m_RuleGroup);
}


BEGIN_MESSAGE_MAP(CIVRuleDlg, CDialog)
    ON_NOTIFY(NM_RCLICK, IDC_RULE_CAMERA_TREE, &CIVRuleDlg::OnNMRclickRuleCameraTree)
    ON_COMMAND(ID_RULE_NEWRULE, &CIVRuleDlg::OnRuleNewrule)
    ON_COMMAND(ID_RULE_ENABLEALLRULE, &CIVRuleDlg::OnRuleEnableallrule)
    ON_COMMAND(ID_RULE_DISABLEALLRULE, &CIVRuleDlg::OnRuleDisableallrule)
    ON_COMMAND(ID_RULE_USE, &CIVRuleDlg::OnRuleUse)
    ON_COMMAND(ID_RULE_SHOWOBJECT, &CIVRuleDlg::OnRuleShowobject)
    ON_COMMAND(ID_RULE_SHOWTRACE, &CIVRuleDlg::OnRuleShowtrace)
    ON_COMMAND(ID_RULE_SHOWOBJTRACE, &CIVRuleDlg::OnRuleShowobjtrace)   
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CIVRuleDlg message handlers

static double g_WndRatio = 352.0/288.0;

BOOL CIVRuleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
 
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVRuleDlg::Init( CWnd* pWnd, const CRect& Rect)
{
    Create(IDD, pWnd);
    MoveWindow(Rect);
    InitCameraTree(m_CameraTree, this, m_RuleGroup, 0,Rect.Height());

    CRect rect;
    GetClientRect(&rect);
    int nMaxWidth = rect.Width() - CameraCtrl_Width - 2*X_Offset;
    int nMaxHeight = rect.Height() - 2*Y_Offset;
    double nNowRatio = double(nMaxWidth)/nMaxHeight;

    int x = CameraCtrl_Width + X_Offset;
    int y = Y_Offset;
    int nVideoWidth, nVideoHeight;
    if ( g_WndRatio > nNowRatio )
    {
        nVideoWidth = nMaxWidth;
        nVideoHeight = int(nMaxWidth/g_WndRatio);
    }
    else
    {
        nVideoHeight = nMaxHeight;
        nVideoWidth = int(nMaxHeight*g_WndRatio);
    }

    rect.left += 5;
    rect.top +=5 ;
    m_PlayerWnd.Create(
        NULL, NULL, WS_CHILD|WS_VISIBLE,
        CRect(x,y,x+nVideoWidth,y+nVideoHeight),
        this,
        PlayerWnd_ID );
    m_Player.InitDirectDraw(
        m_PlayerWnd.m_hWnd, 352, 288);

    g_IIVDeviceBase2->RegisterLiveDataCallBack(0, this);
    return TRUE;
}

void CIVRuleDlg::OnNMRclickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    PopUpCameraMemu(m_CameraTree, 0, this, this);
}

void CIVRuleDlg::OnRuleEnableallrule()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleDisableallrule()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleUse()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleShowobject()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleShowtrace()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleShowobjtrace()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleNewrule()
{
    CIVFunctionSelDlg FunctionSelDlg;
    if ( IDCANCEL== FunctionSelDlg.DoModal() )
    {
        return;
    }
    
    CRuleAddMainDlg RuleAddMainDlg;
    RuleAddMainDlg.SetIVRuleType(FunctionSelDlg.GetUserSelect());
    RuleAddMainDlg.DoModal();
}

void CIVRuleDlg::OnUpdateMemu(
    CMenu* pMenu,
    WhichMemu Which,
    int nChannelID,
    void* pData )
{
    switch (Which)
    {
    case Root:
    	break;
    case Camera:

    	break;
    case Rule:
    	break;
    default:
    	break;
    }
}

void CIVRuleDlg::OnInitCameraTree( 
    int nChannelID,
    HTREEITEM Item )
{

}

void CIVRuleDlg::OnDestroy()
{
    g_IIVDeviceBase2->UnRegisterLiveDataCallBack(0, this);
    m_Player.UnitDirectDraw();
    UnitCameraTree(m_CameraTree);
    __super::OnDestroy(); 
}

void CIVRuleDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call __super::OnPaint() for painting messages
    m_Player.ShowBack();
}

void CIVRuleDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
   
    __super::OnClose();
}

#define YUV422 1
BOOL CIVRuleDlg::OnVideoSend( FRAMEBUFSTRUCT *bufStruct )
{
    CRect Rect;
    FILETIME* time = (FILETIME*)&bufStruct->localTime;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);
    m_Player.Show(&Rect, bufStruct->pBuf, YUV422, 0, time);

    g_IIVDeviceBase2->ReleaseLiveBuf(bufStruct);
    return TRUE;
}
    