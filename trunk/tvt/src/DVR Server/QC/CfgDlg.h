#pragma once


// CCfgDlg dialog

class CCfgDlg : public CDialog
{
	DECLARE_DYNAMIC(CCfgDlg)

public:
	CCfgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgDlg();

// Dialog Data
	enum { IDD = IDD_CFG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

private:
    //CDialog* m_pIVDlg;
};
