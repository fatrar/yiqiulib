#pragma once
#include "afxwin.h"


// CIVFunctionSelDlg dialog

class CIVFunctionSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVFunctionSelDlg)

public:
	CIVFunctionSelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVFunctionSelDlg();

// Dialog Data
	enum { IDD = IDD_IVFUNCTIONSEL };

public:
    int GetUserSelect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_FunList;
    int m_nSel;
};
