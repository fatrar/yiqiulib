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
    if ( !m_EditMode )
    {
        OnBnClickedLineCheck();
    }
    //
    return TRUE;
}

BOOL CInvadeAddDlg::GatherUseSet()
{
    CRect rect;
    m_pDrawContainer->GetClientRect(&rect);
    WPG_EventDescriptionUnion& des = m_pRule->ruleDescription.description;
    if ( m_nToolsChoose == IVUtil::Choose_Line )
    {
        if ( !GetUseInputLineInfor(m_LineDrawer, rect, des.tripwireEventDescription))
        {
            AfxMessageBox(_T("Please Input line!"));
            return FALSE;
        }
        m_pRule->ruleDescription.type = TRIPWIRE_EVENT;
    }
    else if (m_nToolsChoose == IVUtil::Choose_Rectangle)
    {
        if ( !GetUseInputRectangleInfor(m_RectangleDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input line!"));
            return FALSE;
        }
        m_pRule->ruleDescription.type = AOI_EVENT;
    }
    else if (m_nToolsChoose == IVUtil::Choose_Polygon)
    {
        if ( !GetUseInputPolygonInfor(m_PolygonDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input line!"));
            return FALSE;
        }
        m_pRule->ruleDescription.type = AOI_EVENT;
    }
    else 
    {
        ASSERT(FALSE);
        return FALSE;
    }
    return TRUE;
}

BOOL CLeftBehindAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    
    return TRUE;
}

BOOL CLeftBehindAddDlg::GatherUseSet()
{
    CRect rect;
    m_pDrawContainer->GetClientRect(&rect);
    WPG_EventDescriptionUnion& des = m_pRule->ruleDescription.description;
    if (m_nToolsChoose == IVUtil::Choose_Rectangle)
    {
        if ( !GetUseInputRectangleInfor(m_RectangleDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input line!"));
            return FALSE;
        }
    }
    else if (m_nToolsChoose == IVUtil::Choose_Polygon)
    {
        if ( !GetUseInputPolygonInfor(m_PolygonDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input line!"));
            return FALSE;
        }
    }
    else 
    {
        ASSERT(FALSE);
        return FALSE;
    }
    return TRUE;
}

BOOL CStatisticAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Line_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedLineCheck();
    }
    return TRUE;
}

BOOL CVehicleRetrogradeAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    return TRUE;
}

BOOL CIllegalParkingAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    return TRUE;
}

BOOL CStageChangeAddDlg::OnInitDialog()
{
    CRuleAddMainDlg::OnInitDialog();
    UseToolCtrlMode(Nothing_Mode);
    return TRUE;
}



CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type,  CWnd* pParentWnd)
{
    switch (type)
    {
    case IV_Invade: return new CInvadeAddDlg(pParentWnd);
    case IV_Leave_Disappear: return new CLeaveDisappearAddDlg(pParentWnd);
    case IV_LeftBehind: return new CLeftBehindAddDlg(pParentWnd);
    case IV_Loiters: return new CLoitersAddDlg(pParentWnd);
    case IV_Statistic: return new CStatisticAddDlg(pParentWnd);
    case IV_Vehicle_Retrograde: return new CVehicleRetrogradeAddDlg(pParentWnd);
    case IV_Illegal_Parking: return new CIllegalParkingAddDlg(pParentWnd);
    case IV_Stage_Change: return new CStageChangeAddDlg(pParentWnd);
    default:
        assert(false);
        return NULL;
    }
}