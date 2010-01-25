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
    BOOL Init(CWnd* pWnd, const CRect& rect);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:   
    afx_msg void ClickTabBt(UINT ID/*, WPARAM w, LPARAM l*/);

    enum
    {
        TAB_BT_NUM = 3,
    };
private:
    CButton m_TabBt[TAB_BT_NUM];
    /*CButton m_RuleTabBt;
    CButton m_AlarmOutTabBt;
    CButton m_SchuduleTabBt;*/
};
