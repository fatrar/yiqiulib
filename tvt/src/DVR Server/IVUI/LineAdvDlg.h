/*H***************************************************************************
 File            : LineAdvDlg.h
 Subsystem       : 
 Function Name(s): CLineAdvDlg
 Author          : YiQiu
 Date            : 2010-1-28  
 Time            : 11:45
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _LINEADVDLG_H_2010_
#define _LINEADVDLG_H_2010_

// CLineAdvDlg dialog

class CLineAdvDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineAdvDlg)

public:
	CLineAdvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLineAdvDlg();

// Dialog Data
	enum { IDD = IDD_LINEADV };

    void Init(WPG_Rule* pRule);

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheckPerson();
    afx_msg void OnBnClickedCheckVehicle();
    afx_msg void OnBnClickedCheckOther();
    afx_msg void OnBnClickedCheckAll();
	DECLARE_MESSAGE_MAP()

private:
    CString m_strRuleName;
    CString m_strRuleNameEdit;
    CString m_strDefaultRuleName;

    CString m_strObject;
    CButton m_PersonCheck;
    CButton m_VehicleCheck;
    CButton m_OtherCheck;
    CButton m_AllCheck;
private:
    void CLineAdvDlg::SetAllCheck();

    WPG_Rule* m_pRule;
};









#endif  // End of file





