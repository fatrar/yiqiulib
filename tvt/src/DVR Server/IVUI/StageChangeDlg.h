#pragma once
#include "afxwin.h"


// CStageChangeDlg dialog

class CStageChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CStageChangeDlg)

public:
	CStageChangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStageChangeDlg();

// Dialog Data
	enum { IDD = IDD_STAGECHANGEADV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()


private:
    CString m_strRuleName;
    CString m_strRuleNameEdit;

    CString m_strEvent;
    CButton m_LightOnBt;
    CButton m_LightOffBt;
    CButton m_MontionBt;
    CButton m_OtherBt;
    CButton m_AnythingBt;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCheckLighton();
    afx_msg void OnBnClickedCheckLightoff();
    afx_msg void OnBnClickedCheckMontion();
    afx_msg void OnBnClickedCheckOther2();
    afx_msg void OnBnClickedCheckAnything();
};
