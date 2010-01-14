// IVSwitchTab.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSwitchTab.h"


// CIVSwitchTab dialog

IMPLEMENT_DYNAMIC(CIVSwitchTab, CDialog)

CIVSwitchTab::CIVSwitchTab(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSwitchTab::IDD, pParent)
{

}

CIVSwitchTab::~CIVSwitchTab()
{
}

void CIVSwitchTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIVSwitchTab, CDialog)
END_MESSAGE_MAP()


// CIVSwitchTab message handlers
