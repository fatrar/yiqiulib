/*H***************************************************************************
 File            : ZoneAdvDlg.h
 Subsystem       : 
 Function Name(s): CZoneAdvDlg
 Author          : YiQiu
 Date            : 2010-1-28  
 Time            : 11:47
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _ZONEADVDLG_H_2010_
#define _ZONEADVDLG_H_2010_

#include "afxwin.h"


// CZoneAdvDlg dialog

class CZoneAdvDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoneAdvDlg)

public:
	CZoneAdvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZoneAdvDlg();

// Dialog Data
	enum { IDD = IDD_ZONEADV };

public:
    void Init(IVRuleType type, WPG_Rule* pRule);
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();  
    afx_msg void OnBnClickedCheckAll();  // click All check button
    afx_msg void OnBnClickedCheckPerson();
    afx_msg void OnBnClickedCheckVehicle();
    afx_msg void OnBnClickedCheckOther();

    afx_msg void OnBnClickedVView();
    afx_msg void OnBnClickedPView();
	DECLARE_MESSAGE_MAP()

private:
    CString m_StrRuleName;
    CString m_strRuleNameEdit;
    CString m_strDefaultRuleName;

    CString m_strView;
    CButton m_VerticalBt;
    CButton m_ParallelBt;
    
    CString m_strObject;
    CButton m_PersonCheck;
    CButton m_VehicleCheck;
    CButton m_OtherCheck;
    CButton m_AllCheck;

    CString m_StrTime;
    CString m_strLoiters;
    CString m_strLeftBehind;
    int m_nLoitersEdit;      // Max 10 min
    int m_nLeftBehindEdit;   // Max 10 min  
   
    WPG_Rule* m_pRule;
    IVRuleType m_type;

protected:
    void SetAllCheck();  
    void HideAllTimeWindow();
    void HideObjAndView();

    void GetObjSet();
    void GetViewSet();

    void UpdateUIByRule();
    void SetObjSet();
    void SetViewSet(); 
};









#endif  // _ZONEADVDLG_H_2010_


// End of file