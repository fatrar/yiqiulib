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
#include "RuleMainBaseDlg.h"


// CRuleAddMainDlg dialog

class CRuleAddMainDlg : 
    public CRuleMainBaseDlg
{
	//DECLARE_DYNAMIC(CRuleAddMainDlg)
public:
    virtual ~CRuleAddMainDlg() {};
//
//public:
//
//protected:
//    virtual BOOL OnInitDialog();
//    afx_msg void OnClose();
//
//    afx_msg void OnBnClickedOk();
//    afx_msg void OnBnClickedCancel();
//
//    afx_msg void OnBnClickedAdvBt();
//    afx_msg void OnBnClickedFilterBt();
//    afx_msg void OnBnClickedSimulationBt();
//
//    afx_msg void OnBnClickedLineCheck();
//    afx_msg void OnBnClickedZoneCheck();
//    afx_msg void OnBnClickedSelectCheck();
//
//    afx_msg void OnBnClickedRightCheck();
//    afx_msg void OnBnClickedLeftCheck();
//    afx_msg void OnBnClickedBothCheck();
//
//    afx_msg void OnBnClickedColourBt();
//
//    afx_msg void OnBnClickedPolygonCheck();
//    afx_msg void OnBnClickedRectangleCheck();
};


class CInvadeAddDlg:
    public CRuleAddMainDlg
{
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
};

typedef CInvadeAddDlg CLeaveDisappearAddDlg;
//class CLeaveDisappearAddDlg:
//    public CRuleAddMainDlg
//{
//protected:
//    virtual BOOL OnInitDialog();
//    virtual BOOL GatherUseSet();
//};

class CLeftBehindAddDlg:
    public CRuleAddMainDlg
{
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
};
 
typedef CLeftBehindAddDlg CLoitersAddDlg;
//class CLoitersAddDlg:
//    public CRuleAddMainDlg
//{
//protected:
//    virtual BOOL OnInitDialog();
//    virtual BOOL GatherUseSet();
//};

class CStatisticAddDlg:
    public CRuleAddMainDlg
{
    virtual BOOL OnInitDialog();
   // virtual BOOL GatherUseSet();
};

class CVehicleRetrogradeAddDlg:
    public CRuleAddMainDlg
{
    virtual BOOL OnInitDialog();
   // virtual BOOL GatherUseSet();
};

class CIllegalParkingAddDlg:
    public CRuleAddMainDlg
{
    virtual BOOL OnInitDialog();
    //virtual BOOL GatherUseSet();
};

class CStageChangeAddDlg:
    public CRuleAddMainDlg
{
    virtual BOOL OnInitDialog();
};


CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type);


#endif  // _RULEADDMAINDLG_H_2010_


// End of file





