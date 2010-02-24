// IVFunctionSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVFunctionSelDlg.h"




// CIVFunctionSelDlg dialog

IMPLEMENT_DYNAMIC(CIVFunctionSelDlg, CDialog)

CIVFunctionSelDlg::CIVFunctionSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVFunctionSelDlg::IDD, pParent)
    , m_nSel(0)
{

}

CIVFunctionSelDlg::~CIVFunctionSelDlg()
{
}

void CIVFunctionSelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_FunList);
}


BEGIN_MESSAGE_MAP(CIVFunctionSelDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CIVFunctionSelDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CIVFunctionSelDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIVFunctionSelDlg message handlers

void CIVFunctionSelDlg::OnBnClickedOk()
{
    POSITION pos = m_FunList.GetFirstSelectedItemPosition(); 
    if ( pos == NULL )
    {
        AfxMessageBox(_T("Please Select a Column"));
        return;
    }

    m_nSel = m_FunList.GetNextSelectedItem(pos);
    if ( m_nSel == -1 )
    {
        AfxMessageBox(_T("Please Select a Column"));
        return;
    }
    OnOK();
}

void CIVFunctionSelDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

BOOL CIVFunctionSelDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    CString strHead; 
    strHead.LoadString(g_hmodule, IDS_Type);
    m_FunList.InsertColumn(0, strHead,LVCFMT_LEFT, 110);
    strHead.LoadString(g_hmodule, IDS_Description);
    m_FunList.InsertColumn(1, strHead, LVCFMT_LEFT, 450);

    CString strFunctionName;
    for (int i = 0; i< IV_Name_Number; ++i)
    {
        BOOL bRc = strFunctionName.LoadString(g_hmodule, IV_Name_StringID_Start+i);
        m_FunList.InsertItem(i, strFunctionName);
        strFunctionName.LoadString(g_hmodule, IV_Name_DIR_StringID_Start+i);
        m_FunList.SetItemText(i, 1, strFunctionName);
    }

    m_FunList.SetExtendedStyle(m_FunList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    //m_FunList.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
    m_FunList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

IVRuleType CIVFunctionSelDlg::GetUserSelect()
{
    if ( m_nSel == -1 ||
         m_nSel > IV_Stage_Change )
    {
        return IV_UnKnown;
    }

    return (IVRuleType)m_nSel;
}