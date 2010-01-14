#pragma once


// CIVSchuduleDlg dialog

class CIVSchuduleDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVSchuduleDlg)

public:
	CIVSchuduleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSchuduleDlg();

// Dialog Data
	enum { IDD = IDD_IVSCHUDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
