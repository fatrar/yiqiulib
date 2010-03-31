/*H***************************************************************************
 File            : StageChangeAdvDlg.cpp
 Subsystem       : 
 Function Name(s): CStageChangeAdvDlg
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
#include "StageChangeAdvDlg.h"


// CStageChangeDlg dialog

IMPLEMENT_DYNAMIC(CStageChangeAdvDlg, CDialog)

CStageChangeAdvDlg::CStageChangeAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStageChangeAdvDlg::IDD, pParent)
    , m_strEvent(_T(""))
    , m_strRuleName(_T(""))
    , m_strRuleNameEdit(_T(""))
    , m_pRule(NULL)
{

}

CStageChangeAdvDlg::~CStageChangeAdvDlg()
{
}

void CStageChangeAdvDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CStageChangeAdvDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CStageChangeAdvDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CStageChangeAdvDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK_Light_on, &CStageChangeAdvDlg::OnBnClickedCheckLighton)
    ON_BN_CLICKED(IDC_CHECK_Light_off, &CStageChangeAdvDlg::OnBnClickedCheckLightoff)
    ON_BN_CLICKED(IDC_CHECK_Montion, &CStageChangeAdvDlg::OnBnClickedCheckMontion)
    ON_BN_CLICKED(IDC_CHECK_Other2, &CStageChangeAdvDlg::OnBnClickedCheckOther2)
    ON_BN_CLICKED(IDC_CHECK_Anything, &CStageChangeAdvDlg::OnBnClickedCheckAnything)
END_MESSAGE_MAP()


// CStageChangeDlg message handlers

BOOL CStageChangeAdvDlg::OnInitDialog()
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

    //typedef enum WPG_SCENE_CHANGE_TYPE
    //{
    //    SCENE_CHANGE_LIGHT_ON         =   1,
    //    SCENE_CHANGE_LIGHT_OFF        =   2,
    //    SCENE_CHANGE_LIGHT_ON_OR_OFF  =   3,
    //    SCENE_CHANGE_UNKNOWN          =   4,
    //    SCENE_CHANGE_CAMERA_MOTION    =   8,
    //    SCENE_CHANGE_ANYTHING         =   15
    //} WPG_SCENE_CHANGE_TYPE;

    do 
    {
        unsigned int& nTest = m_pRule->ruleDescription.description.sceneChangeEventDescription.sceneChangeType;
        if ( nTest == SCENE_CHANGE_ANYTHING )
        {
            SetAllCheck();
            break;
        }

        int i = 0;
        if ( nTest & SCENE_CHANGE_LIGHT_ON )
        {
            m_LightOnBt.SetCheck(BST_CHECKED);
            ++i;
        }
        if ( nTest & SCENE_CHANGE_LIGHT_OFF )
        {
            m_LightOffBt.SetCheck(BST_CHECKED);
            ++i;
        }
        if ( nTest & SCENE_CHANGE_UNKNOWN )
        {
            m_OtherBt.SetCheck(BST_CHECKED);
            ++i;
        }
        if ( nTest & SCENE_CHANGE_CAMERA_MOTION )
        {
            m_MontionBt.SetCheck(BST_CHECKED);
            ++i;
        }
        
        if ( i == 4 )
        {
            m_AnythingBt.SetCheck(BST_CHECKED);
            nTest = SCENE_CHANGE_ANYTHING;
        }
    } 
    while (0);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CStageChangeAdvDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CStageChangeAdvDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CStageChangeAdvDlg::OnBnClickedCheckLighton()
{
    if ( BST_UNCHECKED == m_LightOnBt.GetCheck() )
    {
        m_AnythingBt.SetCheck(BST_UNCHECKED);
    }
}

void CStageChangeAdvDlg::OnBnClickedCheckLightoff()
{
    if ( BST_UNCHECKED == m_LightOffBt.GetCheck() )
    {
        m_AnythingBt.SetCheck(BST_UNCHECKED);
    }
}

void CStageChangeAdvDlg::OnBnClickedCheckMontion()
{
    if ( BST_UNCHECKED == m_MontionBt.GetCheck() )
    {
        m_AnythingBt.SetCheck(BST_UNCHECKED);
    }
}

void CStageChangeAdvDlg::OnBnClickedCheckOther2()
{
    if ( BST_UNCHECKED == m_OtherBt.GetCheck() )
    {
        m_AnythingBt.SetCheck(BST_UNCHECKED);
    }
}

void CStageChangeAdvDlg::OnBnClickedCheckAnything()
{
    if ( BST_CHECKED == m_AnythingBt.GetCheck() )
    {
        SetAllCheck();
    }
}

void CStageChangeAdvDlg::SetAllCheck()
{
    m_LightOnBt.SetCheck(BST_CHECKED);
    m_LightOffBt.SetCheck(BST_CHECKED);
    m_MontionBt.SetCheck(BST_CHECKED);
    m_OtherBt.SetCheck(BST_CHECKED);
    m_AnythingBt.SetCheck(BST_CHECKED);
}

void CStageChangeAdvDlg::Init( WPG_Rule* pRule )
{
    m_pRule = pRule;
}



// End of file