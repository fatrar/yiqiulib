/*H***************************************************************************
 File            : RuleAddMainDlg.h
 Subsystem       : 
 Function Name(s): CRuleAddMainDlg
 Author          : YiQiu
 Date            : 2010-2-8  
 Time            : 15:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _RULEADDMAINDLG_H_2010_2
#define _RULEADDMAINDLG_H_2010_2
#include "afxwin.h"



// CRuleAddMainDlg dialog

class CRuleAddMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CRuleAddMainDlg)

public:
	CRuleAddMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRuleAddMainDlg();

// Dialog Data
	enum { IDD = IDD_Add_Rule_Main };

public:
    void SetIVRuleType(IVRuleType RuleType){ m_RuleType = RuleType; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedAdvBt();
    afx_msg void OnBnClickedFilterBt();
    afx_msg void OnBnClickedSimulationBt();

    afx_msg void OnBnClickedLineCheck();
    afx_msg void OnBnClickedZoneCheck();
    afx_msg void OnBnClickedSelectCheck();

    afx_msg void OnBnClickedRightCheck();
    afx_msg void OnBnClickedLeftCheck();
    afx_msg void OnBnClickedBothCheck();

    afx_msg void OnBnClickedColourBt();

    afx_msg void OnBnClickedPolygonCheck();
    afx_msg void OnBnClickedRectangleCheck();

private:
    enum 
    {
        Choose_Line,
        Choose_Zone
    };

    CButton m_LineBT;
    CButton m_ZoneBT;
    CButton m_SelectBT;

    CButton m_LineRightBT;
    CButton m_LineLeftBT;
    CButton m_LineBothBT;

    CButton m_ColourBT;
    CButton m_AdvBT;
    CButton m_FilterBT;
    CButton m_SimulationBT;

    CButton m_RectangleBT;
    CButton m_PolygonBT;

    int m_nToolsChoose;
    IVRuleType m_RuleType;
};








#endif  // _RULEADDMAINDLG_H_2010_


// End of file





