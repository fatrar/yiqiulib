// SignalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "qc.h"
#include "SignalDlg.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CSignalDlg dialog


CSignalDlg::CSignalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSignalDlg)
	m_bSignal = -1;
	//}}AFX_DATA_INIT
}


void CSignalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignalDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_bSignal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSignalDlg, CDialog)
	//{{AFX_MSG_MAP(CSignalDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalDlg message handlers

void CSignalDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CSignalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    ((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	//UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSignalDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(TRUE);
	CDialog::OnCancel();
}
