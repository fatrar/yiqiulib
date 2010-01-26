// NewUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "qc.h"
#include "NewUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewUserDlg dialog


CNewUserDlg::CNewUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewUserDlg)
	m_strConfirm = _T("");
	m_strPassword = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewUserDlg)
	DDX_Text(pDX, IDC_EDITCONFIRM, m_strConfirm);
	DDX_Text(pDX, IDC_EDITPASS, m_strPassword);
	DDX_Text(pDX, IDC_EDITUSER, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewUserDlg, CDialog)
	//{{AFX_MSG_MAP(CNewUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewUserDlg message handlers

void CNewUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_strName == "")
		MessageBox("用户名不能为空!");
	else
	{
		if (m_strPassword == m_strConfirm)
			CDialog::OnOK();
		else
			MessageBox("两次输入的密码不一样!");
	}
}
