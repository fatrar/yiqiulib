/**CPP*************************************************************************
 File            : IVRuleDlg.cpp
 Subsystem       : 
 Function Name(s): CIVRuleDlg
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
#include "IVRuleDlg.h"


// CIVRuleDlg dialog

IMPLEMENT_DYNAMIC(CIVRuleDlg, CDialog)

CIVRuleDlg::CIVRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVRuleDlg::IDD, pParent)
    , m_nCurrentChan(0)
    , m_ClickItem(NULL)
{
}

CIVRuleDlg::~CIVRuleDlg()
{
}

void CIVRuleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RULE_CAMERA_TREE, m_CameraTree);
    DDX_Control(pDX, IDC_Rule_Group, m_TreeGroup);
}

void CIVRuleDlg::OnDestroy()
{
    if ( m_nCurrentChan != Invaild_ChannelID &&
        g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }

    m_Player.UnitDirectDraw();
    CameraTreeUtil::UnitCameraTree(m_CameraTree);
    __super::OnDestroy(); 
}

void CIVRuleDlg::OnPaint()
{
    CPaintDC dc(this);
    m_Player.ShowBack();
}


void CIVRuleDlg::OnNMRclickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::PopUpCameraMemu(
        m_CameraTree, 0, 
        this, this);
}

void CIVRuleDlg::OnNMClickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    CameraTreeUtil::SendClickCameraTreeMes(
        m_CameraTree, this);
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
    ON_NOTIFY(NM_CLICK, IDC_RULE_CAMERA_TREE, &CIVRuleDlg::OnNMClickRuleCameraTree)
    ON_COMMAND(ID_RULE_DELETERULE, &CIVRuleDlg::OnRuleDeleterule)
    ON_COMMAND(ID_RULE_EDITRULE, &CIVRuleDlg::OnRuleEditrule)
    ON_COMMAND(ID_RULE_RENAMERULE, &CIVRuleDlg::OnRuleRenamerule)
    ON_COMMAND(ID_RULE_ENABLERULE, &CIVRuleDlg::OnRuleEnablerule)
    ON_COMMAND(ID_RULE_DISABLERULE, &CIVRuleDlg::OnRuleDisablerule)
END_MESSAGE_MAP()


// CIVRuleDlg message handlers

static double g_WndRatio = 352.0/288.0;

BOOL CIVRuleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString strTmp;
    strTmp.LoadString(g_hmodule, IDS_Rule_Tree_Group);
    m_TreeGroup.SetWindowText(strTmp);
 
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVRuleDlg::Init( CWnd* pWnd, const CRect& Rect)
{
    Create(IDD, pWnd);
    MoveWindow(Rect);
    CameraTreeUtil::InitCameraTree(
        m_CameraTree, this,
        m_TreeGroup, 0,
        Rect.Height() );

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

    m_PlayerWnd.Create(
        NULL, NULL, WS_CHILD|WS_VISIBLE,
        CRect(x,y,x+nVideoWidth,y+nVideoHeight),
        this, PlayerWnd_ID );
    m_Player.InitDirectDraw(
        m_PlayerWnd.m_hWnd, 352, 288);

    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    
    return TRUE;
}


void CIVRuleDlg::OnInitCameraTree( 
    int nChannelID,
    HTREEITEM Item )
{
    CIVCfgDoc::OnInitCameraTree(nChannelID,Item);
}

void CIVRuleDlg::OnUpdateMemu(
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
        m_nCurrentChan = nChannelID;
    	break;
    case IV_Tree_Camera:
        UpdateLiveChannel(nChannelID);
        UpdateCameraMenu(pMenu);
    	break;
    case IV_Tree_Rule:
        UpdateLiveChannel(nChannelID);
        UpdateRuleMenu(pMenu);
    	break;
    default:
    	break;
    }
}

void CIVRuleDlg::OnClickCameraTree(
    WhichMemu Which, 
    int nChannelID,
    const void* pData,
    HTREEITEM Item )
{
    m_ClickItem = Item;
    switch ( Which )
    {
    case IV_Tree_Camera:
    case IV_Tree_Rule:
        UpdateLiveChannel(nChannelID);
        break;
    case IV_Tree_Root:
        if ( g_IIVDeviceBase2 )
        {
            g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
        }
        m_nCurrentChan = Invaild_ChannelID;
        break;
    default:
        ASSERT(FALSE);
        return;
    }
}

BOOL CIVRuleDlg::OnVideoSend( FRAMEBUFSTRUCT *bufStruct )
{
    CRect Rect;
    FILETIME* time = (FILETIME*)&bufStruct->localTime;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);
    m_Player.Show(
        &Rect, bufStruct->pBuf,
        bufStruct->width, bufStruct->height,
        CSingleVideoPlayer::YUV422, 0, time);

    g_IIVDeviceBase2->ReleaseLiveBuf(bufStruct);
    return TRUE;
}

void CIVRuleDlg::UpdateLiveChannel(int nChannelID)
{
    if ( m_nCurrentChan == Invaild_ChannelID ||
         g_IIVDeviceBase2 == NULL )
    {
        m_nCurrentChan = nChannelID;
        return;
    }

    g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    m_nCurrentChan = nChannelID;
    g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
}

void CIVRuleDlg::UpdateCameraMenu( CMenu* pMenu )
{
    if ( CIVRuleCfgDoc::IsIVChannel(m_nCurrentChan) )
    {
        pMenu->CheckMenuItem(ID_RULE_USE, MFS_CHECKED|MF_BYCOMMAND);
    }
    else
    {
        pMenu->CheckMenuItem(ID_RULE_USE, MFS_UNCHECKED|MF_BYCOMMAND);
    }

    int nState = CIVRuleCfgDoc::GetShowState(m_nCurrentChan);
    if ( nState == IIVViewer::Show_Object )
    {
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJECT, 
            MFS_CHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWTRACE,
            MFS_UNCHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJTRACE,
            MFS_UNCHECKED|MF_BYCOMMAND);
    
    }
    else if ( nState == IIVViewer::Show_Trace )
    {
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJECT,
            MFS_UNCHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWTRACE,
            MFS_CHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJTRACE,
            MFS_UNCHECKED|MF_BYCOMMAND);
    }
    else
    {
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJECT,
            MFS_UNCHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWTRACE,
            MFS_UNCHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_SHOWOBJTRACE,
            MFS_CHECKED|MF_BYCOMMAND);
    }

}

void CIVRuleDlg::UpdateRuleMenu( CMenu* pMenu )
{
    if ( CIVRuleCfgDoc::IsRuleEnbale(m_ClickItem) )
    {
        pMenu->CheckMenuItem(
            ID_RULE_ENABLERULE,
            MFS_CHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_DISABLERULE, 
            MFS_UNCHECKED|MF_BYCOMMAND);
    }
    else
    {
        pMenu->CheckMenuItem(
            ID_RULE_ENABLERULE,
            MFS_UNCHECKED|MF_BYCOMMAND);
        pMenu->CheckMenuItem(
            ID_RULE_DISABLERULE,
            MFS_CHECKED|MF_BYCOMMAND);
    }
}

//
// ***************** IVRule Camera Menu Command *****************
// {

void CIVRuleDlg::OnRuleEnableallrule()
{
    
}

void CIVRuleDlg::OnRuleDisableallrule()
{
    // TODO: Add your command handler code here
}

void CIVRuleDlg::OnRuleUse()
{
    if ( CIVRuleCfgDoc::IsUse(m_nCurrentChan) )
    {
        CIVRuleCfgDoc::Use(m_nCurrentChan,false); 
    }
    else
    {
        if ( g_IIVDeviceBase2->IsHaveFreeDevice() )
        {
            CIVRuleCfgDoc::Use(m_nCurrentChan,true);
        }
        else
        {
            MessageBox(
                _T("Is not enought IV Channel, You Must Stop Other IV Channel First!"));
        }
    }
}

void CIVRuleDlg::OnRuleShowobject()
{
    CIVRuleCfgDoc::SetShowState(
        m_nCurrentChan, IIVViewer::Show_Object);
    IVLiveFactory::GetLiveViewer()->SetDataShowState(
        m_nCurrentChan, IIVViewer::Show_Object);
}

void CIVRuleDlg::OnRuleShowtrace()
{
    CIVRuleCfgDoc::SetShowState(
        m_nCurrentChan, IIVViewer::Show_Trace);
    IVLiveFactory::GetLiveViewer()->SetDataShowState(
        m_nCurrentChan, IIVViewer::Show_Trace);
}

void CIVRuleDlg::OnRuleShowobjtrace()
{
    CIVRuleCfgDoc::SetShowState(
        m_nCurrentChan, IIVViewer::Show_Object_Trace);
    IVLiveFactory::GetLiveViewer()->SetDataShowState(
        m_nCurrentChan, IIVViewer::Show_Object_Trace);
}

void CIVRuleDlg::OnRuleNewrule()
{
    CIVFunctionSelDlg FunctionSelDlg;
    if ( m_nCurrentChan == Invaild_ChannelID || 
         m_ClickItem == NULL || 
         IDCANCEL== FunctionSelDlg.DoModal() )
    {
        return;
    }

    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }

    WPG_Rule* pRule = new WPG_Rule;
    IVRuleType RuleType = FunctionSelDlg.GetUserSelect();
    IVUtil::InitWPGRuleByType(pRule, RuleType);

    CRuleMainBaseDlg* pDlg = CreateRuleCfgDlgByRule(RuleType, this);
    pDlg->SetComomParm(m_nCurrentChan, pRule, RuleType);
    if ( IDOK == pDlg->DoModal() )
    {
        /**
        *@note  1. Operator Device
        */
        if ( g_IIVDeviceBase2->IsUse(m_nCurrentChan) )
            g_IIVDeviceBase2->Add(m_nCurrentChan, *pRule);

        /**
        *@note 2. Save To XML and Memory, and update Tree
        */
        CIVRuleCfgDoc::AddRule(
            *pRule,
            m_ClickItem);    
    }
    else
    {
        delete pRule;
    }

    delete pDlg;
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
}

// }
// IVRule Camera Menu Command


//
// ***************** IVRule Rule Menu Command *****************
// {

void CIVRuleDlg::OnRuleDeleterule()
{
    CIVRuleCfgDoc::RemoveRule(m_ClickItem);
}

void CIVRuleDlg::OnRuleEditrule()
{
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }

    WPG_Rule* pRule = CIVRuleCfgDoc::GetRule(m_ClickItem);
    IV_RuleID& RuleID = (IV_RuleID&)(pRule->ruleId);
    IVRuleType RuleType = RuleID.RuleID.nType;
    CRuleMainBaseDlg* pDlg = CreateRuleCfgDlgByRule(RuleType, this);
    pDlg->SetComomParm(m_nCurrentChan, pRule, RuleType, TRUE);
    if ( IDOK == pDlg->DoModal() )
    {
        /**
        *@note  1. Operator Device
        */
        if ( g_IIVDeviceBase2->IsUse(m_nCurrentChan) )
            g_IIVDeviceBase2->ModifyRule(m_nCurrentChan, *pRule);

        /**
        *@note 2. Update To XML and Memory
        */
        CIVRuleCfgDoc::UpdateRule(
            *pRule, m_ClickItem);    
    }
    else {}
    delete pDlg;

    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
}

void CIVRuleDlg::OnRuleRenamerule()
{
    // [] 
    m_CameraTree.EditLabel(m_ClickItem);
}

void CIVRuleDlg::OnRuleEnablerule()
{
    CIVRuleCfgDoc::EnableRule(m_ClickItem, TRUE);
}

void CIVRuleDlg::OnRuleDisablerule()
{
    CIVRuleCfgDoc::EnableRule(m_ClickItem, FALSE);
}

// }
// IVRule Rule Menu Command

// End of file

