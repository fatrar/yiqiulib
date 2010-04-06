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
    , m_EditMode(FALSE)
    , m_bUse(TRUE)
    , m_ChangeCursor(FALSE)
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
    DDX_Control(pDX, IDOK, m_OKBt);
    DDX_Control(pDX, IDCANCEL, m_CancelBt);
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
    ON_WM_TIMER()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CRuleAddMainDlg message handlers

BOOL CRuleMainBaseDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    int x = 140, y = 15;
    m_Rect.SetRect(x,y,int(x+352*1.4),int(y+288*1.4)); 
    m_Player.InitDirectDraw(
        this->m_hWnd, 352, 288, &m_Rect);
    m_Player.SetVideoPlayCallback(this, m_nCurrentChan);
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
 
    m_pDrawContainer->Create(
        NULL,NULL,
        WS_VISIBLE|WS_CHILD,
        m_Rect, this, DrawWnd_ID);
    CRect TmpRect;
    m_pDrawContainer->GetClientRect(&TmpRect);
    if ( m_EditMode )
    {
        CPoint* pPointList = NULL;
        IVUtil::UserToolsChoose Choose = IVUtil::GetDrawToolsByRule(*m_pRule, m_type);
        switch (Choose)
        {
        case IVUtil::Choose_Line:
        {   
            WPG_TripwireEventDescription& des = m_pRule->ruleDescription.description.tripwireEventDescription;
            CPoint szPointBuf[2];
            IVUtil::WPGTripwireToPointList(des, TmpRect, szPointBuf);
            m_LineDrawer->SetDefault(szPointBuf, 2);
            OnBnClickedLineCheck();
            if ( des.direction == ANY_DIRECTION )
            {
                m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_All);
                OnBnClickedBothCheck();
            }
            else if ( des.direction == LEFT_TO_RIGHT )
            {
                m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_Left);
                OnBnClickedLeftCheck();
            }
            else if ( des.direction == RIGHT_TO_LEFT )
            {
                m_LineDrawer->SendCommond(Windows::IDrawer::Line_Show_Right);
                OnBnClickedRightCheck();
            }
            else{}
        	break;
        }
        case IVUtil::Choose_Rectangle:
        {
            size_t nCount;
            pPointList = IVUtil::WPGPolygonToPointList(
                m_pRule->ruleDescription.description.aoiEventDescription.polygon,
                TmpRect, nCount );
            m_RectangleDrawer->SetDefault(pPointList, nCount);
            OnBnClickedZoneCheck();
            OnBnClickedRectangleCheck();
            break;
        }    
        case IVUtil::Choose_Polygon:
        {    
            size_t nCount;
            pPointList = IVUtil::WPGPolygonToPointList(
            m_pRule->ruleDescription.description.aoiEventDescription.polygon,
            TmpRect, nCount );
            m_PolygonDrawer->SetDefault(pPointList, nCount);
            OnBnClickedZoneCheck();
            OnBnClickedPolygonCheck();
    	    break;
        }
        default:
        	break;
        }
    }
    else{}
    
    return TRUE;
}

void CRuleMainBaseDlg::SetComomParm(
    int nChannelID,
    WPG_Rule* pRule,
    IVRuleType type,
    BOOL EditMode)
{
    m_nCurrentChan = nChannelID;
    m_pRule = pRule;
    m_type = type;
    m_EditMode = EditMode;
}

void CRuleMainBaseDlg::OnBnClickedOk()
{
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    GatherUseSet();
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
    if ( m_nToolsChoose == IVUtil::Choose_Line )
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
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    CFilterDlg Dlg;
    Dlg.SetComomParm(m_nCurrentChan, m_pRule);
    Dlg.DoModal();
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }
}

void CRuleMainBaseDlg::OnBnClickedSimulationBt()
{
    // ȡ����
    BOOL bRc = GatherUseSet();
    if ( !bRc )
    {
        AfxMessageBox(_T("Please Input Rule!"));
        return;
    }

    if ( m_bUse )
    {
        bRc = g_IIVDeviceBase2->StartSimulation(
            m_nCurrentChan, this, *m_pRule);
        if ( !bRc )
        {
            AfxMessageBox(_T("StartSimulation Failed!"));
            return;
        }

        SetTimer(Start_Simulation, Simulation_Wait_Time, NULL);
        SimulationEnable(!m_bUse);
        m_SimulationBT.SetWindowText(_T("Stop"));
        m_ChangeCursor = TRUE;
        BeginWaitCursor();
        m_pDrawContainer->EnableWindow(FALSE);
    }
    else
    {
        bRc = g_IIVDeviceBase2->StopSimulation(m_nCurrentChan);
        if ( !bRc )
        {
            AfxMessageBox(_T("StopSimulation Failed!"));
            return;
        }

        SetTimer(Start_Simulation, Simulation_Wait_Time, NULL);
        SimulationEnable(!m_bUse);
        m_SimulationBT.SetWindowText(_T("Simulation"));
        m_ChangeCursor = TRUE;
        BeginWaitCursor();
        m_pDrawContainer->EnableWindow(TRUE);
    }
    m_bUse = !m_bUse;
}

void CRuleMainBaseDlg::DrawToolChange( Windows::IDrawer* pDrawer )
{
    m_pDrawContainer->EnableToolAndNotEnableOther(pDrawer, TRUE);
}

void CRuleMainBaseDlg::OnBnClickedLineCheck()
{
    m_nToolsChoose = IVUtil::Choose_Line;
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
    m_nToolsChoose = IVUtil::Choose_Rectangle;
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
    if ( m_nToolsChoose == IVUtil::Choose_Line )
    {
        m_LineDrawer->SetLineColour(dwColour);
    }
    else if (m_nToolsChoose == IVUtil::Choose_Rectangle)
    {
        m_RectangleDrawer->SetLineColour(dwColour);
    }
    else if (m_nToolsChoose == IVUtil::Choose_Polygon)
    {
        m_PolygonDrawer->SetLineColour(dwColour);
    }
}

void CRuleMainBaseDlg::OnBnClickedPolygonCheck()
{
    m_nToolsChoose = IVUtil::Choose_Polygon;
    m_RectangleBT.SetCheck(BST_UNCHECKED);
    m_PolygonBT.SetCheck(BST_CHECKED);
    DrawToolChange(m_PolygonDrawer);
}

void CRuleMainBaseDlg::OnBnClickedRectangleCheck()
{
    m_nToolsChoose = IVUtil::Choose_Rectangle;
    m_RectangleBT.SetCheck(BST_CHECKED);
    m_PolygonBT.SetCheck(BST_UNCHECKED);
    DrawToolChange(m_RectangleDrawer);
}

void CRuleMainBaseDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnClose();
}

BOOL CRuleMainBaseDlg::OnVideoSend( FRAMEBUFSTRUCT *bufStruct )
{
    CRect Rect = m_Rect;
    FILETIME* time = (FILETIME*)&bufStruct->localTime;
    //GetClientRect(&Rect);
    ClientToScreen(&Rect);
    m_Player.Show(
        &Rect, 
        bufStruct->pBuf,
        CSingleVideoPlayer::YUV422,
        0, time);

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
        m_RectangleBT.ShowWindow(SW_HIDE);
        m_PolygonBT.ShowWindow(SW_HIDE);
    	break;
    case Line_Mode:
        m_ZoneBT.EnableWindow(FALSE);
    	break;
    case Zone_Mode:
        m_LineBT.EnableWindow(FALSE);
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
    int nFlag,
    DWORD dwUserData )
{
    //m_Drawer->ShowWindow(SW_HIDE);
    //m_Drawer->ShowWindow(SW_SHOW);
    static IIVViewer* pViewer = IVLiveFactory::GetLiveViewer();
    pViewer->Paint(dwUserData, dc, *rect, *pTime);

    m_pDrawContainer->Invalidate();
    return TRUE;
}

void CRuleMainBaseDlg::OnAlarmCallBack(
    IVRuleType type, int nChannelID, const FILETIME* pTime )
{
    m_AlarmOccurStatic.OnAlarmOccur();

}

void CRuleMainBaseDlg::SimulationEnable( BOOL bEnable )
{
    m_OKBt.EnableWindow(bEnable);
    m_CancelBt.EnableWindow(bEnable);
}

void CRuleMainBaseDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if ( nIDEvent == Start_Simulation )
    {
        KillTimer(Start_Simulation);
        m_ChangeCursor = FALSE;
        EndWaitCursor();
    }
    __super::OnTimer(nIDEvent);
}

// End of file


BOOL CRuleMainBaseDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_ChangeCursor)
    {
        RestoreWaitCursor();
        return TRUE;
    }

    return __super::OnSetCursor(pWnd, nHitTest, message);
}
