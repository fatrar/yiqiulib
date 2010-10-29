#pragma once
#include "afxwin.h"


// CResToolBarDlg dialog

class CResToolBarDlg : public CDialog
{
	DECLARE_DYNAMIC(CResToolBarDlg)

public:
	CResToolBarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResToolBarDlg();

// Dialog Data
	enum { IDD = IDR_MAINFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CButton m_AddBt;
    CButton m_UnpackBt;
    CButton m_OpenBt;
    CButton m_DeleteBt;
};
