#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CIVAlarmOutDlg dialog

class CIVAlarmOutDlg : public CDialog
{
	DECLARE_DYNAMIC(CIVAlarmOutDlg)

public:
	CIVAlarmOutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVAlarmOutDlg();

// Dialog Data
	enum { IDD = IDD_IVALARMOUT };

public:
    BOOL Init(CWnd* pWnd, const CRect& rect);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    enum
    {
        ALARM_CHECK_NUM = 10,
    };
private:
    CComboBox m_AlarmOutHoldTimeComb;
    CButton m_AlarmCheck[ALARM_CHECK_NUM];
    CTreeCtrl m_CameraTree;
    virtual BOOL OnInitDialog();
};
