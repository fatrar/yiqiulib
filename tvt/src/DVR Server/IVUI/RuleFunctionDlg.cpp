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

BOOL CStageChangeDlg::OnInitDialog()
{
    CRuleMainBaseDlg::OnInitDialog();
    UseToolCtrlMode(Nothing_Mode);
    m_AdvBT.EnableWindow(FALSE);
    m_FilterBT.EnableWindow(FALSE);
    return TRUE;
}



CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type,  CWnd* pParentWnd)
{
    switch (type)
    {
    case IV_Invade: return new CInvadeDlg(pParentWnd);
    case IV_Leave_Disappear: return new CLeaveDisappearDlg(pParentWnd);
    case IV_LeftBehind: return new CLeftBehindDlg(pParentWnd);
    case IV_Loiters: return new CLoitersAddDlg(pParentWnd);
    case IV_Statistic: return new CStatisticDlg(pParentWnd);
    case IV_Vehicle_Retrograde: return new CVehicleRetrogradeDlg(pParentWnd);
    case IV_Illegal_Parking: return new CIllegalParkingDlg(pParentWnd);
    case IV_Stage_Change: return new CStageChangeDlg(pParentWnd);
    default:
        assert(false);
        return NULL;
    }
}