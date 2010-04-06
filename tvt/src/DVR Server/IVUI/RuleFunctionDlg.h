/*H***************************************************************************
 File            : RuleFunctionDlg.h
 Subsystem       : 
 Function Name(s): 
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
//#include "afxwin.h"
//#include "RuleMainBaseDlg.h"


// CRuleAddMainDlg dialog

class CInvadeDlg:
    public CRuleMainBaseDlg
{
public:
    CInvadeDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
    virtual void SimulationEnable(BOOL bEnable);
};

typedef CInvadeDlg CLeaveDisappearDlg;


class CLeftBehindDlg:
    public CRuleMainBaseDlg
{
public:
    CLeftBehindDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
    virtual void SimulationEnable(BOOL bEnable);
};
 
typedef CLeftBehindDlg CLoitersAddDlg;

class CStatisticDlg:
    public CRuleMainBaseDlg
{
public:
    CStatisticDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
   // virtual BOOL GatherUseSet();

};

class CVehicleRetrogradeDlg:
    public CRuleMainBaseDlg
{
public:
    CVehicleRetrogradeDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
   // virtual BOOL GatherUseSet();
};

class CIllegalParkingDlg:
    public CRuleMainBaseDlg
{
public:
    CIllegalParkingDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:  
    virtual BOOL OnInitDialog();
    //virtual BOOL GatherUseSet();
};

class CStageChangeDlg:
    public CRuleMainBaseDlg
{
public:
    CStageChangeDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:   
    virtual BOOL OnInitDialog();
};


CRuleMainBaseDlg* CreateRuleCfgDlgByRule(IVRuleType type,  CWnd* pParentWnd);


#endif  // _RULEADDMAINDLG_H_2010_


// End of file





