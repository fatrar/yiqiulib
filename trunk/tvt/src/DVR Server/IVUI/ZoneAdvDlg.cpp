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
    , m_nLoitersEdit(Loiters_Default_Time)
    , m_nLeftBehindEdit(LeftBehind_Default_Time)
    , m_IsInit(FALSE)
{

}

CZoneAdvDlg::~CZoneAdvDlg()
{
}

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

    // [] 看需要刷新不？

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CZoneAdvDlg::OnBnClickedOk()
{
    if ( !CheckUserSet() )
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

void CZoneAdvDlg::Init(IVRuleType type, const CString& strRuleName )
{
    switch (type)
    {
    case IV_Invade:
    	break;
    case IV_Leave_Disappear:
    	break;
    case IV_LeftBehind:
    	break;
    case IV_Loiters:
        break;
    default:
    	return;
    }

    m_IsInit = TRUE;
    m_strDefaultRuleName = m_strRuleNameEdit = strRuleName;
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

bool CZoneAdvDlg::CheckUserSet()
{
    UpdateData(TRUE);
    
    CString strTmp;
    if ( m_strRuleNameEdit.GetLength() == 0 )
    {
        strTmp.LoadString(IDS_RuleName_Alarm);
        if ( AfxMessageBox(strTmp) == IDCANCEL )
        {
            return false;
        }

        m_strRuleNameEdit = m_strDefaultRuleName;
    }

    if ( m_PersonCheck.GetCheck() == BST_UNCHECKED && 
         m_VehicleCheck.GetCheck() == BST_UNCHECKED && 
         m_OtherCheck.GetCheck() == BST_UNCHECKED )
    {
        strTmp.LoadString(IDS_Object_Alarm);
        if ( AfxMessageBox(strTmp) == IDCANCEL )
        {
            return false;
        }

        SetAllCheck();
    }

    return true;
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
