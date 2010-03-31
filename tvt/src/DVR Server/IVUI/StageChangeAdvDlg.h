/**H**************************************************************************
 File            : StageChangeAdvDlg.h
 Subsystem       : 
 Function Name(s): CStageChangeAdvDlg
 Author          : YiQiu
 Date            : 2010-3-29  
 Time            : 9:21
 Description     : 

   �����ԭ����ʵ�ֳ����仯���ܵĸ߼�ѡ�
    ����ʱȥ����Ĭ�ϲ����û�ȥ����
 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
************************************************.***************************H*/
#ifndef _STAGECHANGEADVDLG_H_2010_3
#define _STAGECHANGEADVDLG_H_2010_3
#pragma once
#include "afxwin.h"


// CStageChangeDlg dialog

class CStageChangeAdvDlg : public CDialog
{
	DECLARE_DYNAMIC(CStageChangeAdvDlg)

public:
	CStageChangeAdvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStageChangeAdvDlg();

// Dialog Data
	enum { IDD = IDD_STAGECHANGEADV };

public:
    void Init(WPG_Rule* pRule);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedCheckLighton();
    afx_msg void OnBnClickedCheckLightoff();
    afx_msg void OnBnClickedCheckMontion();
    afx_msg void OnBnClickedCheckOther2();
    afx_msg void OnBnClickedCheckAnything();
	DECLARE_MESSAGE_MAP()

protected:
    void SetAllCheck();

private:
    CString m_strRuleName;
    CString m_strRuleNameEdit;

    CString m_strEvent;
    CButton m_LightOnBt;
    CButton m_LightOffBt;
    CButton m_MontionBt;
    CButton m_OtherBt;
    CButton m_AnythingBt;
    
    WPG_Rule* m_pRule;
};









#endif  // _STAGECHANGEADVDLG_H_2010_


// End of file



