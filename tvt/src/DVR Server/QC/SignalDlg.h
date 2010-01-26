#if !defined(AFX_SIGNALDLG_H__15D0ABF5_31C9_4364_81E6_ABFBCD9529AD__INCLUDED_)
#define AFX_SIGNALDLG_H__15D0ABF5_31C9_4364_81E6_ABFBCD9529AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSignalDlg dialog

class CSignalDlg : public CDialog
{
// Construction
public:
	CSignalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSignalDlg)
	enum { IDD = IDD_DIALOGSELECTSIGNAL };
	int		m_bSignal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSignalDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALDLG_H__15D0ABF5_31C9_4364_81E6_ABFBCD9529AD__INCLUDED_)
