/*H***************************************************************************
 File            : RuleMainBaseDlg.cpp
 Subsystem       : 
 Function Name(s): CRuleMainBaseDlg
 Author          : YiQiu
 Date            : 2010-2-24  
 Time            : 14:54
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "IVUI.h"
#include "RuleMainBaseDlg.h"


// CRuleAddMainDlg dialog

IMPLEMENT_DYNAMIC(CRuleMainBaseDlg, CDialog)

CRuleMainBaseDlg::CRuleMainBaseDlg(CWnd* pParent)
	: CDialog(CRuleMainBaseDlg::IDD, pParent)
    , m_LineDrawer(NULL)
    , m_RectangleDrawer(NULL)
    , m_PolygonDrawer(NULL)
    , m_bUse(FALSE)
{
     m_pDrawContainer = Windows::CreateDrawContainer();
     m_LineDrawer = m_pDrawContainer->Add(Windows::IDrawer_ArrowLine);
     m_RectangleDrawer = m_pDrawContainer->Add(Windows::IDrawer_Rectangle);
     m_PolygonDrawer = m_pDrawContainer->Add(Windows::IDrawer_Polygon);
}

CRuleMainBaseDlg::~CRuleMainBaseDlg()
{
    Windows::DestoryDrawContainer(m_pDrawContainer);
}

void CRuleMainBaseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_Line_Check, m_LineBT);
    DDX_Control(pDX, IDC_Zone_Check, m_ZoneBT);
    DDX_Control(pDX, IDC_Select_Check, m_SelectBT);
    DDX_Control(pDX, IDC_Right_Check, m_LineRightBT);
    DDX_Control(pDX, IDC_Left_Check, m_LineLeftBT);
    DDX_Control(pDX, IDC_Both_Check, m_LineBothBT);
    DDX_Control(pDX, IDC_Colour_BT, m_ColourBT);
    DDX_Control(pDX, IDC_Adv_BT, m_AdvBT);
    DDX_Control(pDX, IDC_Filter_BT, m_FilterBT);
    DDX_Control(pDX, IDC_Simulation_BT, m_SimulationBT);
    DDX_Control(pDX, IDC_Rectangle_Check, m_RectangleBT);
    DDX_Control(pDX, IDC_Polygon_Check, m_PolygonBT);
    DDX_Control(pDX, IDC_Alarm_Occur_STATIC, m_AlarmOccurStatic);
}


BEGIN_MESSAGE_MAP(CRuleMainBaseDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CRuleMainBaseDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CRuleMainBaseDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_Adv_BT, &CRuleMainBaseDlg::OnBnClickedAdvBt)
    ON_BN_CLICKED(IDC_Filter_BT, &CRuleMainBaseDlg::OnBnClickedFilterBt)
    ON_BN_CLICKED(IDC_Simulation_BT, &CRuleMainBaseDlg::OnBnClickedSimulationBt)
    ON_BN_CLICKED(IDC_Line_Check, &CRuleMainBaseDlg::OnBnClickedLineCheck)
    ON_BN_CLICKED(IDC_Zone_Check, &CRuleMainBaseDlg::OnBnClickedZoneCheck)
    ON_BN_CLICKED(IDC_Select_Check, &CRuleMainBaseDlg::OnBnClickedSelectCheck)
    ON_BN_CLICKED(IDC_Right_Check, &CRuleMainBaseDlg::OnBnClickedRightCheck)
    ON_BN_CLICKED(IDC_Left_Check, &CRuleMainBaseDlg::OnBnClickedLeftCheck)
    ON_BN_CLICKED(IDC_Both_Check, &CRuleMainBaseDlg::OnBnClickedBothCheck)
    ON_BN_CLICKED(IDC_Colour_BT, &CRuleMainBaseDlg::OnBnClickedColourBt)
    ON_BN_CLICKED(IDC_Polygon_Check, &CRuleMainBaseDlg::OnBnClickedPolygonCheck)
    ON_BN_CLICKED(IDC_Rectangle_Check, &CRuleMainBaseDlg::OnBnClickedRectangleCheck)
    ON_WM_CLOSE()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CRuleAddMainDlg message handlers

BOOL CRuleMainBaseDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    
    int x = 140, y = 15;
    m_Rect.SetRect(x,y,x+352*1.4,y+288*1.4);
    /*m_AlarmOccurStatic.Create(
        _T("Alarm Occur!"),
        WS_CHILD|WS_VISIBLE, 
        CRect(120, 446, 200, 466), this);*/
    /*m_PlayerWnd.Create(
        NULL, NULL, WS_CHILD|WS_VISIBLE,
        m_Rect,
        this,
        PlayerWnd_ID );*/  
    m_Player.InitDirectDraw(
        this->m_hWnd, 352, 288, &m_Rect);
    m_Player.SetVideoPlayCallback(this);
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
 
    m_pDrawContainer->Create(
        NULL,NULL,
        WS_VISIBLE|WS_CHILD,
        m_Rect, this, DrawWnd_ID);
    OnBnClickedLineCheck();
    return TRUE;
}

void CRuleMainBaseDlg::SetComomParm(
    int nChannelID, WPG_Rule* pRule, IVRuleType type)
{
    m_nCurrentChan = nChannelID;
    m_pRule = pRule;
    m_type = type;
}

void CRuleMainBaseDlg::OnBnClickedOk()
{
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    OnOK();
}

void CRuleMainBaseDlg::OnBnClickedCancel()
{
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    OnCancel();
}

void CRuleMainBaseDlg::OnBnClickedAdvBt()
{  
    if ( m_nToolsChoose == Choose_Line )
    {
        CLineAdvDlg Dlg;
        Dlg.Init(m_pRule);
        Dlg.DoModal();
    }
    else
    {
        CZoneAdvDlg Dlg;
        Dlg.Init(m_type,m_pRule);
        Dlg.DoModal();
    }
}

void CRuleMainBaseDlg::OnBnClickedFilterBt()
{
    CFilterDlg Dlg;
    Dlg.SetComomParm(m_nCurrentChan, m_pRule);
    Dlg.DoModal();
}

void CRuleMainBaseDlg::OnBnClickedSimulationBt()
{
    // È¡Êý¾Ý
    GatherUseSet();
    m_bUse = !m_bUse;
    if ( m_bUse )
    {
        m_SimulationBT.SetWindowText(_T("Stop"));
        g_IIVDeviceBase2->Start(m_nCurrentChan, this, *m_pRule);
    }
    else
    {
        m_SimulationBT.SetWindowText(_T("Simulation"));
        g_IIVDeviceBase2->Stop(m_nCurrentChan);
    }
}

void CRuleMainBaseDlg::DrawToolChange( Windows::IDrawer* pDrawer )
{
    m_pDrawContainer->EnableToolAndNotEnableOther(pDrawer, TRUE);
}

void CRuleMainBaseDlg::OnBnClickedLineCheck()
{
    m_nToolsChoose = Choose_Line;
    m_LineBT.SetCheck(BST_CHECKED);
    m_ZoneBT.SetCheck(BST_UNCHECKED);
    m_SelectBT.SetCheck(BST_UNCHECKED);

    m_RectangleBT.ShowWindow(SW_HIDE);
    m_PolygonBT.ShowWindow(SW_HIDE);
    m_LineBothBT.SetCheck(BST_CHECKED);
    m_LineRightBT.ShowWindow(SW_SHOW);
    m_LineLeftBT.ShowWindow(SW_SHOW);
    m_LineBothBT.ShowWindow(SW_SHOW);
    
    DrawToolChange(m_LineDrawer);
}

void CRuleMainBaseDlg::OnBnClickedZoneCheck()
{
    m_nToolsChoose = Choose_Rectangle;
    m_LineBT.SetCheck(BST_UNCHECKED);
    m_ZoneBT.SetCheck(BST_CHECKED);
    m_SelectBT.SetCheck(BST_UNCHECKED);

    //m_RectangleBT.SetCheck(BST_CHECKED);
    m_RectangleBT.ShowWindow(SW_SHOW);
    m_PolygonBT.ShowWindow(SW_SHOW);
    m_LineRightBT.ShowWindow(SW_HIDE);
    m_LineLeftBT.ShowWindow(SW_HIDE);
    m_LineBothBT.ShowWindow(SW_HIDE);

    OnBnClickedRectangleCheck();
}

void CRuleMainBaseDlg::OnBnClickedSelectCheck()
{
    m_AlarmOccurStatic.OnAlarmOccur();
    //m_LineBT.SetCheck(BST_UNCHECKED);
    //m_ZoneBT.SetCheck(BST_UNCHECKED);
    //m_SelectBT.SetCheck(BST_CHECKED);
}

void CRuleMainBaseDlg::OnBnClickedRightCheck()
{
    m_LineRightBT.SetCheck(BST_CHECKED);
    m_LineLeftBT.SetCheck(BST_UNCHECKED);
    m_LineBothBT.SetCheck(BST_UNCHECKED);
    m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_Right,NULL,NULL);
}

void CRuleMainBaseDlg::OnBnClickedLeftCheck()
{
    m_LineRightBT.SetCheck(BST_UNCHECKED);
    m_LineLeftBT.SetCheck(BST_CHECKED);
    m_LineBothBT.SetCheck(BST_UNCHECKED);
    m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_Left,NULL,NULL);
}

void CRuleMainBaseDlg::OnBnClickedBothCheck()
{
    m_LineRightBT.SetCheck(BST_UNCHECKED);
    m_LineLeftBT.SetCheck(BST_UNCHECKED);
    m_LineBothBT.SetCheck(BST_CHECKED); 
    m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_All,NULL,NULL);
}

void CRuleMainBaseDlg::OnBnClickedColourBt()
{
    CColorDialog Dlg;
    if ( IDCANCEL == Dlg.DoModal() )
    {
        return;
    }  

    DWORD dwColour = Dlg.GetColor();
    if ( m_nToolsChoose == Choose_Line )
    {
        m_LineDrawer->SetLineColour(dwColour);
    }
    else if (m_nToolsChoose == Choose_Rectangle)
    {
        m_RectangleDrawer->SetLineColour(dwColour);
    }
    else if (m_nToolsChoose == Choose_Polygon)
    {
        m_PolygonDrawer->SetLineColour(dwColour);
    }
}

void CRuleMainBaseDlg::OnBnClickedPolygonCheck()
{
    m_nToolsChoose = Choose_Polygon;
    m_RectangleBT.SetCheck(BST_UNCHECKED);
    m_PolygonBT.SetCheck(BST_CHECKED);
    DrawToolChange(m_PolygonDrawer);
}

void CRuleMainBaseDlg::OnBnClickedRectangleCheck()
{
    m_nToolsChoose = Choose_Rectangle;
    m_RectangleBT.SetCheck(BST_CHECKED);
    m_PolygonBT.SetCheck(BST_UNCHECKED);
    DrawToolChange(m_RectangleDrawer);
}

void CRuleMainBaseDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnClose();
}

#define YUV422 1
BOOL CRuleMainBaseDlg::OnVideoSend( FRAMEBUFSTRUCT *bufStruct )
{
    CRect Rect = m_Rect;
    FILETIME* time = (FILETIME*)&bufStruct->localTime;
    //GetClientRect(&Rect);
    ClientToScreen(&Rect);
    m_Player.Show(&Rect, bufStruct->pBuf, YUV422, 0, time);

    g_IIVDeviceBase2->ReleaseLiveBuf(bufStruct);
    return TRUE;
}

void CRuleMainBaseDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call __super::OnPaint() for painting messages
    m_Player.ShowBack();
    m_pDrawContainer->Invalidate();
}

void CRuleMainBaseDlg::UseToolCtrlMode(ToolMode Mode)
{
    switch (Mode)
    {
    case Nothing_Mode:
        m_ZoneBT.EnableWindow(FALSE);
        m_LineBT.EnableWindow(FALSE);
        m_LineRightBT.EnableWindow(FALSE);
        m_LineLeftBT.EnableWindow(FALSE);
        m_LineBothBT.EnableWindow(FALSE);
        m_ColourBT.EnableWindow(FALSE);
        m_SelectBT.EnableWindow(FALSE);
    	break;
    case Line_Mode:
        m_ZoneBT.EnableWindow(FALSE);
    	break;
    case Zone_Mode:
        m_LineBT.EnableWindow(FALSE);
        OnBnClickedZoneCheck();
    	break;
    default:
    	break;
    }
}

BOOL CRuleMainBaseDlg::OnVideoPlay(
    HDC dc,
    const tagRECT* rect,
    const FILETIME* pTime, 
    HWND hwnd,
    int nFlag )
{
    //m_Drawer->ShowWindow(SW_HIDE);
    //m_Drawer->ShowWindow(SW_SHOW);
    IIVViewer* pViewer = IVLiveFactory::GetLiveViewer();
    pViewer->Paint(m_nCurrentChan, dc, *rect, *pTime);

    m_pDrawContainer->Invalidate();
    return TRUE;
}

void CRuleMainBaseDlg::OnAlarmCallBack(
    IVRuleType type, int nChannelID, const FILETIME* pTime )
{
    m_AlarmOccurStatic.OnAlarmOccur();

}


// End of file

