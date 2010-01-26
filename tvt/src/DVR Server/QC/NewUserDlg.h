#if !defined(AFX_NEWUSERDLG_H__BB3261C0_63AC_4EA9_B14F_91DBC0AD7FAF__INCLUDED_)
#define AFX_NEWUSERDLG_H__BB3261C0_63AC_4EA9_B14F_91DBC0AD7FAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewUserDlg dialog

class CNewUserDlg : public CDialog
{
// Construction
public:
	CNewUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewUserDlg)
	enum { IDD = IDD_DIALOGNEWUSER };
	CString	m_strConfirm;
	CString	m_strPassword;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewUserDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWUSERDLG_H__BB3261C0_63AC_4EA9_B14F_91DBC0AD7FAF__INCLUDED_)
