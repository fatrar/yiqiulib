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
#include "RuleFunctionDlg.h"


// 
// **************************** CInvadeDlg **************************
//
BOOL CInvadeDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Use_All_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedLineCheck();
    }
    //
    return TRUE;
}

BOOL CInvadeDlg::GatherUseSet()
{
    CRect rect;
    m_pDrawContainer->GetClientRect(&rect);
    WPG_EventDescriptionUnion& des = m_pRule->ruleDescription.description;
    if ( m_nToolsChoose == IVUtil::Choose_Line ||
         m_nToolsChoose == IVUtil::Choose_LineEx )
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
            AfxMessageBox(_T("Please Input Rectangle!"));
            return FALSE;
        }
        m_pRule->ruleDescription.type = AOI_EVENT;
    }
    else if (m_nToolsChoose == IVUtil::Choose_Polygon)
    {
        if ( !GetUseInputPolygonInfor(m_PolygonDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input Polygon!"));
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

void CInvadeDlg::SimulationEnable( BOOL bEnable )
{
    m_LineBT.EnableWindow(bEnable);
    m_ZoneBT.EnableWindow(bEnable);
    m_SelectBT.EnableWindow(bEnable);

    m_ColourBT.EnableWindow(bEnable);
    m_AdvBT.EnableWindow(bEnable);
    m_FilterBT.EnableWindow(bEnable);
    m_OKBt.EnableWindow(bEnable);
    m_CancelBt.EnableWindow(bEnable);

    if ( m_nToolsChoose == IVUtil::Choose_Line ||
         m_nToolsChoose == IVUtil::Choose_LineEx )
    {
        m_LineRightBT.EnableWindow(bEnable);
        m_LineLeftBT.EnableWindow(bEnable);
        m_LineBothBT.EnableWindow(bEnable);
    }
    else if ( m_nToolsChoose == IVUtil::Choose_Rectangle ||
              m_nToolsChoose == IVUtil::Choose_Polygon )
    {
        m_RectangleBT.EnableWindow(bEnable);
        m_PolygonBT.EnableWindow(bEnable);
    }
}

// } 
// CInvadeDlg

// 
// **************************** CLeftBehindDlg **************************
//
BOOL CLeftBehindDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    
    return TRUE;
}

BOOL CLeftBehindDlg::GatherUseSet()
{
    CRect rect;
    m_pDrawContainer->GetClientRect(&rect);
    WPG_EventDescriptionUnion& des = m_pRule->ruleDescription.description;
    if (m_nToolsChoose == IVUtil::Choose_Rectangle)
    {
        if ( !GetUseInputRectangleInfor(m_RectangleDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input Rectangle!"));
            return FALSE;
        }
    }
    else if (m_nToolsChoose == IVUtil::Choose_Polygon)
    {
        if ( !GetUseInputPolygonInfor(m_PolygonDrawer, rect, des.aoiEventDescription.polygon))
        {
            AfxMessageBox(_T("Please Input Polygon!"));
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

void CLeftBehindDlg::SimulationEnable( BOOL bEnable )
{
    if ( m_nToolsChoose == IVUtil::Choose_Rectangle ||
         m_nToolsChoose == IVUtil::Choose_Polygon )
    {
        m_SelectBT.EnableWindow(bEnable);
        m_ZoneBT.EnableWindow(bEnable);
        m_LineBT.EnableWindow(bEnable);

        m_RectangleBT.EnableWindow(bEnable);
        m_PolygonBT.EnableWindow(bEnable);

        m_ColourBT.EnableWindow(bEnable);
        m_AdvBT.EnableWindow(bEnable);
        m_FilterBT.EnableWindow(bEnable);
        m_OKBt.EnableWindow(bEnable);
        m_CancelBt.EnableWindow(bEnable);
    }
    //else if ()
    //{
    //    m_RectangleBT.EnableWindow(bEnable);
    //    m_PolygonBT.EnableWindow(bEnable);
    //}
    else 
    {
        ASSERT(FALSE);
    }
}
// } 
// CLeftBehindDlg

// 
// **************************** CStatisticDlg **************************
//
BOOL CStatisticDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Line_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedLineCheck();
    }
    return TRUE;
}

void CStatisticDlg::OnBnClickedLineCheck()
{
    m_nToolsChoose = IVUtil::Choose_LineEx;
    m_LineBT.SetCheck(BST_CHECKED);
    m_ZoneBT.SetCheck(BST_UNCHECKED);
    m_SelectBT.SetCheck(BST_UNCHECKED);

    m_RectangleBT.ShowWindow(SW_HIDE);
    m_PolygonBT.ShowWindow(SW_HIDE);
    m_LineBothBT.SetCheck(BST_CHECKED);
    m_LineRightBT.ShowWindow(SW_SHOW);
    m_LineLeftBT.ShowWindow(SW_SHOW);
    m_LineBothBT.ShowWindow(SW_SHOW);

    DrawToolChange(m_LineDrawerEx);
}

BOOL CStatisticDlg::GatherUseSet()
{
    CRect rect;
    m_pDrawContainer->GetClientRect(&rect);
    WPG_EventDescriptionUnion& des = m_pRule->ruleDescription.description;
    if ( m_nToolsChoose != IVUtil::Choose_LineEx )
    {
        ASSERT(FALSE);
        return FALSE;
    }

    if ( !GetUseInputLineInfor(m_LineDrawerEx, rect, des.tripwireEventDescription))
    {
        AfxMessageBox(_T("Please Input line!"));
        return FALSE;
    }
    m_pRule->ruleDescription.type = TRIPWIRE_EVENT;
    return TRUE;
}

void CStatisticDlg::SimulationEnable( BOOL bEnable )
{
    m_LineBT.EnableWindow(bEnable);
    m_SelectBT.EnableWindow(bEnable);

    m_ColourBT.EnableWindow(bEnable);
    m_AdvBT.EnableWindow(bEnable);
    m_FilterBT.EnableWindow(bEnable);
    m_OKBt.EnableWindow(bEnable);
    m_CancelBt.EnableWindow(bEnable);

    m_LineRightBT.EnableWindow(bEnable);
    m_LineLeftBT.EnableWindow(bEnable);
    m_LineBothBT.EnableWindow(bEnable);
}

// } 
// CStatisticDlg

// 
// **************************** CVehicleRetrogradeDlg **************************
//
BOOL CVehicleRetrogradeDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    return TRUE;
}
// } 
// CVehicleRetrogradeDlg

// 
// **************************** CIllegalParkingDlg **************************
//
BOOL CIllegalParkingDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Zone_Mode);
    if ( !m_EditMode )
    {
        OnBnClickedZoneCheck();
    }
    return TRUE;
}
// } 
// CIllegalParkingDlg

// 
// **************************** CStageChangeDlg **************************
//
BOOL CStageChangeDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Nothing_Mode);
    m_AdvBT.EnableWindow(FALSE);
    m_FilterBT.EnableWindow(FALSE);
    return TRUE;
}
// } 
// CStageChangeDlg


CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type,  CWnd* pParentWnd)
{
    switch (type)
    {
    case IV_Invade: return new CInvadeDlg(pParentWnd);
    case IV_Leave_Disappear: return new CLeaveDisappearDlg(pParentWnd);
    case IV_LeftBehind: return new CLeftBehindDlg(pParentWnd);
    case IV_Loiters: return new CLoitersAddDlg(pParentWnd);
    case IV_Statistic: return new CStatisticDlg(pParentWnd);
    case IV_Vehicle_Retrograde: return NULL;// new CVehicleRetrogradeDlg(pParentWnd);
    case IV_Illegal_Parking: return NULL;//new CIllegalParkingDlg(pParentWnd);
    case IV_Stage_Change: return new CStageChangeDlg(pParentWnd);
    default:
        assert(false);
        return NULL;
    }
}