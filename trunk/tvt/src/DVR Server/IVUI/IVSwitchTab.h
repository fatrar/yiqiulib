#pragma once
#include "afxwin.h"


// CIVSwitchTab dialog

class CIVSwitchTab : public CDialog
{
	DECLARE_DYNAMIC(CIVSwitchTab)

public:
	CIVSwitchTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSwitchTab();

// Dialog Data
	enum { IDD = IDD_IVTAB };

public:
    BOOL Init(HWND hWnd, const CRect& rect);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void ClickTabBt(UINT ID/*, WPARAM w, LPARAM l*/);
	DECLARE_MESSAGE_MAP()

public:   
    enum
    {
        TAB_BT_NUM = 3,
    };

private:
    CButton m_TabBt[TAB_BT_NUM];
    /*CButton m_RuleTabBt;
    CButton m_AlarmOutTabBt;
    CButton m_SchuduleTabBt;*/

    CIVRuleDlg m_IVRuleDlg;
    CIVSchuduleDlg m_IVSchuduleDlg;
    CIVAlarmOutDlg m_IVAlarmOutDlg;
    
private:
    CStatic m_IVTabGroup;
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
    CStatic m_FuncGroup;
};
