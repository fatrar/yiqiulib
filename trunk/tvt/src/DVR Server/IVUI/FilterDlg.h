#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFilterDlg dialog

class CFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterDlg();

// Dialog Data
	enum { IDD = IDD_FILTER };

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedFilterOn();
    afx_msg void OnBnClickedFilterOff();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
 
private:
    CListCtrl m_list;
    CString m_strFilterInfo;
    CButton m_FiterOnBt;
    CButton m_FiterOffBt;

    CString m_strStateOff;
    CString m_strStateOn;
private:
    CString m_strMultiplier;
private:
    float m_nMultiplierEdit;
};
