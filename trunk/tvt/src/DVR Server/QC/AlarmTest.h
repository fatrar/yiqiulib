#if !defined(AFX_ALARMTEST_H__DFFF126E_619F_482C_95A4_6B64ABC07E2D__INCLUDED_)
#define AFX_ALARMTEST_H__DFFF126E_619F_482C_95A4_6B64ABC07E2D__INCLUDED_

#include "vfw.h"
#include "DEVICECONTROL\DeviceControl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmTest dialog
class CAlarmTest : public CDialog
{
// Construction
public:
	void ManueAlarmOut();
	BOOL m_bAutoAlarmOut;
	int alarmrelay;
	int alarmout;
	int alarmin;
	void InitialIn(int num);
	void InitialRelay(BOOL have);
	void InitialOut(int num);
	void InitialControl();
	BOOL MakeAlarmCardOutStatus(DWORD * out);
	DWORD m_in;
	DWORD m_out;
	long npf(int d,int n);
	BOOL isRightData(char c,int jz);
	long StrToData(CString strData,int jz);
	void SetKeyBoard(DWORD in);
	DWORD m_AlarmOutStatus[5];
	DWORD m_AlarmInStatus[16];
//	BOOL m_AlarmIn[16];
	void InitialVar();
	BOOL m_CheckAlarm[TEST_ALARM_TYPE_NUMBER];
	void RefreshControlList(DWORD * AlarmIn,DWORD * AlarmOut);
	CWnd* m_parent;
	CAlarmTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlarmTest)
	enum { IDD = IDD_ALARM_DLG };
	CButton	m_relay_capture;
	CButton	m_out_capture;
	CButton	m_in_capture;
	CButton	m_auto_test;
	CButton	m_c_alarm_out_res;
	CButton	m_c_alarm_relay_res;
	CButton	m_c_alarm_in_res;
	CButton	m_C_alarm_out_j;
	CButton	m_C_alarm_out_4;
	CButton	m_C_alarm_out_3;
	CButton	m_C_alarm_out_2;
	CButton	m_C_alarm_out_1;
	CButton	m_C_alarm_in_1;
	CButton	m_C_alarm_in_2;
	CButton	m_C_alarm_in_3;
	CButton	m_C_alarm_in_4;
	CButton	m_C_alarm_in_5;
	CButton	m_C_alarm_in_6;
	CButton	m_C_alarm_in_7;
	CButton	m_C_alarm_in_8;
	CButton	m_C_alarm_in_9;
	CButton	m_C_alarm_in_10;
	CButton	m_C_alarm_in_11;
	CButton	m_C_alarm_in_12;
	CButton	m_C_alarm_in_13;
	CButton	m_C_alarm_in_14;
	CButton	m_C_alarm_in_15;
	CButton	m_C_alarm_in_16;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmTest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlarmTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnFinish();
	afx_msg void OnAlarmInRes();
	afx_msg void OnAlarmOutRes();
	afx_msg void OnAlarmRelayRes();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAutoTest();
	afx_msg void OnLampAlarmJ();
	afx_msg void OnLampAlarmOut1();
	afx_msg void OnLampAlarmOut2();
	afx_msg void OnLampAlarmOut4();
	afx_msg void OnLampAlarmOut3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMTEST_H__DFFF126E_619F_482C_95A4_6B64ABC07E2D__INCLUDED_)
