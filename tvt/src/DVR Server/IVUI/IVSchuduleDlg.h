#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"

// CIVSchuduleDlg dialog

class CIVSchuduleDlg : 
    public CDialog,
    public IUpdateMemu
{
	DECLARE_DYNAMIC(CIVSchuduleDlg)

public:
	CIVSchuduleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSchuduleDlg();

// Dialog Data
	enum { IDD = IDD_IVSCHUDULE };

public:
    BOOL Init(CWnd* pWnd, const CRect& rect);

    virtual void OnUpdateMemu(CMenu* pMenu,WhichMemu Which);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnNMRclickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
    CTreeCtrl m_CameraTree;
    
};
