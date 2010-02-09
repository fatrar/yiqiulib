// RuleAddMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "RuleAddMainDlg.h"


// CRuleAddMainDlg dialog

IMPLEMENT_DYNAMIC(CRuleAddMainDlg, CDialog)

CRuleAddMainDlg::CRuleAddMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleAddMainDlg::IDD, pParent)
{

}

CRuleAddMainDlg::~CRuleAddMainDlg()
{
}

void CRuleAddMainDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CRuleAddMainDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CRuleAddMainDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CRuleAddMainDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_Adv_BT, &CRuleAddMainDlg::OnBnClickedAdvBt)
    ON_BN_CLICKED(IDC_Filter_BT, &CRuleAddMainDlg::OnBnClickedFilterBt)
    ON_BN_CLICKED(IDC_Simulation_BT, &CRuleAddMainDlg::OnBnClickedSimulationBt)
    ON_BN_CLICKED(IDC_Line_Check, &CRuleAddMainDlg::OnBnClickedLineCheck)
    ON_BN_CLICKED(IDC_Zone_Check, &CRuleAddMainDlg::OnBnClickedZoneCheck)
    ON_BN_CLICKED(IDC_Select_Check, &CRuleAddMainDlg::OnBnClickedSelectCheck)
    ON_BN_CLICKED(IDC_Right_Check, &CRuleAddMainDlg::OnBnClickedRightCheck)
    ON_BN_CLICKED(IDC_Left_Check, &CRuleAddMainDlg::OnBnClickedLeftCheck)
    ON_BN_CLICKED(IDC_Both_Check, &CRuleAddMainDlg::OnBnClickedBothCheck)
    ON_BN_CLICKED(IDC_Colour_BT, &CRuleAddMainDlg::OnBnClickedColourBt)
    ON_BN_CLICKED(IDC_Polygon_Check, &CRuleAddMainDlg::OnBnClickedPolygonCheck)
    ON_BN_CLICKED(IDC_Rectangle_Check, &CRuleAddMainDlg::OnBnClickedRectangleCheck)
END_MESSAGE_MAP()


// CRuleAddMainDlg message handlers

BOOL CRuleAddMainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    OnBnClickedLineCheck();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CRuleAddMainDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CRuleAddMainDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CRuleAddMainDlg::OnBnClickedAdvBt()
{  
    if ( m_nToolsChoose == Choose_Line )
    {
        CLineAdvDlg Dlg;
        Dlg.DoModal();
    }
    else
    {
        CZoneAdvDlg Dlg;
        Dlg.DoModal();
    }
}

void CRuleAddMainDlg::OnBnClickedFilterBt()
{
    CFilterDlg Dlg;
    Dlg.DoModal();
}

void CRuleAddMainDlg::OnBnClickedSimulationBt()
{
    // TODO: Add your control notification handler code here
}

void CRuleAddMainDlg::OnBnClickedLineCheck()
{
    m_nToolsChoose = Choose_Line;
    m_LineBT.SetCheck(BST_CHECKED);
    m_ZoneBT.SetCheck(BST_UNCHECKED);
    m_SelectBT.SetCheck(BST_UNCHECKED);

    m_RectangleBT.ShowWindow(SW_HIDE);
    m_PolygonBT.ShowWindow(SW_HIDE);
    m_LineRightBT.SetCheck(BST_CHECKED);
    m_LineRightBT.ShowWindow(SW_SHOW);
    m_LineLeftBT.ShowWindow(SW_SHOW);
    m_LineBothBT.ShowWindow(SW_SHOW);
}

void CRuleAddMainDlg::OnBnClickedZoneCheck()
{
    m_nToolsChoose = Choose_Zone;
    m_LineBT.SetCheck(BST_UNCHECKED);
    m_ZoneBT.SetCheck(BST_CHECKED);
    m_SelectBT.SetCheck(BST_UNCHECKED);

    m_RectangleBT.SetCheck(BST_CHECKED);
    m_RectangleBT.ShowWindow(SW_SHOW);
    m_PolygonBT.ShowWindow(SW_SHOW);
    m_LineRightBT.ShowWindow(SW_HIDE);
    m_LineLeftBT.ShowWindow(SW_HIDE);
    m_LineBothBT.ShowWindow(SW_HIDE);
}

void CRuleAddMainDlg::OnBnClickedSelectCheck()
{
    m_LineBT.SetCheck(BST_UNCHECKED);
    m_ZoneBT.SetCheck(BST_UNCHECKED);
    m_SelectBT.SetCheck(BST_CHECKED);
}

void CRuleAddMainDlg::OnBnClickedRightCheck()
{
    m_LineRightBT.SetCheck(BST_CHECKED);
    m_LineLeftBT.SetCheck(BST_UNCHECKED);
    m_LineBothBT.SetCheck(BST_UNCHECKED);
}

void CRuleAddMainDlg::OnBnClickedLeftCheck()
{
    m_LineRightBT.SetCheck(BST_UNCHECKED);
    m_LineLeftBT.SetCheck(BST_CHECKED);
    m_LineBothBT.SetCheck(BST_UNCHECKED);
}

void CRuleAddMainDlg::OnBnClickedBothCheck()
{
    m_LineRightBT.SetCheck(BST_UNCHECKED);
    m_LineLeftBT.SetCheck(BST_UNCHECKED);
    m_LineBothBT.SetCheck(BST_CHECKED); 
}

void CRuleAddMainDlg::OnBnClickedColourBt()
{
   
}

void CRuleAddMainDlg::OnBnClickedPolygonCheck()
{
    // TODO: Add your control notification handler code here
}

void CRuleAddMainDlg::OnBnClickedRectangleCheck()
{
    // TODO: Add your control notification handler code here
}

