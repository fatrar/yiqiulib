/*H***************************************************************************
 File            : LineAdvDlg.cpp
 Subsystem       : 
 Function Name(s): CLineAdvDlg
 Author          : YiQiu
 Date            : 2010-1-28  
 Time            : 11:48
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVUI.h"
#include "LineAdvDlg.h"


// CLineAdvDlg dialog

IMPLEMENT_DYNAMIC(CLineAdvDlg, CDialog)

CLineAdvDlg::CLineAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineAdvDlg::IDD, pParent)
    , m_strRuleName(_T(""))
    , m_strRuleNameEdit(_T(""))
    , m_strObject(_T(""))
{

}

CLineAdvDlg::~CLineAdvDlg()
{
}

void CLineAdvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_RULENAME, m_strRuleName);
    DDX_Text(pDX, IDC_RULENAME_EDIT, m_strRuleNameEdit);
    DDV_MaxChars(pDX, m_strRuleNameEdit, Max_Rule_Name);
    DDX_Text(pDX, IDC_OBJECT, m_strObject);
    DDX_Control(pDX, IDC_CHECK_PERSON, m_PersonCheck);
    DDX_Control(pDX, IDC_CHECK_VEHICLE, m_VehicleCheck);
    DDX_Control(pDX, IDC_CHECK_OTHER, m_OtherCheck);
    DDX_Control(pDX, IDC_CHECK_ALL, m_AllCheck);
}


BEGIN_MESSAGE_MAP(CLineAdvDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CLineAdvDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CLineAdvDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_PERSON, &CLineAdvDlg::OnBnClickedCheckPerson)
    ON_BN_CLICKED(IDC_CHECK_VEHICLE, &CLineAdvDlg::OnBnClickedCheckVehicle)
    ON_BN_CLICKED(IDC_CHECK_OTHER, &CLineAdvDlg::OnBnClickedCheckOther)
    ON_BN_CLICKED(IDC_CHECK_ALL, &CLineAdvDlg::OnBnClickedCheckAll)
END_MESSAGE_MAP()


// CLineAdvDlg message handlers

BOOL CLineAdvDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Rule
    m_strRuleName.LoadString(IDS_RuleName);

    // Object
    CString strTmp;
    m_strObject.LoadString(IDS_Object);
    strTmp.LoadString(IDS_Object_Person);
    m_PersonCheck.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Object_Vehicle);
    m_VehicleCheck.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Object_Other);
    m_OtherCheck.SetWindowText(strTmp); 
    strTmp.LoadString(IDS_Object_All);
    m_AllCheck.SetWindowText(strTmp);

    do 
    {
        unsigned int nTest = m_pRule->ruleDescription.targetClassification;
        if ( nTest == 7 )
        {
            SetAllCheck();
            break;
        }

        if ( nTest & TARGET_CLASSIFICATION_HUMAN )
        {
            m_PersonCheck.SetCheck(BST_CHECKED);
        }
        if ( nTest & TARGET_CLASSIFICATION_VEHICLE )
        {
            m_VehicleCheck.SetCheck(BST_CHECKED);
        }
        if ( nTest & TARGET_CLASSIFICATION_UNKNOWN )
        {
            m_OtherCheck.SetCheck(BST_CHECKED);
        }
    } 
    while (0);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineAdvDlg::OnBnClickedOk()
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
    unsigned int& nValue = m_pRule->ruleDescription.targetClassification;
    if ( m_AllCheck.GetCheck() == BST_CHECKED )
    {
        nValue = TARGET_CLASSIFICATION_ANYTHING;
        OnOK();
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
            return;
        }
        nValue = TARGET_CLASSIFICATION_ANYTHING;
    }
    OnOK();
}

void CLineAdvDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CLineAdvDlg::OnBnClickedCheckPerson()
{
    if ( BST_UNCHECKED == m_PersonCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CLineAdvDlg::OnBnClickedCheckVehicle()
{
    if ( BST_UNCHECKED == m_VehicleCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CLineAdvDlg::OnBnClickedCheckOther()
{
    if ( BST_UNCHECKED == m_OtherCheck.GetCheck() )
    {
        m_AllCheck.SetCheck(BST_UNCHECKED);
    }
}

void CLineAdvDlg::OnBnClickedCheckAll()
{
    if ( BST_CHECKED == m_AllCheck.GetCheck() )
    {
        SetAllCheck();
    }
}

void CLineAdvDlg::SetAllCheck()
{
    m_PersonCheck.SetCheck(BST_CHECKED);
    m_VehicleCheck.SetCheck(BST_CHECKED);
    m_OtherCheck.SetCheck(BST_CHECKED);
    m_AllCheck.SetCheck(BST_CHECKED);
}

void CLineAdvDlg::Init( WPG_Rule* pRule )
{
    m_pRule = pRule;
    m_strRuleNameEdit = pRule->ruleName;
}
//
//typedef enum WPG_TARGET_CLASSIFICATION
//{ 
//    TARGET_CLASSIFICATION_HUMAN     =  1, 
//    TARGET_CLASSIFICATION_VEHICLE   =  2,
//    TARGET_CLASSIFICATION_UNKNOWN   =  4,
//    TARGET_CLASSIFICATION_ANYTHING  =  7
//} WPG_TARGET_CLASSIFICATION;
//

// End of file