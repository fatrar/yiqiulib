#pragma once
#include "afxwin.h"


// CResToolBarDlg dialog

class CResToolBarDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CResToolBarDlg)

public:
	CResToolBarDlg();   // standard constructor
	virtual ~CResToolBarDlg();

// Dialog Data
	//enum { IDD = IDR_MAINFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg LONG OnInitDialog ( UINT, LONG );
    afx_msg void OnUpdateButton(CCmdUI * pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
    CButton m_AddBt;
    CButton m_UnpackBt;
    CButton m_OpenBt;
    CButton m_DeleteBt;
public:
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedUnpack();
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedDelete();
};
