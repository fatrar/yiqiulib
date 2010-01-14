#pragma once


// CIVAlarmOutDlg dialog

class CIVAlarmOutDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVAlarmOutDlg)

public:
	CIVAlarmOutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVAlarmOutDlg();

// Dialog Data
	enum { IDD = IDD_IVALARMOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
