#pragma once


// CIVSwitchTab dialog

class CIVSwitchTab : public CDialog
{
	DECLARE_DYNAMIC(CIVSwitchTab)

public:
	CIVSwitchTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSwitchTab();

// Dialog Data
	enum { IDD = IDD_IVTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
