#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"

#include "ScheduleCtrl.h"
#include "afxwin.h"

// CIVSchuduleDlg dialog

class CIVSchuduleDlg : 
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree,
    public IClickCameraTree,
    public CIVScheduleCfgDoc,
    public IRuleTrigger
{
	DECLARE_DYNAMIC(CIVSchuduleDlg)

public:
	CIVSchuduleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVSchuduleDlg();

// Dialog Data
	enum { IDD = IDD_IVSCHUDULE };

public:
    BOOL Init(CWnd* pWnd, const CRect& Rect);

    void Enable(BOOL bEnable = TRUE);

    // IUpdateMemu
protected:
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        const void* pData,
        HTREEITEM Item );

    // IInitCameraTree
protected:
    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item );

    // IClickCameraTree
protected:
    virtual void OnClickCameraTree(
        WhichMemu Which,
        int nChannelID,
        const void* pData,
        HTREEITEM Item );

    // IRuleTrigger
protected:
    virtual void OnRuleRemove(
        int nChannelID,
        const char* pIdentityID,
        CString& strRuleName );

    virtual void OnRuleAdd(
        int nChannelID, 
        const char* pIdentityID,
        CString& strRuleName);

    virtual void OnUseIV(
        int nChannelID,
        BOOL bEnbale){};

protected:
    void UpdateSchudule();

    void CollectUserSet(ScheduleSettings& TmpSchedule);

    void UpdateUI(const ScheduleSettings& Sch);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    
    afx_msg void OnNMRclickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedAddCheck();
    afx_msg void OnBnClickedEraseCheck();
    afx_msg void OnBnClickedApplyBt();
    
    afx_msg void OnSchuduleFull();
    afx_msg void OnSchuduleEmpty();
    afx_msg void OnSchuduleCopy();
    afx_msg void OnSchudulePaste();
    afx_msg void OnSchuduleUsetoall();
    DECLARE_MESSAGE_MAP()

    enum
    {
        Week_Day = 7,
        Ctrl_ID_Start = WM_USER,
        Week_Ctrl_X_Offset = 20,

        Week_Ctrl_Between_Off = 20,
        Week_Ctrl_Name_ID =IDS_Schu_Week_Sun,

        BT_X_Offset = 40,
        BT_Start_Y = 20,

        BT_Width = 40,
        BT_Height = 20,
    };

private:
    //CStatic m_SchuduleGroup; 
    //CTreeCtrl m_CameraTree;

    CScheduleCtrl m_ScheduleCtrl[Week_Day];
    CButton m_AddCheck;
    CButton m_EraseCheck;

    int m_nCurrentChan;
    HTREEITEM m_ClickItem;
   
    CButton m_ApplyBT; 


    ScheduleSettings* m_pScheduleSettings;
    //ScheduleSettings m_TmpSchedule;
    ScheduleSettings m_CopySchedule;
    BOOL m_bIsCopy;
};
