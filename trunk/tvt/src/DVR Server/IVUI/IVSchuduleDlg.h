#pragma once
#include "afxcmn.h"


// CIVSchuduleDlg dialog

class CIVSchuduleDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVSchuduleDlg)

public:
	CIVSchuduleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSchuduleDlg();

// Dialog Data
	enum { IDD = IDD_IVSCHUDULE };

public:
    BOOL Init(CWnd* pWnd, const CRect& rect);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CTreeCtrl m_CameraTree;
    virtual BOOL OnInitDialog();
};
