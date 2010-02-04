// IVSchuduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSchuduleDlg.h"


// CIVSchuduleDlg dialog

IMPLEMENT_DYNAMIC(CIVSchuduleDlg, CDialog)

CIVSchuduleDlg::CIVSchuduleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSchuduleDlg::IDD, pParent)
{

}

CIVSchuduleDlg::~CIVSchuduleDlg()
{
}

void CIVSchuduleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SCHUDULE_CAMERA_TREE, m_CameraTree);
}


BEGIN_MESSAGE_MAP(CIVSchuduleDlg, CDialog)
END_MESSAGE_MAP()


// CIVSchuduleDlg message handlers

BOOL CIVSchuduleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIVSchuduleDlg::Init( CWnd* pWnd, const CRect& rect )
{
     Create(CIVSchuduleDlg::IDD, pWnd);
     return TRUE;
}