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


typedef void (*UpdateAIODataFn)(WPG_AOI_ACTION_TYPE&);

void InvadeUpdateAIOData(WPG_AOI_ACTION_TYPE& actionType);
void LeftBehindUpdateAIOData(WPG_AOI_ACTION_TYPE& actionType);

template<UpdateAIODataFn Fn, UINT nNameID>
class CLineAndRectDlg:
    public CRuleMainBaseDlg
{
public:
    CLineAndRectDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
    virtual void SimulationEnable(BOOL bEnable);
};

typedef CLineAndRectDlg<
    InvadeUpdateAIOData,IDS_Invade> CInvadeDlg;
typedef CLineAndRectDlg<
    LeftBehindUpdateAIOData,IDS_Leave_Disappear> CLeaveDisappearDlg;


template<UINT nNameID>
class CZoneDlg:
    public CRuleMainBaseDlg
{
public:
    CZoneDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
    virtual BOOL GatherUseSet();
    virtual void SimulationEnable(BOOL bEnable);
};
 
typedef CZoneDlg<IDS_LeftBehind> CLeftBehindDlg;
typedef CZoneDlg<IDS_Loiters> CLoitersAddDlg;

class CStatisticDlg:
    public CRuleMainBaseDlg
{
public:
    CStatisticDlg(CWnd* pParent = NULL):CRuleMainBaseDlg(pParent){}
protected:
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnBnClickedLineCheck();
    virtual BOOL GatherUseSet();
    virtual void SimulationEnable(BOOL bEnable);

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




