#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "IVUICommon.h"


// CIVAlarmOutDlg dialog

class CIVAlarmOutDlg :
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree
{
	DECLARE_DYNAMIC(CIVAlarmOutDlg)

public:
	CIVAlarmOutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVAlarmOutDlg();

// Dialog Data
	enum { IDD = IDD_IVALARMOUT };

public:
    BOOL Init(CWnd* pWnd,const CRect& Rect);

protected:
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        void* pData);

    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnNMRclickAlarmoutCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

    enum
    {
        ALARM_CHECK_NUM = 10,
    };
private:
    CComboBox m_AlarmOutHoldTimeComb;
    CButton m_AlarmCheck[ALARM_CHECK_NUM];
    CTreeCtrl m_CameraTree;
  
private:
    CStatic m_AlarmOutGroup;
};
