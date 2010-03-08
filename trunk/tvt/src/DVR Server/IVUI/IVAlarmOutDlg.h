#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "IVUICommon.h"


// CIVAlarmOutDlg dialog

class CIVAlarmOutDlg :
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree,
    public IClickCameraTree,
    public CIVScheduleCfgDoc,
    public IRuleTrigger
{
	DECLARE_DYNAMIC(CIVAlarmOutDlg)

public:
	CIVAlarmOutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVAlarmOutDlg();

// Dialog Data
	enum { IDD = IDD_IVALARMOUT };

public:
    BOOL Init(CWnd* pWnd,const CRect& Rect);

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
        const char* pIdentityID);

    virtual void OnRuleAdd(
        int nChannelID,
        const char* pIdentityID);

    virtual void OnUseIV(
        int nChannelID,
        BOOL bEnbale);

protected:
    void UpdateChannel(int nChannelID);

    void CollectUserSet();

    BOOL IsModify();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnNMRclickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedNouseHold();
    afx_msg void OnBnClickedUseHold();
    afx_msg void OnBnClickedApplyBt();
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

    enum
    {
        Alarm_Check_Num = AlarmOutTable_Count,
        Alarm_Out_String_Start = IDS_AlarmOut_Record,

        Alarm_Out_X_Offset = 10,
        Alarm_Out_Y_Offset = 15,
       
        Alarm_Hold_Select_Y_Offset = 20,

        BT_Height   = 25,
        Combo_Width = 50,

        Alarm_Hold_Height  = Alarm_Out_Y_Offset*3+2*BT_Height,

    
    };

private:   
    // Tree
    //CStatic m_AlarmOutGroup;
    //CTreeCtrl m_CameraTree;  

    // Alarm Hold
    CStatic m_AlarmHoldGroup;
    CButton m_AlarmNoHoldBt;
    CButton m_AlarmHoldBt;
    CComboBox m_AlarmOutHoldTimeComb;

    // Choose Alarm
    CStatic m_ChooseAlarmGroup;
    CButton m_AlarmCheck[Alarm_Check_Num];

    int m_nCurrentChan;
    HTREEITEM m_ClickItem;
    
    CButton m_ApplyBT;

    AlarmOutSettings* m_pCurentAlarmSet;
    AlarmOutSettings m_TmpAlarmSet;
};
