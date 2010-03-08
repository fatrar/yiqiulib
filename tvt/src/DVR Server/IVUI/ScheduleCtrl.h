#if !defined(AFX_COSTOMCTRL_H__9DF4C32F_B561_4D3F_B63F_91FF3F04B500__INCLUDED_)
#define AFX_COSTOMCTRL_H__9DF4C32F_B561_4D3F_B63F_91FF3F04B500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CostomCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CScheduleCtrl window
#include "Afxtempl.h"
#include "Resource.h"  
//#include "../netheader/netconfigdata.h"

#define COLOR_WHITE_FLAG     0

#define MINUTE_SCALE         5
#define HOUR_SCALE           4*MINUTE_SCALE
#define LEFT_NOSCALE         12
#define DAY_HOURS            24
#define HOUR_SCALE_HEIGHT    8
#define MINUTE_SCALE_HEIGHT  4
#define ONE_HOUR_SCALE_NUM   4
#define PER_SCALE_MINUTES    15

#define MOVE_RECT_DIS        1
#define SHOW_TIME_RECT_DIS   6
#define SELECT_TIME_RECT_DIS 3
#define PER_HOUR_SEC         3600
#define PER_MINUTE_SEC       60

#define COLOR_BLUE           RGB(0, 0, 255)
#define COLOR_WHITE          RGB(255, 255, 255)
#define COLOR_BLACK          RGB(0, 0, 0)
#define COLOR_LIGHT_BLUE     RGB(200, 200, 255)
#define COLOR_YELLOW         RGB(192, 192, 0)
#define DECREASE_SIZE        0
#define DAY_QUARTER_NUM      96
#define PER_ULONG_BIT			 32

#define UM_SCHEDULE_MODIFY      WM_USER + 9999

typedef struct  _week_time_section
{
		long startTimeSec;//开始时间，从0:0:0开始的秒数
		long endTimeSec; //结束时间，从0:0:0开始的秒数
}WEEKTIMESECTION, *PWEEKTIMESECTION;


typedef CList<CRect,CRect> CRECTLIST;
typedef CList<WEEKTIMESECTION, WEEKTIMESECTION> TIMESECTLIST;

class CScheduleCtrl : public CWnd
{
// Construction
public:
	CScheduleCtrl();

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	static void InitCursor();

    //设置控件名
    void SetTitle(const CString& sTitle);

	CString GetTitle();

    ///////控件接口函数////////

	void SetTimeSec(Scheduleday *ptime);
	int GetTimeSec(Scheduleday *ptimelist);


	//获得一天24小时每一刻钟的状态
	void GetTimeStatus(ULONG &Status0, ULONG &Status1, ULONG &Status2);
	//设置一天24小时每一刻钟的状态
	void SetTimeStatus(ULONG Status0, ULONG Status1, ULONG Status2 );

	//获得操作模式(添加或删除)
	bool GetOperateMode();
	//设置操作模式(添加或删除)
	void SetOperateMode(bool bModeFlag);

	BOOL IsModified();
	void ResetModifyFlag();
	/*unsigned int m_addcurID;
	unsigned int m_delcurID;*/
/////////////////////
	virtual ~CScheduleCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScheduleCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void LeaveOrUpDraw();
	void AddDeleteSect();
	void DrawShowTimeRect();
	void MoveNoDown(CPoint point);
	void TimeSectToRect(WEEKTIMESECTION &TimeSect, CRect &rect);
	void EnsureSectNum(TIMESECTLIST &TimeSect, int &nNum);
	//1010
	ULONG m_EightHourStatus[3];
	bool m_bQuarStatus[DAY_QUARTER_NUM];
	///////
	void CompareDelRect(CRect rcSelect, CRECTLIST &NewList);
	void CopyList(CRECTLIST &List);
	void CompareSelectRect(CRect rcSelect, CRECTLIST &NewList);
	void CompareListElement(CRECTLIST &NewList, CRECTLIST &LastList);
	void DelSelectTimeSect(CRect rcSelect);
	void AddSelectTimeSect(CRect rcSelect);
	void GetRectTime(CPoint RealPoint, CRect rect, int &nStartHour, int &nStartMinute, int &nEndHour, int &nEndMinute);
	void GetPointTime(CPoint point, int &nStartHour, int &nStartMinute, int &nEndHour, int &nEndMinute);
	void GetLeftRightScalePoint(CPoint RealPoint, CPoint &LeftScalePoint, CPoint &RightScalePoint);
	void DrawMouseMoveRec(CPoint RealPoint, CPoint FrontPoint, CPoint BackPoint);

private:
	int m_nHourScale;
	int m_nMinuteScale;
	int m_nLeftNoScale;
	int m_nRightNoScale;

	CRect m_rcLeftTop;
	CRect m_rcLeftBottom;
	CRect m_rcRightTop;
	CRect m_rcRightBottom;

	CRect m_rcMove;
	CRect m_rcShowTime;
	CRect m_rcSelectTime;//1013
	bool m_bDownInFlag;//1013

	BOOL m_bCaptured;
	bool m_bDown;

	CPoint m_DownPoint;
	CPoint m_MovePrePoint;
	CPoint m_BlackDotPoint;
	CPoint m_MouseLastPoint;

	CBrush m_brBlue, m_brWhite;
	CPen m_penSolidBlue, m_penSolidWhite;

	int m_nStartHour;
	int m_nStartMinute;
	int m_nEndHour;
	int m_nEndMinute;

	void InitRect(const CRect rect);
	void ShowTitle(CString string);

	CList<CRect, CRect>m_cRectList;
	bool m_bModeFlag;
	bool m_bCursorFlag;
	CString m_sTitle;
	
	static HCURSOR m_hAddCursor;
	static HCURSOR m_hEraseCursor;
	
	BOOL	m_bModified;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COSTOMCTRL_H__9DF4C32F_B561_4D3F_B63F_91FF3F04B500__INCLUDED_)
