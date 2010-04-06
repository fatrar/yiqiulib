// ZoneAdvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "ZoneAdvDlg.h"


// CZoneAdvDlg dialog

IMPLEMENT_DYNAMIC(CZoneAdvDlg, CDialog)

CZoneAdvDlg::CZoneAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoneAdvDlg::IDD, pParent)
    , m_StrRuleName(_T(""))
    , m_strRuleNameEdit(_T(""))
    , m_strView(_T(""))
    , m_strObject(_T(""))
    , m_StrTime(_T(""))
    , m_strLoiters(_T(""))
    , m_strLeftBehind(_T(""))
    , m_nLoitersEdit(Default_Loiters_duration)
    , m_nLeftBehindEdit(Default_LeftBehind_duration){}

CZoneAdvDlg::~CZoneAdvDlg(){}

void CZoneAdvDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_RULENAME, m_StrRuleName);
    DDX_Text(pDX, IDC_RULENAME_EDIT, m_strRuleNameEdit);
    DDV_MaxChars(pDX, m_strRuleNameEdit, Max_Rule_Name);
    DDX_Text(pDX, IDC_VIEW, m_strView);
    DDX_Text(pDX, IDC_OBJECT, m_strObject);
    DDX_Control(pDX, IDC_CHECK_PERSON, m_PersonCheck);
    DDX_Control(pDX, IDC_CHECK_VEHICLE, m_VehicleCheck);
    DDX_Control(pDX, IDC_CHECK_OTHER, m_OtherCheck);
    DDX_Control(pDX, IDC_CHECK_ALL, m_AllCheck);
    DDX_Text(pDX, IDC_TIME, m_StrTime);
    DDX_Text(pDX, IDC_LOITERS, m_strLoiters);
    DDX_Text(pDX, IDC_IDC_LOITERS_EDIT, m_nLoitersEdit);
    DDV_MinMaxInt(pDX, m_nLoitersEdit, 1, Loiters_Max_Time);
    DDX_Text(pDX, IDC_LEFTBEHIND_EDIT, m_nLeftBehindEdit);
    DDV_MinMaxInt(pDX, m_nLeftBehindEdit, 1, LeftBehind_Max_Time);
    DDX_Control(pDX, IDC_V_VIEW, m_VerticalBt);
    DDX_Control(pDX, IDC_P_VIEW, m_ParallelBt);
    DDX_Text(pDX, IDC_LEFTBEHIND, m_strLeftBehind);
}


BEGIN_MESSAGE_MAP(CZoneAdvDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CZoneAdvDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CZoneAdvDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_ALL, &CZoneAdvDlg::OnBnClickedCheckAll)
    ON_BN_CLICKED(IDC_CHECK_PERSON, &CZoneAdvDlg::OnBnClickedCheckPerson)
    ON_BN_CLICKED(IDC_CHECK_VEHICLE, &CZoneAdvDlg::OnBnClickedCheckVehicle)
    ON_BN_CLICKED(IDC_CHECK_OTHER, &CZoneAdvDlg::OnBnClickedCheckOther)
    ON_BN_CLICKED(IDC_V_VIEW, &CZoneAdvDlg::OnBnClickedVView)
    ON_BN_CLICKED(IDC_P_VIEW, &CZoneAdvDlg::OnBnClickedPView)
END_MESSAGE_MAP()


// CZoneAdvDlg message handlers

BOOL CZoneAdvDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Rule
    m_StrRuleName.LoadString(IDS_RuleName);

    // View
    m_strView.LoadString(IDS_View);   
    CString strTmp;
    strTmp.LoadString(IDS_Vertical_View);
    m_VerticalBt.SetWindowText(strTmp);
    m_VerticalBt.SetCheck(BST_CHECKED);
    strTmp.LoadString(IDS_Parallel_View);
    m_ParallelBt.SetWindowText(strTmp);
 
    // Object
    m_strObject.LoadString(IDS_Object);
    strTmp.LoadString(IDS_Object_Person);
    m_PersonCheck.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Object_Vehicle);
    m_VehicleCheck.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Object_Other);
    m_OtherCheck.SetWindowText(strTmp); 
    strTmp.LoadString(IDS_Object_All);
    m_AllCheck.SetWindowText(strTmp);
    
    // Time
    m_StrTime.LoadString(IDS_Time_S);
    m_strLoiters.LoadString(IDS_Loiters_Time);
    m_strLeftBehind.LoadString(IDS_LeftBehind_Time);

    //switch (m_type)
    //{
    //case IV_Invade:
    //case IV_Leave_Disappear:
    //    HideAllTimeWindow();
    //    break;
    //case IV_LeftBehind:
    //    GetDlgItem(IDC_LOITERS)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_IDC_LOITERS_EDIT)->ShowWindow(SW_HIDE);
    //    break;
    //case IV_Loiters:
    //    GetDlgItem(IDC_LEFTBEHIND_EDIT)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_LEFTBEHIND)->ShowWindow(SW_HIDE);
    //    break;
    //case IV_Stage_Change:
    //    HideAllTimeWindow();
    //    break;
    //default:
    //    TRACE("CZoneAdvDlg::OnInitDialog() Type Error!\n");
    //}

    UpdateUIByRule();  
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CZoneAdvDlg::GetObjSet()
{
    unsigned int& nValue = m_pRule->ruleDescription.targetClassification;
    if ( m_AllCheck.GetCheck() == BST_CHECKED )
    {
        nValue = TARGET_CLASSIFICATION_ANYTHING;
        return;
    }

    nValue = 0;
    if ( m_PersonCheck.GetCheck() == BST_CHECKED )
    {
        nValue |= TARGET_CLASSIFICATION_HUMAN;
    }
    if ( m_VehicleCheck.GetCheck() == BST_CHECKED )
    {
        nValue |= TARGET_CLASSIFICATION_VEHICLE;
    }
    if ( m_OtherCheck.GetCheck() == BST_CHECKED )
    {
        nValue |= TARGET_CLASSIFICATION_UNKNOWN;
    }
    if ( nValue == 0 )
    {
        CString strTmp;
        strTmp.LoadString(IDS_Object_Alarm);
        if ( AfxMessageBox(strTmp, MB_OKCANCEL) == IDCANCEL )
        {
            throw 0;
        }
    }
    //nValue = TARGET_CLASSIFICATION_ANYTHING;
}

void CZoneAdvDlg::GetViewSet()
{
    WPG_AOIEventDescription& aoiDes = m_pRule->ruleDescription.description.aoiEventDescription;
    if ( m_VerticalBt.GetCheck() == BST_CHECKED) 
    {
        aoiDes.planeType = IMAGE_PLANE;
    }
    else
    {
        aoiDes.planeType = GROUND_PLANE;
    } 
}

void CZoneAdvDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    CString strTmp;
    if ( m_strRuleNameEdit.GetLength() == 0 )
    {
        strTmp.LoadString(IDS_RuleName_Alarm);
        if ( AfxMessageBox(strTmp, MB_OKCANCEL) == IDCANCEL )
        {
            return;
        }  
    }

    _bstr_t bstrTmp = (LPCTSTR)m_strRuleNameEdit;
    strcpy(m_pRule->ruleName, bstrTmp);
    
    WPG_AOIEventDescription& aoiDes = m_pRule->ruleDescription.description.aoiEventDescription;
    try
    {
        switch (m_type)
        {
        case IV_Invade:
        case IV_Leave_Disappear:
            GetObjSet();
            GetViewSet();
            break;
        case IV_LeftBehind:
            GetObjSet();
            GetViewSet();
            aoiDes.actionType.leftBehind.duration = m_nLeftBehindEdit;
            break;
        case IV_Loiters:
            GetObjSet();
            GetViewSet();
            aoiDes.actionType.loiters.duration = m_nLoitersEdit;
            break;
        case IV_Stage_Change:
            HideAllTimeWindow();
            break;
        default:
            return;
        }
    }
    catch(...)
    {
        return;
    }
    OnOK();
}

void CZoneAdvDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CZoneAdvDlg::HideAllTimeWindow()
{
    GetDlgItem(IDC_TIME)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_LOITERS)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_IDC_LOITERS_EDIT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_LEFTBEHIND_EDIT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_LEFTBEHIND)->ShowWindow(SW_HIDE);
}

void CZoneAdvDlg::HideObjAndView()
{
    // hide View
    GetDlgItem(IDC_VIEW)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_V_VIEW)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_P_VIEW)->ShowWindow(SW_HIDE);

    // Hide Object
    GetDlgItem(IDC_OBJECT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CHECK_PERSON)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CHECK_VEHICLE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CHECK_OTHER)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CHECK_ALL)->ShowWindow(SW_HIDE);
}

void CZoneAdvDlg::Init(IVRuleType type, WPG_Rule* pRule)
{
    m_pRule = pRule;


    m_strDefaultRuleName = m_strRuleNameEdit = pRule->ruleName;
    m_type = type;
}

// click All check button
void CZoneAdvDlg::OnBnClickedCheckAll()
{
    if ( BST_CHECKED == m_AllCheck.GetCheck() )
    {
        SetAllCheck();
    }
}

void CZoneAdvDlg::SetAllCheck()
{
    m_PersonCheck.SetCheck(BST_CHECKED);
    m_VehicleCheck.SetCheck(BST_CHECKED);
    m_OtherCheck.SetCheck(BST_CHECKED);
    m_AllCheck.SetCheck(BST_CHECKED);
}

void CZoneAdvDlg::OnBnClickedCheckPerson()
{
    if ( BST_UNCHECKED == m_PersonCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CZoneAdvDlg::OnBnClickedCheckVehicle()
{
    if ( BST_UNCHECKED == m_VehicleCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CZoneAdvDlg::OnBnClickedCheckOther()
{
    if ( BST_UNCHECKED == m_OtherCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CZoneAdvDlg::UpdateUIByRule()
{
    WPG_AOIEventDescription& aoiDes = m_pRule->ruleDescription.description.aoiEventDescription;
    switch (m_type)
    {
    case IV_Invade:
    case IV_Leave_Disappear:
        HideAllTimeWindow();
        SetObjSet();
        SetViewSet();
        break;
    case IV_LeftBehind:
        GetDlgItem(IDC_LOITERS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_IDC_LOITERS_EDIT)->ShowWindow(SW_HIDE);
        SetObjSet();
        SetViewSet();
        m_nLeftBehindEdit = aoiDes.actionType.leftBehind.duration;
        break;
    case IV_Loiters:
        GetDlgItem(IDC_LEFTBEHIND_EDIT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LEFTBEHIND)->ShowWindow(SW_HIDE);
        SetObjSet();
        SetViewSet();
        m_nLoitersEdit = aoiDes.actionType.loiters.duration;
        break;
    case IV_Stage_Change:
        HideAllTimeWindow();
    default:
        ASSERT(FALSE);
        return;
    }
    UpdateData(FALSE);
}

void CZoneAdvDlg::SetObjSet()
{
    unsigned int& nValue = m_pRule->ruleDescription.targetClassification;
    if ( TARGET_CLASSIFICATION_ANYTHING == nValue )
    {
        SetAllCheck();
        return;
    }

    if ( nValue & TARGET_CLASSIFICATION_HUMAN )
    {
        m_PersonCheck.SetCheck(BST_CHECKED);
    }
    if ( nValue & TARGET_CLASSIFICATION_VEHICLE )
    {
        m_VehicleCheck.SetCheck(BST_CHECKED);
    }
    if ( nValue & TARGET_CLASSIFICATION_UNKNOWN )
    {
        m_OtherCheck.SetCheck(BST_CHECKED);
    }
}

void CZoneAdvDlg::SetViewSet()
{
    WPG_AOIEventDescription& aoiDes = m_pRule->ruleDescription.description.aoiEventDescription;
    if ( GROUND_PLANE == aoiDes.planeType ) 
    {
        m_VerticalBt.SetCheck(BST_CHECKED);
        m_ParallelBt.SetCheck(BST_UNCHECKED);
    }
    else
    {
        m_VerticalBt.SetCheck(BST_UNCHECKED);
        m_ParallelBt.SetCheck(BST_CHECKED);
    }
}

void CZoneAdvDlg::OnBnClickedVView()
{
    if ( BST_UNCHECKED == m_VerticalBt.GetCheck() )
    {
        return;
    }

    m_VerticalBt.SetCheck(BST_CHECKED);
    m_ParallelBt.SetCheck(BST_UNCHECKED);
}

void CZoneAdvDlg::OnBnClickedPView()
{
    if ( BST_UNCHECKED == m_ParallelBt.GetCheck() )
    {
        return;
    }

    m_VerticalBt.SetCheck(BST_UNCHECKED);
    m_ParallelBt.SetCheck(BST_CHECKED);
}
