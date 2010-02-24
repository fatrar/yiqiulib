/*H***************************************************************************
 File            : RuleAddMainDlg.cpp
 Subsystem       : 
 Function Name(s): CRuleAddMainDlg
 Author          : YiQiu
 Date            : 2010-2-24  
 Time            : 14:58
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVUI.h"
#include "RuleAddMainDlg.h"


// CRuleAddMainDlg dialog


// CRuleAddMainDlg message handlers
//
//BOOL CRuleAddMainDlg::OnInitDialog()
//{
//    CRuleMainBaseDlg::OnInitDialog();
//
//    // TODO:  Add extra initialization here
//    return TRUE;
//}
//
//
//void CRuleAddMainDlg::OnBnClickedOk()
//{
//    OnOK();
//}
//
//void CRuleAddMainDlg::OnBnClickedCancel()
//{
//    OnCancel();
//}
//
//void CRuleAddMainDlg::OnBnClickedAdvBt()
//{  
//    if ( m_nToolsChoose == Choose_Line )
//    {
//        CLineAdvDlg Dlg;
//        Dlg.DoModal();
//    }
//    else
//    {
//        CZoneAdvDlg Dlg;
//        Dlg.DoModal();
//    }
//}
//
//void CRuleAddMainDlg::OnBnClickedFilterBt()
//{
//    CFilterDlg Dlg;
//    Dlg.DoModal();
//}
//
//void CRuleAddMainDlg::OnBnClickedSimulationBt()
//{
//    // TODO: Add your control notification handler code here
//}
//
//void CRuleAddMainDlg::OnBnClickedLineCheck()
//{
//    m_nToolsChoose = Choose_Line;
//    m_LineBT.SetCheck(BST_CHECKED);
//    m_ZoneBT.SetCheck(BST_UNCHECKED);
//    m_SelectBT.SetCheck(BST_UNCHECKED);
//
//    m_RectangleBT.ShowWindow(SW_HIDE);
//    m_PolygonBT.ShowWindow(SW_HIDE);
//    m_LineRightBT.SetCheck(BST_CHECKED);
//    m_LineRightBT.ShowWindow(SW_SHOW);
//    m_LineLeftBT.ShowWindow(SW_SHOW);
//    m_LineBothBT.ShowWindow(SW_SHOW);
//}
//
//void CRuleAddMainDlg::OnBnClickedZoneCheck()
//{
//    m_nToolsChoose = Choose_Zone;
//    m_LineBT.SetCheck(BST_UNCHECKED);
//    m_ZoneBT.SetCheck(BST_CHECKED);
//    m_SelectBT.SetCheck(BST_UNCHECKED);
//
//    m_RectangleBT.SetCheck(BST_CHECKED);
//    m_RectangleBT.ShowWindow(SW_SHOW);
//    m_PolygonBT.ShowWindow(SW_SHOW);
//    m_LineRightBT.ShowWindow(SW_HIDE);
//    m_LineLeftBT.ShowWindow(SW_HIDE);
//    m_LineBothBT.ShowWindow(SW_HIDE);
//}
//
//void CRuleAddMainDlg::OnBnClickedSelectCheck()
//{
//    m_LineBT.SetCheck(BST_UNCHECKED);
//    m_ZoneBT.SetCheck(BST_UNCHECKED);
//    m_SelectBT.SetCheck(BST_CHECKED);
//}
//
//void CRuleAddMainDlg::OnBnClickedRightCheck()
//{
//    m_LineRightBT.SetCheck(BST_CHECKED);
//    m_LineLeftBT.SetCheck(BST_UNCHECKED);
//    m_LineBothBT.SetCheck(BST_UNCHECKED);
//}
//
//void CRuleAddMainDlg::OnBnClickedLeftCheck()
//{
//    m_LineRightBT.SetCheck(BST_UNCHECKED);
//    m_LineLeftBT.SetCheck(BST_CHECKED);
//    m_LineBothBT.SetCheck(BST_UNCHECKED);
//}
//
//void CRuleAddMainDlg::OnBnClickedBothCheck()
//{
//    m_LineRightBT.SetCheck(BST_UNCHECKED);
//    m_LineLeftBT.SetCheck(BST_UNCHECKED);
//    m_LineBothBT.SetCheck(BST_CHECKED); 
//}
//
//void CRuleAddMainDlg::OnBnClickedColourBt()
//{
//   
//}
//
//void CRuleAddMainDlg::OnBnClickedPolygonCheck()
//{
//    // TODO: Add your control notification handler code here
//}
//
//void CRuleAddMainDlg::OnBnClickedRectangleCheck()
//{
//    // TODO: Add your control notification handler code here
//}
//
//void CRuleAddMainDlg::OnClose()
//{
//    // TODO: Add your message handler code here and/or call default
//    CDialog::OnClose();
//}



// End of file

BOOL CInvadeAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Use_All_Mode);
    return TRUE;
}

BOOL CLeaveDisappearAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Use_All_Mode);
    return TRUE;
}

BOOL CLeftBehindAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    return TRUE;
}


BOOL CLoitersAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    return TRUE;
}

BOOL CStatisticAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Line_Mode);
    return TRUE;
}

BOOL CVehicleRetrogradeAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    return TRUE;
}

BOOL CIllegalParkingAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    return TRUE;
}

BOOL CStageChangeAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Nothing_Mode);
    return TRUE;
}



CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type)
{
    switch (type)
    {
    case IV_Invade: return new CInvadeAddDlg();
    case IV_Leave_Disappear: return new CLeaveDisappearAddDlg();
    case IV_LeftBehind: return new CLeftBehindAddDlg();
    case IV_Loiters: return new CLoitersAddDlg();
    case IV_Statistic: return new CStatisticAddDlg();
    case IV_Vehicle_Retrograde: return new CVehicleRetrogradeAddDlg();
    case IV_Illegal_Parking: return new CIllegalParkingAddDlg();
    case IV_Stage_Change: return new CStageChangeAddDlg();
    default:
        assert(false);
        return NULL;
    }
}