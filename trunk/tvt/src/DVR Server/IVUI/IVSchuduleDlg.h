#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"

#include "ScheduleCtrl.h"
#include "afxwin.h"

// CIVSchuduleDlg dialog

class CIVSchuduleDlg : 
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree
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

protected:
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        void* pData,
        HTREEITEM Item );

    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnNMRclickSchuduleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedAddCheck();
    afx_msg void OnBnClickedEraseCheck();
    afx_msg void OnDestroy();
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
    CStatic m_SchuduleGroup; 
    CTreeCtrl m_CameraTree;

    CScheduleCtrl m_ScheduleCtrl[Week_Day];
    CButton m_AddCheck;
    CButton m_EraseCheck;

    int m_nCurrentChan;
    HTREEITEM m_ClickItem;
};
