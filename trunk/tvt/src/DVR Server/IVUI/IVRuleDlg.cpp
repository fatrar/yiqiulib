// IVRuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVRuleDlg.h"


// CIVRuleDlg dialog

IMPLEMENT_DYNAMIC(CIVRuleDlg, CDialog)

CIVRuleDlg::CIVRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVRuleDlg::IDD, pParent)
{

}

CIVRuleDlg::~CIVRuleDlg()
{
}

void CIVRuleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RULE_CAMERA_TREE, m_CameraTree);
}


BEGIN_MESSAGE_MAP(CIVRuleDlg, CDialog)
END_MESSAGE_MAP()


// CIVRuleDlg message handlers
