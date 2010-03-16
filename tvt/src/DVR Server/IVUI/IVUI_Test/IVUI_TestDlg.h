// IVUI_TestDlg.h : header file
//

#if !defined(AFX_IVUI_TESTDLG_H__03346926_1C57_4492_ABFF_501D4E6D2187__INCLUDED_)
#define AFX_IVUI_TESTDLG_H__03346926_1C57_4492_ABFF_501D4E6D2187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIVUI_TestDlg dialog

class CIVUI_TestDlg : public CDialog
{
// Construction
public:
	CIVUI_TestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIVUI_TestDlg)
	enum { IDD = IDD_IVUI_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVUI_TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIVUI_TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//CIVSwtichTab m_IVSwtichTab;
    ISnapShotSender* m_pISnapShotSender;
    afx_msg void OnBnClickedTest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVUI_TESTDLG_H__03346926_1C57_4492_ABFF_501D4E6D2187__INCLUDED_)
