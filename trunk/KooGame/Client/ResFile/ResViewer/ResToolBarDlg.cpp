// ResToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResViewer.h"
#include "ResToolBarDlg.h"


// CResToolBarDlg dialog

IMPLEMENT_DYNAMIC(CResToolBarDlg, CDialog)

CResToolBarDlg::CResToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResToolBarDlg::IDD, pParent)
{

}

CResToolBarDlg::~CResToolBarDlg()
{
}

void CResToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_Add, m_AddBt);
    DDX_Control(pDX, IDC_Unpack, m_UnpackBt);
    DDX_Control(pDX, IDC_Open, m_OpenBt);
    DDX_Control(pDX, IDC_Delete, m_DeleteBt);
}


BEGIN_MESSAGE_MAP(CResToolBarDlg, CDialog)
END_MESSAGE_MAP()


// CResToolBarDlg message handlers
