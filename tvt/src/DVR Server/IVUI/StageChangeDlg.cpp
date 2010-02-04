/*H***************************************************************************
 File            : StageChangeDlg.cpp
 Subsystem       : 
 Function Name(s): CStageChangeDlg
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
#include "StageChangeDlg.h"


// CStageChangeDlg dialog

IMPLEMENT_DYNAMIC(CStageChangeDlg, CDialog)

CStageChangeDlg::CStageChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStageChangeDlg::IDD, pParent)
    , m_strEvent(_T(""))
    , m_strRuleName(_T(""))
    , m_strRuleNameEdit(_T(""))
{

}

CStageChangeDlg::~CStageChangeDlg()
{
}

void CStageChangeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_Event, m_strEvent);
    DDX_Control(pDX, IDC_CHECK_Light_on, m_LightOnBt);
    DDX_Control(pDX, IDC_CHECK_Light_off, m_LightOffBt);
    DDX_Control(pDX, IDC_CHECK_Montion, m_MontionBt);
    DDX_Control(pDX, IDC_CHECK_Other2, m_OtherBt);
    DDX_Control(pDX, IDC_CHECK_Anything, m_AnythingBt);
    DDX_Text(pDX, IDC_RULENAME, m_strRuleName);
    DDX_Text(pDX, IDC_RULENAME_EDIT, m_strRuleNameEdit);
	DDV_MaxChars(pDX, m_strRuleNameEdit, Max_Rule_Name);
}


BEGIN_MESSAGE_MAP(CStageChangeDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CStageChangeDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CStageChangeDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_Light_on, &CStageChangeDlg::OnBnClickedCheckLighton)
    ON_BN_CLICKED(IDC_CHECK_Light_off, &CStageChangeDlg::OnBnClickedCheckLightoff)
    ON_BN_CLICKED(IDC_CHECK_Montion, &CStageChangeDlg::OnBnClickedCheckMontion)
    ON_BN_CLICKED(IDC_CHECK_Other2, &CStageChangeDlg::OnBnClickedCheckOther2)
    ON_BN_CLICKED(IDC_CHECK_Anything, &CStageChangeDlg::OnBnClickedCheckAnything)
END_MESSAGE_MAP()


// CStageChangeDlg message handlers

BOOL CStageChangeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Rule
    m_strRuleName.LoadString(IDS_RuleName);

    // Event
    m_strEvent.LoadString(IDS_Event);
    CString strTmp;
    strTmp.LoadString(IDS_Event_Light_on);
    m_LightOnBt.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Event_Light_off);
    m_LightOffBt.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Event_Montion);
    m_MontionBt.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Event_Other);
    m_OtherBt.SetWindowText(strTmp);
    strTmp.LoadString(IDS_Event_Anything);
    m_AnythingBt.SetWindowText(strTmp);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CStageChangeDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CStageChangeDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CStageChangeDlg::OnBnClickedCheckLighton()
{
    // TODO: Add your control notification handler code here
}

void CStageChangeDlg::OnBnClickedCheckLightoff()
{
    // TODO: Add your control notification handler code here
}

void CStageChangeDlg::OnBnClickedCheckMontion()
{
    // TODO: Add your control notification handler code here
}

void CStageChangeDlg::OnBnClickedCheckOther2()
{
    // TODO: Add your control notification handler code here
}

void CStageChangeDlg::OnBnClickedCheckAnything()
{
    // TODO: Add your control notification handler code here
}





// End of file