// CostomCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "Schedule.h"
#include "ScheduleCtrl.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

HCURSOR CScheduleCtrl::m_hAddCursor = NULL;
HCURSOR CScheduleCtrl::m_hEraseCursor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CCostomCtrl

CScheduleCtrl::CScheduleCtrl()
{
	m_nMinuteScale = MINUTE_SCALE;
	m_nHourScale = HOUR_SCALE;
	m_nLeftNoScale = LEFT_NOSCALE;
	m_nRightNoScale = 0;

	m_bCaptured = false;
	m_DownPoint = (0,0);
	m_MovePrePoint = (0,0);

	m_brBlue.CreateSolidBrush(COLOR_BLUE);
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
	m_penSolidBlue.CreatePen(PS_SOLID, 1, COLOR_BLUE);
	m_penSolidWhite.CreatePen(PS_SOLID, 1, COLOR_WHITE);
	

	m_nStartHour = 0;
	m_nStartMinute = 0;
	m_nEndHour = 0;
	m_nEndMinute = 0;

	m_bModeFlag = true; // 默认为写模式
	m_bCursorFlag = 0;
	
	m_bDownInFlag = 0;//1013
	m_bDown = false;
	m_bCaptured = false;

	// load button cursorLoadCursor(,MAKEINTRESOURCE(IDC_CURSOR_ADD));//
	
	m_bModified = FALSE;
}

CScheduleCtrl::~CScheduleCtrl()
{
	m_brBlue.DeleteObject();
	m_brWhite.DeleteObject();
	m_penSolidBlue.DeleteObject();
	m_penSolidWhite.DeleteObject();

}
void CScheduleCtrl::InitCursor()
{
    //m_hAddCursor = theApp.LoadCursor(IDC_CURSOR_ADD);
    //m_hEraseCursor = theApp.LoadCursor(IDC_CURSOR_CLEAR);  
	m_hAddCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ADD));
	m_hEraseCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CLEAR));
}

BEGIN_MESSAGE_MAP(CScheduleCtrl, CWnd)
	//{{AFX_MSG_MAP(CScheduleCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_SIZING()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduleCtrl message handlers

void CScheduleCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd::PreSubclassWindow();
	
	OnSize(0, 0, 0);
}

void CScheduleCtrl::InitRect(const CRect rect)
{
	m_rcLeftTop.left = rect.left;
	m_rcLeftTop.top = rect.top;
	m_rcLeftTop.right = (rect.Width( ))/6;
	m_rcLeftTop.bottom = (rect.Height( ))/2;

	m_rcLeftBottom.left = rect.left;
	m_rcLeftBottom.top = rect.top + (rect.Height( ))/2;
	m_rcLeftBottom.right = (rect.Width( ))/6;
	m_rcLeftBottom.bottom = rect.bottom;

	m_rcRightTop.left = rect.left + (rect.Width( ))/6;
	m_rcRightTop.top = rect.top;
	m_rcRightTop.right = rect.right;
	m_rcRightTop.bottom = (rect.Height( ))/2;

	m_nHourScale = (int)((m_rcRightTop.Width()-12)/24.0);
	m_nMinuteScale = (int)(m_nHourScale/4.0);
	m_nLeftNoScale = (m_rcRightTop.Width() - m_nHourScale*24)/2;

	m_rcRightBottom.left = rect.left + (rect.Width( ))/6;
	m_rcRightBottom.top = rect.top + (rect.Height( ))/2;
	m_rcRightBottom.right = rect.right;
	m_rcRightBottom.bottom = rect.bottom;

}

void CScheduleCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//画四个矩形
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);

	CPen *pOldPen = dc.SelectObject(&penBlack);
	CBrush *pOldBrush = dc.SelectObject(&m_brWhite);

	dc.Rectangle(m_rcLeftTop);
	dc.Rectangle(m_rcLeftBottom);
	dc.Rectangle(m_rcRightTop);
	dc.Rectangle(m_rcRightBottom);
	
	penBlack.DeleteObject();

	///在m_rcLeftBottom里写字符串
	//ShowTitle(m_sTitle);	
	ShowTitle(m_sTitle);//1017
	///////////
	//画刻度
	CString strScale;

	for(int i=0; i<=DAY_HOURS; i++)
	{

		CRect rect(
			m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale-3,
			m_rcRightTop.top+1,
			m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale+10,
			m_rcRightTop.bottom -8
			);//用于写刻度数

		//画刻度线
		pOldPen = dc.SelectObject(&m_penSolidBlue);	
		dc.MoveTo(m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale, m_rcRightTop.bottom);
		dc.LineTo(m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale, m_rcRightTop.bottom-HOUR_SCALE_HEIGHT);

		if (i<DAY_HOURS)
		{
			for(int j=0; j<ONE_HOUR_SCALE_NUM; j++)
			{
				dc.MoveTo(m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale+j*m_nMinuteScale, m_rcRightTop.bottom);
				dc.LineTo(m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale+j*m_nMinuteScale, m_rcRightTop.bottom-MINUTE_SCALE_HEIGHT);			
			}
		}

		//写刻度数据
		strScale.Format(_T("%d"),i);
		CFont    font;
		font.CreateFont(-10, 0, 0, 0, 300, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH|FF_SWISS,NULL);
		CFont *Oldfont = dc.SelectObject(&font);
		dc.DrawText(strScale, rect, DT_LEFT );		
		dc.SelectObject(Oldfont);
		font.DeleteObject();
		
	}

	m_nRightNoScale = m_rcRightBottom.right - (m_rcRightTop.left+m_nLeftNoScale+DAY_HOURS*m_nHourScale);

	///画列表中的矩形
	CRect rcList;
	pOldBrush = dc.SelectObject(&m_brBlue);
	pOldPen = dc.SelectObject(&m_penSolidBlue);

	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList = m_cRectList.GetNext(pos);
		dc.Rectangle(rcList);
	}

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CScheduleCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	//画列表中的矩形
/*	CRect rcList;//画不出来
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);
	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);

	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList = m_cRectList.GetNext(pos);
		pDC->Rectangle(rcList);
	}
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
*/
	return TRUE;//return CWnd::OnEraseBkgnd(pDC);//
}

void CScheduleCtrl::DrawMouseMoveRec(CPoint RealPoint, CPoint FrontPoint, CPoint BackPoint)
{
	CDC* pDC = GetDC();
	//m_rcMove为从m_DownPoint到鼠标所移动到的点所形成的矩形
	m_rcMove.top = m_rcRightBottom.top+MOVE_RECT_DIS;
	m_rcMove.bottom = m_rcRightBottom.bottom-MOVE_RECT_DIS;
	m_rcMove.left = 0;
	m_rcMove.right = 0;//不一样

	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);
			
	if (RealPoint.x > m_DownPoint.x)//左往右
	{
		if (m_MovePrePoint.x < RealPoint.x)//左往右
		{
			m_rcMove.left = m_DownPoint.x;
			m_rcMove.right = BackPoint.x;	
		}
		else//右往左
		{
			m_rcMove.left = m_DownPoint.x;
			m_rcMove.right = FrontPoint.x;		
		}
	}
	else//右往左
	{
		if (m_MovePrePoint.x > RealPoint.x)//右往左
		{
			m_rcMove.left = FrontPoint.x;
			m_rcMove.right = m_DownPoint.x;	
		}
		else//左往右
		{
			m_rcMove.left = BackPoint.x;
			m_rcMove.right = m_DownPoint.x;	
		}
	}

	//1013
	m_rcSelectTime.left = m_rcMove.left;
	m_rcSelectTime.top = m_rcRightBottom.top + SELECT_TIME_RECT_DIS;
	m_rcSelectTime.right = m_rcMove.right;
	m_rcSelectTime.bottom = m_rcRightBottom.bottom - SELECT_TIME_RECT_DIS; 

	////////

	//画m_rcRightBottom全部区域
	CPen penSolidBlack;
	penSolidBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);

	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&penSolidBlack);
	pDC->Rectangle(m_rcRightBottom);
	
	penSolidBlack.DeleteObject();

	//画列表中的矩形
	CRect rcList;
	pOldBrush = pDC->SelectObject(&m_brBlue);
	pOldPen = pDC->SelectObject(&m_penSolidBlue);
	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList = m_cRectList.GetNext(pos);
		pDC->Rectangle(rcList);
	}

	//画当前鼠标移动所形成的矩形
	CBrush brLight;
	brLight.CreateSolidBrush(COLOR_LIGHT_BLUE);
	pOldBrush = pDC->SelectObject(&brLight);
	
	pDC->Rectangle(m_rcMove);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	
	ReleaseDC(pDC);
}

void CScheduleCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//return;
	//SetCapture();//1013
	
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}

	CDC* pDC = GetDC();
    pDC->DPtoLP(&point);
	pDC->DPtoLP(&m_rcRightBottom);

	m_nStartHour =0;
	m_nStartMinute = 0;
	m_nEndHour = 0;
	m_nEndMinute = 0;

	m_DownPoint = 0;
	m_rcMove.SetRectEmpty( );//must,设置矩形为空//0922
	m_rcSelectTime.SetRectEmpty();

	CPoint LeftScalePoint, RightScalePoint;

	if (m_bCaptured)
	{
		m_bDown = 1;

		if (point.x <= (m_rcRightBottom.left+m_nLeftNoScale))
		{
			m_DownPoint.x = m_rcRightBottom.left+m_nLeftNoScale;
		}
		else if (point.x >= (m_rcRightBottom.right-m_nRightNoScale))
		{
			m_DownPoint.x = m_rcRightBottom.right-m_nRightNoScale;
		}
		else
		{
			m_DownPoint.x = m_BlackDotPoint.x;
		}
		
		int nNoScaleLong = m_rcLeftBottom.right+m_nLeftNoScale;
		int nScaleLong = m_DownPoint.x-nNoScaleLong;
		int nHourNo = nScaleLong/m_nHourScale;
		int nMinuteNo = (nScaleLong%(int)m_nHourScale)/m_nMinuteScale;
		
		m_nStartHour = nHourNo;
		m_nStartMinute = nMinuteNo*PER_SCALE_MINUTES;
		
		m_nEndHour = m_nStartHour;
		m_nEndMinute = m_nStartMinute;	
	
	}

	ReleaseDC(pDC);

	CWnd::OnLButtonDown(nFlags, point);
}
///鼠标没按下时移动鼠标
void CScheduleCtrl::MoveNoDown(CPoint point)
{
	CDC* pDC = GetDC();

	CPen *pOldPen;
	CBrush *pOldBrush;

	if (m_bCaptured)
	{
		CPoint LeftScalePoint, RightScalePoint;
		GetLeftRightScalePoint(point, LeftScalePoint, RightScalePoint);
		
		//擦除黑点划线
		pOldPen = pDC->SelectObject(&m_penSolidWhite);
		pDC->MoveTo(m_BlackDotPoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(m_BlackDotPoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
		
		//画列表中的矩形(目的是消除擦除黑点划线时所画的白色直线)
		CRect rcList;//0922
		pOldBrush = pDC->SelectObject(&m_brBlue);
		pOldPen = pDC->SelectObject(&m_penSolidBlue);
		POSITION pos = m_cRectList.GetHeadPosition();
		while (pos != NULL)
		{
			rcList = m_cRectList.GetNext(pos);
			pDC->Rectangle(rcList);
		}
		
		//画黑点划线
		CPen penDotBlack;
		penDotBlack.CreatePen(PS_DOT, 1, COLOR_BLACK);
		
		pOldPen = pDC->SelectObject(&penDotBlack);
		pDC->MoveTo(LeftScalePoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(LeftScalePoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
			
		m_BlackDotPoint = LeftScalePoint;
		penDotBlack.DeleteObject();
	}
	else//0926(目的是擦除最后留下的黑点划线,不在矩形m_rcRightBottom时) 
	{
		//擦除黑点划线
		pOldPen = pDC->SelectObject(&m_penSolidWhite);
		pDC->MoveTo(m_BlackDotPoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(m_BlackDotPoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
		
		//画列表中的矩形(目的是消除擦除黑点划线时所画的白色直线)
		CRect rcList;//0922
		pOldBrush = pDC->SelectObject(&m_brBlue);
		pOldPen = pDC->SelectObject(&m_penSolidBlue);
		POSITION pos = m_cRectList.GetHeadPosition();
		while (pos != NULL)
		{
			rcList = m_cRectList.GetNext(pos);
			pDC->Rectangle(rcList);
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	ReleaseDC(pDC);	
}
//画显示选择时间的矩形
void CScheduleCtrl::DrawShowTimeRect()
{
	CDC* pDC = GetDC();

	CBrush brYellow;
	brYellow.CreateSolidBrush(COLOR_YELLOW);
	
	//m_rcShowTime为用来显示所选时间
	m_rcShowTime.left = m_rcLeftTop.left+SHOW_TIME_RECT_DIS;
	m_rcShowTime.top = m_rcLeftTop.top+SHOW_TIME_RECT_DIS;
	m_rcShowTime.right = m_rcLeftTop.right-SHOW_TIME_RECT_DIS;
	m_rcShowTime.bottom = m_rcLeftBottom.bottom-SHOW_TIME_RECT_DIS;
	
	if(m_nStartMinute == 60)
	{
		m_nStartHour += 1;
		m_nStartMinute = 0;
	}

	if(m_nEndMinute == 60)
	{
		m_nEndHour += 1;
		m_nEndMinute = 0;
	}
	CString strTime;
	strTime.Format(_T(" From: %.2d:%.2d\n To    : %.2d:%.2d"), m_nStartHour, m_nStartMinute, m_nEndHour, m_nEndMinute);
	
	CFont    font;
	font.CreateFont(-10, 0, 0, 0, 300, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH|FF_SWISS,NULL);
	CFont *Oldfont = pDC->SelectObject(&font);
	CBrush *pOldBrush = pDC->SelectObject(&brYellow);
	pDC->Rectangle(m_rcShowTime);
	pDC->SetBkMode(TRANSPARENT);//Background is not changed before drawing.
	pDC->DrawText(strTime, &m_rcShowTime, DT_LEFT);

	pDC->SelectObject(Oldfont);
	font.DeleteObject();		
	pDC->SelectObject(pOldBrush);
	brYellow.DeleteObject();

	ReleaseDC(pDC);	
}

void CScheduleCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CDC* pDC = GetDC();
			
	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);

	////////修改光标形状
	CRect  ValidateRect = m_rcRightBottom; 
	ValidateRect.DeflateRect(0, 0, 0, 4);

	if (PtInRect(&ValidateRect, point))//1016
	{
		if (m_bModeFlag)
		{
			SetCursor(m_hAddCursor);
		}
		else
		{
			SetCursor(m_hEraseCursor);
		}
///		SetCapture();//lh_Modify
		m_bCaptured = 1;
	}
	else
	{
///		ReleaseCapture();//lh_Modify
		m_bCaptured = 0;
	}
	///////
//	TRACE("m_bCaptured=%d,m_bDown = %d\n",m_bCaptured,m_bDown);
	if (!m_bDown)//(nFlags != MK_LBUTTON)//鼠标没按下
	{
		MoveNoDown(point);
	}
	else if (m_bCaptured && m_bDown)//鼠标在m_rcRightBottom里并且按下
	{	
		m_bModified = TRUE;
		SetCapture();//lh_Modify
		CPoint CurrentPoint;
		
		if ((point.x > (m_rcRightBottom.left + m_nLeftNoScale)) && (point.x < (m_rcRightBottom.right - m_nRightNoScale)))
		{
			CurrentPoint = point;
		}
		else if (point.x <= (m_rcRightBottom.left + m_nLeftNoScale))
		{
			CurrentPoint = m_rcRightBottom.left + m_nLeftNoScale;
		}
		else if (point.x >= (m_rcRightBottom.right - m_nRightNoScale))
		{
			CurrentPoint = m_rcRightBottom.right - m_nRightNoScale;
		}
		
		CPoint LeftScalePoint, RightScalePoint;
		GetLeftRightScalePoint(CurrentPoint, LeftScalePoint, RightScalePoint);
		//画鼠标移动所形成的矩形
		DrawMouseMoveRec(point, LeftScalePoint, RightScalePoint);
		//将矩形用时间来表示

		GetRectTime(CurrentPoint, m_rcMove, m_nStartHour, m_nStartMinute, m_nEndHour, m_nEndMinute);
		
		m_MovePrePoint = point;//获得移动的前一个点
		//画显示时间的矩形
		DrawShowTimeRect();	

		if( (m_nStartHour == m_nEndHour) && (m_nStartMinute == m_nEndMinute) )
		{
			m_rcSelectTime.SetRectEmpty();
		}
	}
	else ///m_bCapture == false && m_bDown == true
	{
///		m_bDown = 0;//must//lh_Modify
///		if (!(m_rcSelectTime.IsRectEmpty()))//lh_Modify
///		{//lh_Modify
///			m_cRectList.AddTail(m_rcSelectTime);//lh_Modify
///		}//lh_Modify
		//重画所选区域等
///		LeaveOrUpDraw();//lh_Modify
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	
	ReleaseDC(pDC);	
	
	CWnd::OnMouseMove(nFlags, point);
}

void CScheduleCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	
	ReleaseCapture();//lh_Modify
	CDC* pDC = GetDC();
	
    pDC->DPtoLP(&point);
	pDC->DPtoLP(&m_rcRightBottom);

	CRect rcMouseLastMove;
	
//	m_BlackDotPoint = 0;//0921//0922有问题

	m_bDown = 0;
	m_rcMove.SetRectEmpty( );

	CPoint LeftScalePoint, RightScalePoint;
	
	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);

///	if (m_bCaptured)//lh_Modify
///	{//lh_Modify
		if (!(m_rcSelectTime.IsRectEmpty()))
		{		
			//添加或删除列表中的矩形
			AddDeleteSect();
		}
		//重画所选区域等
		LeaveOrUpDraw();
///	}//lh_Modify

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	ReleaseDC(pDC);

	//ReleaseCapture();//1013
	
	VERIFY(GetParent() != NULL);

	GetParent()->SendMessage(UM_SCHEDULE_MODIFY, reinterpret_cast<WPARAM>(this));

	CWnd::OnLButtonUp(nFlags, point);
}


void CScheduleCtrl::GetLeftRightScalePoint(CPoint RealPoint, CPoint &LeftScalePoint, CPoint &RightScalePoint)
{
	if (RealPoint.x <= (m_rcRightBottom.left+m_nLeftNoScale))
	{
		LeftScalePoint = m_rcRightBottom.left+m_nLeftNoScale;
		RightScalePoint = LeftScalePoint;
	}
	else if (RealPoint.x >= (m_rcRightBottom.right-m_nRightNoScale))
	{
		LeftScalePoint = m_rcRightBottom.right-m_nRightNoScale;
		RightScalePoint = LeftScalePoint;
	}
	else//在0至24点之间
	{
		int nNoScaleLong = m_rcLeftBottom.right+m_nLeftNoScale;
		int nScaleLong = RealPoint.x-nNoScaleLong;
		int nHourNo = nScaleLong/m_nHourScale;
		int nMinuteNo = (nScaleLong%(int)m_nHourScale)/m_nMinuteScale;
				
		LeftScalePoint.x = nNoScaleLong+nHourNo*m_nHourScale+nMinuteNo*m_nMinuteScale;
		RightScalePoint.x = nNoScaleLong+nHourNo*m_nHourScale+(nMinuteNo+1)*m_nMinuteScale;	
	}
}

void CScheduleCtrl::GetRectTime(CPoint RealPoint, CRect rect, int &nStartHour, int &nStartMinute, int &nEndHour, int &nEndMinute)
{
	int nNoScaleLong = m_rcLeftBottom.right+m_nLeftNoScale;

	int nLScaleLong = rect.left-nNoScaleLong;
	int nLHourNo = nLScaleLong/m_nHourScale;
	int nLMinuteNo = (nLScaleLong%(int)m_nHourScale)/m_nMinuteScale;

	int nRScaleLong = rect.right-nNoScaleLong;
	int nRHourNo = nRScaleLong/m_nHourScale;
	int nRMinuteNo = (nRScaleLong%(int)m_nHourScale)/m_nMinuteScale;

	if (RealPoint.x > m_DownPoint.x)//左往右移动important
	{
		nStartHour = nLHourNo;
		nStartMinute = nLMinuteNo*PER_SCALE_MINUTES;
		nEndHour = nRHourNo;
		nEndMinute = nRMinuteNo*PER_SCALE_MINUTES;	
	}
	else
	{
		nStartHour = nLHourNo;
		nStartMinute = nLMinuteNo*PER_SCALE_MINUTES;
		nEndHour = nRHourNo;
		nEndMinute = nRMinuteNo*PER_SCALE_MINUTES;
	}
}

void CScheduleCtrl::ShowTitle(CString string)
{
	CDC* pDC = GetDC();

	CFont    font;
	font.CreateFont(-12, 0, 0, 0, 300, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH|FF_SWISS,NULL);
	CFont *Oldfont = pDC->SelectObject(&font);
	pDC->DrawText(string, &m_rcLeftBottom, DT_CENTER|DT_VCENTER);	
	pDC->SelectObject(Oldfont);
	font.DeleteObject();

	CPen penSolidBlack;
	penSolidBlack.CreatePen(PS_SOLID, 1, RGB(0,0,0));

	CPen *pOldPen = pDC->SelectObject(&penSolidBlack);
	pDC->MoveTo(m_rcLeftBottom.left, m_rcLeftBottom.top);
	pDC->LineTo(m_rcLeftBottom.right, m_rcLeftBottom.top);
	pDC->SelectObject(pOldPen);

	penSolidBlack.DeleteObject();

	ReleaseDC(pDC);
}

void CScheduleCtrl::AddSelectTimeSect(CRect rcSelect)
{
	//将选定的区域与列表(m_cRectList)中的矩形进行比较,并将结果放在NewList中
	CList<CRect, CRect>NewList;
	CompareSelectRect(rcSelect, NewList);

	///将新列表(NewList)中的元素两两比较,并将最后的结果放在LastList中
	CList<CRect, CRect>LastList;
	CompareListElement(NewList, LastList);

	////将最后的结果(LastList中)拷贝到m_cRectList中
	CopyList(LastList);
}
///拷贝列表（将List中的内容拷贝到m_cRectList中）
void CScheduleCtrl::CopyList(CRECTLIST &List)
{
	m_cRectList.RemoveAll();
	CRect rcLast;
	POSITION pos = List.GetHeadPosition();
	while(pos != NULL)
	{
		rcLast = List.GetNext(pos);
		if (rcLast.Width() != 0)//important(不添加(0,0,0,0)矩形)
		{
			m_cRectList.AddTail(rcLast);
		}
	}	
}

///将选定的区域与列表(m_cRectList)中的矩形进行比较,并将结果放在NewList中
void CScheduleCtrl::CompareSelectRect(CRect rcSelect, CRECTLIST &NewList)
{
	CRect rcList, rcReturn;
	CPoint rcListTLPoint, rcListBRPoint;
	CPoint rcSelectTLPoint, rcSelectBRPoint;
	int nUnion = 0;

	int nCounta = m_cRectList.GetCount();
	int nNum = 0;
	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList.SetRectEmpty();
		rcList = m_cRectList.GetNext(pos);

		int nIntersect = rcReturn.IntersectRect(rcSelect, rcList);

		if (nIntersect)//交叉
		{
			rcReturn.SetRectEmpty();
			nUnion = 0;

			nUnion = rcReturn.UnionRect(rcSelect, rcList);
			if (nUnion)
			{
				NewList.AddTail(rcReturn);
			}
		}
		else//不交叉
		{
			rcListTLPoint = rcList.TopLeft();
			rcListBRPoint = rcList.BottomRight();
			rcSelectTLPoint = rcSelect.TopLeft();
			rcSelectBRPoint = rcSelect.BottomRight();

			if ((rcListBRPoint.x == rcSelectTLPoint.x) || (rcListTLPoint.x == rcSelectBRPoint.x))
			{
				rcReturn.SetRectEmpty();
				nUnion = 0;
				nUnion = rcReturn.UnionRect(rcSelect, rcList);

				if (nUnion)
				{
					NewList.AddTail(rcReturn);
				}
			}
			else
			{	
				NewList.AddTail(rcList);
				nNum++;
			}
		}
	}
	///与列表中所有元素都不相交，加自己到列表中
	if (nNum == nCounta)
	{
		NewList.AddTail(rcSelect);	
	}
}
/////比较列表中的元素（两两比较）//////////
void CScheduleCtrl::CompareListElement(CRECTLIST &NewList, CRECTLIST &LastList)
{
	CRect rcResult;
	int nCount = NewList.GetCount();	
	POSITION posa = NewList.GetHeadPosition();

	while(posa != NULL)
	{
		int nLNum = 0;
		bool Flag = 0;

		if (nCount == 1)
		{
			POSITION pos = NewList.GetHeadPosition();
			while (pos != NULL)
			{
				CRect rcOne = NewList.GetNext(pos);
				LastList.AddTail(rcOne);
			}
		}
		
		POSITION temp = posa;
		CRect recta = NewList.GetAt(temp);
		
		POSITION posb = NewList.GetHeadPosition();
		while(posb != NULL)
		{
			Flag = 0;
			
			if (posb == temp)
			{
				NewList.GetNext(posb);
			}
			else
			{
				CRect rectb = NewList.GetAt(posb);
				int nCmp = rcResult.IntersectRect(recta, rectb);
				if (nCmp)//相交
				{
					rcResult.SetRectEmpty();
					
					rcResult.UnionRect(recta, rectb);
					NewList.SetAt(posb, rcResult);
					Flag = 1;
					break;
				}
				else
				{
					CPoint rectaTLPoint = recta.TopLeft();
					CPoint rectaBRPoint = recta.BottomRight();
					CPoint rectbTLPoint = rectb.TopLeft();//
					CPoint rectbBRPoint = rectb.BottomRight();
					
					if ((rectaTLPoint.x == rectbBRPoint.x) || (rectaBRPoint.x == rectbTLPoint.x))
					{
						rcResult.SetRectEmpty();
						rcResult.UnionRect(recta, rectb);
						NewList.SetAt(posb, rcResult);
						Flag = 1;
						break;
					}
					else
					{
						NewList.GetNext(posb);
						nLNum++;
					}
					
				}
				
				//与所有的元素都不相交，把该元素加到列表中
				if (nLNum == (nCount-1))
				{
					LastList.AddTail(recta);
				}
			}	
		}
		
		NewList.GetNext(posa);
		
		if (Flag)
		{
			//删除NewList中已经合并了的矩形
			NewList.RemoveAt(temp);//是temp而不是posa
			nCount--;
		}	
	}
}

void CScheduleCtrl::DelSelectTimeSect(CRect rcSelect)
{
	///将要删除的区域与列表(m_cRectList)中的矩形进行比较，并将结果放到NewList中
	CList<CRect, CRect>NewList;
	CompareDelRect(rcSelect, NewList);

	///拷贝列表(将最后的结果拷贝到m_cRectList中)
	CopyList(NewList);
}
///将要删除的区域与列表(m_cRectList)中的矩形进行比较，并将结果放到NewList中
void CScheduleCtrl::CompareDelRect(CRect rcSelect, CRECTLIST &NewList)
{
	CRect rcList, rcReturn;
	CPoint rcListTLPoint, rcListBRPoint;
	CPoint rcSelectTLPoint, rcSelectBRPoint;

	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList.SetRectEmpty();
		rcList = m_cRectList.GetAt(pos);

		int nIntersect = rcReturn.IntersectRect(rcSelect, rcList);
		if (!nIntersect)//不交叉
		{
			NewList.AddTail(rcList);
		}
		else//交叉,6种情况
		{
			rcListTLPoint = rcList.TopLeft();
			rcListBRPoint = rcList.BottomRight();
			rcSelectTLPoint = rcSelect.TopLeft();
			rcSelectBRPoint = rcSelect.BottomRight();
			
			if ((rcSelectTLPoint.x > rcListTLPoint.x) && (rcSelectBRPoint.x < rcListBRPoint.x))
			{
				CRect rcLeft, rcRight;
				
				rcLeft.left = rcList.left;
				rcLeft.right = rcSelect.left;
				rcLeft.top = rcList.top;
				rcLeft.bottom = rcList.bottom;
				NewList.AddTail(rcLeft);
				
				rcRight.left = rcSelect.right;
				rcRight.right = rcList.right;
				rcRight.top = rcList.top;
				rcRight.bottom = rcList.bottom;
				NewList.AddTail(rcRight);
				
			}
			else
			{
				rcReturn.SetRectEmpty();
				rcReturn.SubtractRect(rcList, rcSelect);
				if(!rcReturn.IsRectEmpty())
					NewList.AddTail(rcReturn);
			}	
		}
		
		m_cRectList.GetNext(pos);
	}
}

//脏标记
BOOL CScheduleCtrl::IsModified()
{
	return m_bModified;
}

void CScheduleCtrl::ResetModifyFlag()
{
	m_bModified = FALSE;
}

////////设置操作模式
void CScheduleCtrl::SetOperateMode(bool bModeFlag)
{
	m_bModeFlag = bModeFlag;
}
////////获得操作模式(m_bModeFlag为1时为添加)
bool CScheduleCtrl::GetOperateMode()
{
	return m_bModeFlag;
}

////////设置控件标题(如Sunday、Monday等)////
void CScheduleCtrl::SetTitle(const CString& sTitle)
{
	m_sTitle = sTitle;

	InvalidateRect(m_rcLeftBottom);
//	ShowTitle(m_sTitle);//1017
}

////////获得控件标题////////
CString CScheduleCtrl::GetTitle()
{
	return m_sTitle;
}


void CScheduleCtrl::Full()
{
    m_cRectList.RemoveAll();

    CRect rcConvert;
    WEEKTIMESECTION TimeSect;
    TimeSect.startTimeSec=0;
    TimeSect.endTimeSec=1440*60;
    TimeSectToRect(TimeSect, rcConvert);
    m_cRectList.AddTail(rcConvert);
}

void CScheduleCtrl::Empty()
{
    m_cRectList.RemoveAll();
    RedrawWindow();
}

///(1010 add)///////
void CScheduleCtrl::SetTimeSec(const Scheduleday *ptimelist)
{
	ASSERT(ptimelist);

	//从存放时间区域的列表中得到时间区域,将其转换为矩形并存于列表(m_cRectList)中
	m_cRectList.RemoveAll();//清空列表

    int nCount = ptimelist->useNo;
    const WORD (&starttime)[Max_Schedule_day] = ptimelist->starttime;
    const WORD (&endtime)[Max_Schedule_day] = ptimelist->endtime;
	for (int i=0; i<nCount; i++)
	{
		CRect rcConvert;
		WEEKTIMESECTION TimeSect;
		TimeSect.startTimeSec=int(starttime[i])*60;
		if (endtime[i]== 1440-1)
		{
			TimeSect.endTimeSec=(int(endtime[i])+1)*60;
		}
		else
		{
			TimeSect.endTimeSec=int(endtime[i])*60;
		}
		//将时间区域转换为矩形
		TimeSectToRect(TimeSect, rcConvert);
		//将矩形存于列表中
		m_cRectList.AddTail(rcConvert);
	}
	RedrawWindow();
}

int CScheduleCtrl::GetTimeSec(Scheduleday *ptimelist)
{
	//获得列表(m_cRectList)中的矩形，从而确定96个状态位
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;

	POSITION pos = m_cRectList.GetHeadPosition();	
	int& nn= ptimelist->useNo = 0;
	while (pos != NULL)
	{
		CRect rcGet;	

		rcGet = m_cRectList.GetNext(pos);
		//获得时间区域的起始和结束时间（以时分刻度表示）
		int nStartHour = (rcGet.left-nLeftAllNoScale)/m_nHourScale;
		int nStartMinute = ((rcGet.left-nLeftAllNoScale)%(int)(m_nHourScale/*+1*/))/m_nMinuteScale;
		int nEndHour = (rcGet.right-nLeftAllNoScale)/m_nHourScale;
		int nEndMinute = ((rcGet.right-nLeftAllNoScale)%(int)(m_nHourScale))/m_nMinuteScale;

		//确定96个状态位
		int nSectStart = (nStartHour*ONE_HOUR_SCALE_NUM)+nStartMinute;
		int nSectEnd = ((nEndHour*ONE_HOUR_SCALE_NUM)+nEndMinute);
		//使在该时间区域里的状态位置1


		ptimelist->starttime[nn] = nSectStart * 15 ;
		if(nSectEnd == 96)
		{
			ptimelist->endtime[nn] = nSectEnd * 15 -1;
		}
		else
		{
			ptimelist->endtime[nn] = nSectEnd * 15 ;
		}
		nn++;		
		if (nn>24) 
		{
			CString	strNote = _T("Too many sections(0-24)");
			//::getString(IDS_TOO_MANY_SECTION, strNote, "Too many sections(0-24).");
			MessageBox(strNote);
			break;
		}
	}	

	return nn;
}


void CScheduleCtrl::SetTimeStatus(ULONG Status0, ULONG Status1, ULONG Status2)//接口
{
	//将状态信息保存到m_EeightHourStatus[3]中
	memset(m_EightHourStatus, 0, 3*sizeof(ULONG));
	m_EightHourStatus[0] = Status0;
	m_EightHourStatus[1] = Status1;
	m_EightHourStatus[2] = Status2;

	//将m_uStatus[3]中的信息保存到m_bQuarStatus[96]中
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<PER_ULONG_BIT; j++)
		{
			bool bStatus = (m_EightHourStatus[i]>>j) & 0x01;
			m_bQuarStatus[i*PER_ULONG_BIT+j] = bStatus;
		}
	}
	//根据96个状态信息确定所选定的时间区域的个数及其起始和结束时间,存放于列表TimeSectList中
	int nNum = 0;
	TIMESECTLIST TimeSectList;
	EnsureSectNum(TimeSectList, nNum);
	//从存放时间区域的列表中得到时间区域,将其转换为矩形并存于列表(m_cRectList)中
	if (!(m_cRectList.IsEmpty()))
	{
		m_cRectList.RemoveAll();//清空列表
	}

	POSITION pos = TimeSectList.GetHeadPosition();
	while (pos != NULL)
	{
		WEEKTIMESECTION TimeSect;
		memset(&TimeSect, 0, sizeof(WEEKTIMESECTION));	
		CRect rcConvert;
		rcConvert.SetRectEmpty();

		TimeSect = TimeSectList.GetNext(pos);
		//将时间区域转换为矩形
		TimeSectToRect(TimeSect, rcConvert);
		//将矩形存于列表中
		m_cRectList.AddTail(rcConvert);
	}	
	RedrawWindow();
}


//将时间区域用矩形来表示
void CScheduleCtrl::TimeSectToRect(WEEKTIMESECTION &TimeSect, CRect &rect)
{
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;

	int nStartHour = (TimeSect.startTimeSec)/PER_HOUR_SEC;
	int nStartMinute = (((TimeSect.startTimeSec)%PER_HOUR_SEC)/PER_MINUTE_SEC)/PER_SCALE_MINUTES; //nStartMinute以15分钟为单位
	int nEndHour = (TimeSect.endTimeSec)/PER_HOUR_SEC;
	int nEndMinute = (((TimeSect.endTimeSec)%PER_HOUR_SEC)/PER_MINUTE_SEC)/PER_SCALE_MINUTES;
	//用矩形来表示
	rect.left = nLeftAllNoScale+(nStartHour*m_nHourScale)+(nStartMinute*m_nMinuteScale);
	rect.right = nLeftAllNoScale+(nEndHour*m_nHourScale)+(nEndMinute*m_nMinuteScale);
	rect.top = m_rcRightBottom.top+SELECT_TIME_RECT_DIS;
	rect.bottom = m_rcRightBottom.bottom-SELECT_TIME_RECT_DIS;

}
//根据96个状态信息确定所选定的时间区域的个数及其起始和结束时间,存放于列表TimeSectList中
void CScheduleCtrl::EnsureSectNum(TIMESECTLIST &TimeSectList, int &nNum)
{
	bool bFlag = 0;
	WEEKTIMESECTION TimeSect;
	memset(&TimeSect, 0, sizeof(WEEKTIMESECTION));
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;

	for (int j=0; j<DAY_QUARTER_NUM; j++)
	{
		if (m_bQuarStatus[j])
		{
			if(!bFlag)//(bFlag从0变为1)得到时间区域的起始点
			{
				if (j == (DAY_QUARTER_NUM-1))//选定区域为23:45-24:00
				{
					TimeSect.startTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
					TimeSect.endTimeSec = ((j+1)/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+((j+1)%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
				}
				else//得到时间区域的起始点
				{
					TimeSect.startTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;//以15分钟为单位
					bFlag = 1;
				}
			}
			else if (j == (DAY_QUARTER_NUM-1))//选定区域的结束为24:00
			{
				TimeSect.endTimeSec = ((j+1)/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
					+((j+1)%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
			}
		}
		else
		{
			if(bFlag)//(bFlag从1变为0)得到时间区域的结束点
			{
				TimeSect.endTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
					+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
				bFlag = 0;
			}
		}

		int nTimeSectSize = TimeSect.endTimeSec-TimeSect.startTimeSec;
		if (nTimeSectSize > 0)
		{
			TimeSectList.AddTail(TimeSect);//将所得到的时间区域加到列表中
			nNum++;
			memset(&TimeSect, 0, sizeof(WEEKTIMESECTION));
		}
	}	
}

///(1010 add)获得一天中每刻钟的状态///////
void CScheduleCtrl::GetTimeStatus(ULONG &Status0, ULONG &Status1, ULONG &Status2)
{
	//获得列表(m_cRectList)中的矩形，从而确定96个状态位
	CRect rcGet;
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;
	int i=0;
	//对96个状态位清零
	memset(m_bQuarStatus, 0, DAY_QUARTER_NUM*sizeof(bool));

	POSITION pos = m_cRectList.GetHeadPosition();	
	while (pos != NULL)
	{	
		rcGet.SetRectEmpty();
		rcGet = m_cRectList.GetNext(pos);
		//获得时间区域的起始和结束时间（以时分刻度表示）
		int nStartHour = (rcGet.left-nLeftAllNoScale)/m_nHourScale;
		int nStartMinute = ((rcGet.left-nLeftAllNoScale)%(int)m_nHourScale)/m_nMinuteScale;
		int nEndHour = (rcGet.right-nLeftAllNoScale)/m_nHourScale;
		int nEndMinute = ((rcGet.right-nLeftAllNoScale)%(int)m_nHourScale)/m_nMinuteScale;

		//确定96个状态位
		int nSectStart = (nStartHour*ONE_HOUR_SCALE_NUM)+nStartMinute;
		int nSectEnd = ((nEndHour*ONE_HOUR_SCALE_NUM)+nEndMinute)-1;
		//使在该时间区域里的状态位置1
		for (i=nSectStart; i<=nSectEnd; i++)
		{	
			m_bQuarStatus[i] = 1;	
		}
	}
	
	//将m_bQuarStatus[96]的值保存到m_EightHourStatus[3]中
	ULONG Status = 0;
	memset(m_EightHourStatus, 0, 3*sizeof(ULONG));
	for (i=0; i<DAY_QUARTER_NUM; i++)
	{
		if (m_bQuarStatus[i])
		{
			int nInteger = i/PER_ULONG_BIT;
			int nRes = i%PER_ULONG_BIT;
			
			m_EightHourStatus[nInteger] |= m_bQuarStatus[i]<<nRes;	
		}
	}

	//根据m_EightHourStatus[3]中的值确定一天中每刻钟的状态
	Status0 = m_EightHourStatus[0];
	Status1 = m_EightHourStatus[1];
	Status2 = m_EightHourStatus[2];
}
//添加或删除列表中的矩形
void CScheduleCtrl::AddDeleteSect()
{
	if (m_bModeFlag)
	{	
		AddSelectTimeSect(m_rcSelectTime);	
	}
	else
	{		
		DelSelectTimeSect(m_rcSelectTime);		
	}
}
//重画所选区域等
void CScheduleCtrl::LeaveOrUpDraw()
{
	CDC *pDC = GetDC();
	CPen *pOldPen;
	CBrush *pOldBrush;


	m_rcShowTime.left = m_rcLeftTop.left+SHOW_TIME_RECT_DIS;
	m_rcShowTime.top = m_rcLeftTop.top+SHOW_TIME_RECT_DIS;
	m_rcShowTime.right = m_rcLeftTop.right-SHOW_TIME_RECT_DIS;
	m_rcShowTime.bottom = m_rcLeftBottom.bottom-SHOW_TIME_RECT_DIS;

	//画显示时间的矩形
	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&m_penSolidWhite);
	pDC->Rectangle(m_rcShowTime);
	//重画一条直线
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, COLOR_BLACK);
	
	pOldPen = pDC->SelectObject(&penBlack);
	
	pDC->MoveTo(m_rcLeftTop.left, m_rcLeftTop.bottom);
	pDC->LineTo(m_rcLeftTop.right, m_rcLeftTop.bottom);
		
	///在m_rcLeftBottom里写字符串
	ShowTitle(m_sTitle);//1017

	////////
	CPen penSolidBlack;
	penSolidBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);
	//画m_rcRightBottom全部区域
	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&penSolidBlack);
	pDC->Rectangle(m_rcRightBottom);
	
	//画列表中的矩形
	CRect rcList;
	pOldBrush = pDC->SelectObject(&m_brBlue);
	pOldPen = pDC->SelectObject(&m_penSolidBlue);	
	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList = m_cRectList.GetNext(pos);
		pDC->Rectangle(rcList);
	}
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	penSolidBlack.DeleteObject();
	penBlack.DeleteObject();

	ReleaseDC(pDC);
}


void CScheduleCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CScheduleCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CScheduleCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CScheduleCtrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnRButtonDblClk(nFlags, point);
}

void CScheduleCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnMButtonDown(nFlags, point);
}

void CScheduleCtrl::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnMButtonUp(nFlags, point);
}

void CScheduleCtrl::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (PtInRect(&m_rcRightBottom, point))//1016
	{
		if (m_bModeFlag)
			SetCursor(m_hAddCursor);
		else
			SetCursor(m_hEraseCursor);
	}
	CWnd::OnMButtonDblClk(nFlags, point);
}

void CScheduleCtrl::OnSize(UINT nType, int cx, int cy) 
{	
//	CList<TIME_SECTION, TIME_SECTION>  timeseclist;
//	
	Scheduleday timeseclist;	

	GetTimeSec(&timeseclist);	
	m_cRectList.RemoveAll();

	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rect, rcDeflate;
	GetClientRect(rect);
	
	rcDeflate = rect;
	rcDeflate.DeflateRect(0, 0, DECREASE_SIZE, DECREASE_SIZE);

	/*改变控间大小的时候 必须把那个相对于以前的位置的矩形区域列表进行从新转换成新的位置的矩形区域*/

    if ( rcDeflate.Width() == 0 ||
         rcDeflate.Height() == 0 )
    {
        return;
    }

	InitRect(rcDeflate);/**/

	SetTimeSec(&timeseclist);

	InvalidateRect(rcDeflate);
}

