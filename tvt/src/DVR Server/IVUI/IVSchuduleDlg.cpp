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
