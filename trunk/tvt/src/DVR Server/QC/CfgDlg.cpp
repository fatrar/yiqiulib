// CfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QC.h"
#include "CfgDlg.h"


// CCfgDlg dialog

IMPLEMENT_DYNAMIC(CCfgDlg, CDialog)

CCfgDlg::CCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCfgDlg::IDD, pParent)
    , m_pIVDlg(NULL)
{

}

CCfgDlg::~CCfgDlg()
{
}

void CCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCfgDlg, CDialog)
END_MESSAGE_MAP()


// CCfgDlg message handlers

BOOL CCfgDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect rect;
    GetClientRect(&rect);
    m_pIVDlg = CreateIVConfigDlg(this, rect);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
