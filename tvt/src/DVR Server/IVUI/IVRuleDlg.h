#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"


// CIVRuleDlg dialog

class CIVRuleDlg :
    public CDialog,
    public IUpdateMemu
{
	DECLARE_DYNAMIC(CIVRuleDlg)

public:
	CIVRuleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVRuleDlg();

// Dialog Data
	enum { IDD = IDD_IVRULE };

public:
    BOOL Init(CWnd* pWnd, const CRect& rect);
    
    virtual void OnUpdateMemu(CMenu* pMenu,WhichMemu Which);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnNMRclickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRuleEnableallrule();
    afx_msg void OnRuleDisableallrule();
    afx_msg void OnRuleUse();
    afx_msg void OnRuleShowobject();
    afx_msg void OnRuleShowtrace();
    afx_msg void OnRuleShowobjtrace();
    afx_msg void OnRuleNewrule();
	DECLARE_MESSAGE_MAP()

private:
    CTreeCtrl m_CameraTree;
};
