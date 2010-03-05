// WindowsTestDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "..\Windows\Include\TextStatic.h"
using namespace OCI::Windows;

// CWindowsTestDlg dialog
class CWindowsTestDlg : public CDialog
{
// Construction
public:
	CWindowsTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WINDOWSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
    IDrawContainer* m_pDrawContainer;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedColour();
    CAlarmTextStatic m_testStatic;
    IDrawer* m_pDrawer;
    IDrawer* m_pDrawer2;
};
