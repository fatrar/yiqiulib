#pragma once


// CIVRuleDlg dialog

class CIVRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVRuleDlg)

public:
	CIVRuleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVRuleDlg();

// Dialog Data
	enum { IDD = IDD_IVRULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
