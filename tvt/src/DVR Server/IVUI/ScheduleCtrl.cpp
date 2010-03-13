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

	m_bModeFlag = true; // Ĭ��Ϊдģʽ
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
	
	//���ĸ�����
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);

	CPen *pOldPen = dc.SelectObject(&penBlack);
	CBrush *pOldBrush = dc.SelectObject(&m_brWhite);

	dc.Rectangle(m_rcLeftTop);
	dc.Rectangle(m_rcLeftBottom);
	dc.Rectangle(m_rcRightTop);
	dc.Rectangle(m_rcRightBottom);
	
	penBlack.DeleteObject();

	///��m_rcLeftBottom��д�ַ���
	//ShowTitle(m_sTitle);	
	ShowTitle(m_sTitle);//1017
	///////////
	//���̶�
	CString strScale;

	for(int i=0; i<=DAY_HOURS; i++)
	{

		CRect rect(
			m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale-3,
			m_rcRightTop.top+1,
			m_rcRightTop.left+m_nLeftNoScale+i*m_nHourScale+10,
			m_rcRightTop.bottom -8
			);//����д�̶���

		//���̶���
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

		//д�̶�����
		strScale.Format(_T("%d"),i);
		CFont    font;
		font.CreateFont(-10, 0, 0, 0, 300, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH|FF_SWISS,NULL);
		CFont *Oldfont = dc.SelectObject(&font);
		dc.DrawText(strScale, rect, DT_LEFT );		
		dc.SelectObject(Oldfont);
		font.DeleteObject();
		
	}

	m_nRightNoScale = m_rcRightBottom.right - (m_rcRightTop.left+m_nLeftNoScale+DAY_HOURS*m_nHourScale);

	///���б��еľ���
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

	//���б��еľ���
/*	CRect rcList;//��������
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
	//m_rcMoveΪ��m_DownPoint��������ƶ����ĵ����γɵľ���
	m_rcMove.top = m_rcRightBottom.top+MOVE_RECT_DIS;
	m_rcMove.bottom = m_rcRightBottom.bottom-MOVE_RECT_DIS;
	m_rcMove.left = 0;
	m_rcMove.right = 0;//��һ��

	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);
			
	if (RealPoint.x > m_DownPoint.x)//������
	{
		if (m_MovePrePoint.x < RealPoint.x)//������
		{
			m_rcMove.left = m_DownPoint.x;
			m_rcMove.right = BackPoint.x;	
		}
		else//������
		{
			m_rcMove.left = m_DownPoint.x;
			m_rcMove.right = FrontPoint.x;		
		}
	}
	else//������
	{
		if (m_MovePrePoint.x > RealPoint.x)//������
		{
			m_rcMove.left = FrontPoint.x;
			m_rcMove.right = m_DownPoint.x;	
		}
		else//������
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

	//��m_rcRightBottomȫ������
	CPen penSolidBlack;
	penSolidBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);

	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&penSolidBlack);
	pDC->Rectangle(m_rcRightBottom);
	
	penSolidBlack.DeleteObject();

	//���б��еľ���
	CRect rcList;
	pOldBrush = pDC->SelectObject(&m_brBlue);
	pOldPen = pDC->SelectObject(&m_penSolidBlue);
	POSITION pos = m_cRectList.GetHeadPosition();
	while (pos != NULL)
	{
		rcList = m_cRectList.GetNext(pos);
		pDC->Rectangle(rcList);
	}

	//����ǰ����ƶ����γɵľ���
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
	m_rcMove.SetRectEmpty( );//must,���þ���Ϊ��//0922
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
///���û����ʱ�ƶ����
void CScheduleCtrl::MoveNoDown(CPoint point)
{
	CDC* pDC = GetDC();

	CPen *pOldPen;
	CBrush *pOldBrush;

	if (m_bCaptured)
	{
		CPoint LeftScalePoint, RightScalePoint;
		GetLeftRightScalePoint(point, LeftScalePoint, RightScalePoint);
		
		//�����ڵ㻮��
		pOldPen = pDC->SelectObject(&m_penSolidWhite);
		pDC->MoveTo(m_BlackDotPoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(m_BlackDotPoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
		
		//���б��еľ���(Ŀ�������������ڵ㻮��ʱ�����İ�ɫֱ��)
		CRect rcList;//0922
		pOldBrush = pDC->SelectObject(&m_brBlue);
		pOldPen = pDC->SelectObject(&m_penSolidBlue);
		POSITION pos = m_cRectList.GetHeadPosition();
		while (pos != NULL)
		{
			rcList = m_cRectList.GetNext(pos);
			pDC->Rectangle(rcList);
		}
		
		//���ڵ㻮��
		CPen penDotBlack;
		penDotBlack.CreatePen(PS_DOT, 1, COLOR_BLACK);
		
		pOldPen = pDC->SelectObject(&penDotBlack);
		pDC->MoveTo(LeftScalePoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(LeftScalePoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
			
		m_BlackDotPoint = LeftScalePoint;
		penDotBlack.DeleteObject();
	}
	else//0926(Ŀ���ǲ���������µĺڵ㻮��,���ھ���m_rcRightBottomʱ) 
	{
		//�����ڵ㻮��
		pOldPen = pDC->SelectObject(&m_penSolidWhite);
		pDC->MoveTo(m_BlackDotPoint.x, m_rcRightBottom.top+MOVE_RECT_DIS);
		pDC->LineTo(m_BlackDotPoint.x, m_rcRightBottom.bottom-MOVE_RECT_DIS);
		
		//���б��еľ���(Ŀ�������������ڵ㻮��ʱ�����İ�ɫֱ��)
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
//����ʾѡ��ʱ��ľ���
void CScheduleCtrl::DrawShowTimeRect()
{
	CDC* pDC = GetDC();

	CBrush brYellow;
	brYellow.CreateSolidBrush(COLOR_YELLOW);
	
	//m_rcShowTimeΪ������ʾ��ѡʱ��
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

	////////�޸Ĺ����״
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
	if (!m_bDown)//(nFlags != MK_LBUTTON)//���û����
	{
		MoveNoDown(point);
	}
	else if (m_bCaptured && m_bDown)//�����m_rcRightBottom�ﲢ�Ұ���
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
		//������ƶ����γɵľ���
		DrawMouseMoveRec(point, LeftScalePoint, RightScalePoint);
		//��������ʱ������ʾ

		GetRectTime(CurrentPoint, m_rcMove, m_nStartHour, m_nStartMinute, m_nEndHour, m_nEndMinute);
		
		m_MovePrePoint = point;//����ƶ���ǰһ����
		//����ʾʱ��ľ���
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
		//�ػ���ѡ�����
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
	
//	m_BlackDotPoint = 0;//0921//0922������

	m_bDown = 0;
	m_rcMove.SetRectEmpty( );

	CPoint LeftScalePoint, RightScalePoint;
	
	CPen *pOldPen = pDC->SelectObject(&m_penSolidBlue);
	CBrush *pOldBrush = pDC->SelectObject(&m_brBlue);

///	if (m_bCaptured)//lh_Modify
///	{//lh_Modify
		if (!(m_rcSelectTime.IsRectEmpty()))
		{		
			//��ӻ�ɾ���б��еľ���
			AddDeleteSect();
		}
		//�ػ���ѡ�����
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
	else//��0��24��֮��
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

	if (RealPoint.x > m_DownPoint.x)//�������ƶ�important
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
	//��ѡ�����������б�(m_cRectList)�еľ��ν��бȽ�,�����������NewList��
	CList<CRect, CRect>NewList;
	CompareSelectRect(rcSelect, NewList);

	///�����б�(NewList)�е�Ԫ�������Ƚ�,�������Ľ������LastList��
	CList<CRect, CRect>LastList;
	CompareListElement(NewList, LastList);

	////�����Ľ��(LastList��)������m_cRectList��
	CopyList(LastList);
}
///�����б���List�е����ݿ�����m_cRectList�У�
void CScheduleCtrl::CopyList(CRECTLIST &List)
{
	m_cRectList.RemoveAll();
	CRect rcLast;
	POSITION pos = List.GetHeadPosition();
	while(pos != NULL)
	{
		rcLast = List.GetNext(pos);
		if (rcLast.Width() != 0)//important(�����(0,0,0,0)����)
		{
			m_cRectList.AddTail(rcLast);
		}
	}	
}

///��ѡ�����������б�(m_cRectList)�еľ��ν��бȽ�,�����������NewList��
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

		if (nIntersect)//����
		{
			rcReturn.SetRectEmpty();
			nUnion = 0;

			nUnion = rcReturn.UnionRect(rcSelect, rcList);
			if (nUnion)
			{
				NewList.AddTail(rcReturn);
			}
		}
		else//������
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
	///���б�������Ԫ�ض����ཻ�����Լ����б���
	if (nNum == nCounta)
	{
		NewList.AddTail(rcSelect);	
	}
}
/////�Ƚ��б��е�Ԫ�أ������Ƚϣ�//////////
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
				if (nCmp)//�ཻ
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
				
				//�����е�Ԫ�ض����ཻ���Ѹ�Ԫ�ؼӵ��б���
				if (nLNum == (nCount-1))
				{
					LastList.AddTail(recta);
				}
			}	
		}
		
		NewList.GetNext(posa);
		
		if (Flag)
		{
			//ɾ��NewList���Ѿ��ϲ��˵ľ���
			NewList.RemoveAt(temp);//��temp������posa
			nCount--;
		}	
	}
}

void CScheduleCtrl::DelSelectTimeSect(CRect rcSelect)
{
	///��Ҫɾ�����������б�(m_cRectList)�еľ��ν��бȽϣ���������ŵ�NewList��
	CList<CRect, CRect>NewList;
	CompareDelRect(rcSelect, NewList);

	///�����б�(�����Ľ��������m_cRectList��)
	CopyList(NewList);
}
///��Ҫɾ�����������б�(m_cRectList)�еľ��ν��бȽϣ���������ŵ�NewList��
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
		if (!nIntersect)//������
		{
			NewList.AddTail(rcList);
		}
		else//����,6�����
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

//����
BOOL CScheduleCtrl::IsModified()
{
	return m_bModified;
}

void CScheduleCtrl::ResetModifyFlag()
{
	m_bModified = FALSE;
}

////////���ò���ģʽ
void CScheduleCtrl::SetOperateMode(bool bModeFlag)
{
	m_bModeFlag = bModeFlag;
}
////////��ò���ģʽ(m_bModeFlagΪ1ʱΪ���)
bool CScheduleCtrl::GetOperateMode()
{
	return m_bModeFlag;
}

////////���ÿؼ�����(��Sunday��Monday��)////
void CScheduleCtrl::SetTitle(const CString& sTitle)
{
	m_sTitle = sTitle;

	InvalidateRect(m_rcLeftBottom);
//	ShowTitle(m_sTitle);//1017
}

////////��ÿؼ�����////////
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

	//�Ӵ��ʱ��������б��еõ�ʱ������,����ת��Ϊ���β������б�(m_cRectList)��
	m_cRectList.RemoveAll();//����б�

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
		//��ʱ������ת��Ϊ����
		TimeSectToRect(TimeSect, rcConvert);
		//�����δ����б���
		m_cRectList.AddTail(rcConvert);
	}
	RedrawWindow();
}

int CScheduleCtrl::GetTimeSec(Scheduleday *ptimelist)
{
	//����б�(m_cRectList)�еľ��Σ��Ӷ�ȷ��96��״̬λ
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;

	POSITION pos = m_cRectList.GetHeadPosition();	
	int& nn= ptimelist->useNo = 0;
	while (pos != NULL)
	{
		CRect rcGet;	

		rcGet = m_cRectList.GetNext(pos);
		//���ʱ���������ʼ�ͽ���ʱ�䣨��ʱ�̶ֿȱ�ʾ��
		int nStartHour = (rcGet.left-nLeftAllNoScale)/m_nHourScale;
		int nStartMinute = ((rcGet.left-nLeftAllNoScale)%(int)(m_nHourScale/*+1*/))/m_nMinuteScale;
		int nEndHour = (rcGet.right-nLeftAllNoScale)/m_nHourScale;
		int nEndMinute = ((rcGet.right-nLeftAllNoScale)%(int)(m_nHourScale))/m_nMinuteScale;

		//ȷ��96��״̬λ
		int nSectStart = (nStartHour*ONE_HOUR_SCALE_NUM)+nStartMinute;
		int nSectEnd = ((nEndHour*ONE_HOUR_SCALE_NUM)+nEndMinute);
		//ʹ�ڸ�ʱ���������״̬λ��1


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


void CScheduleCtrl::SetTimeStatus(ULONG Status0, ULONG Status1, ULONG Status2)//�ӿ�
{
	//��״̬��Ϣ���浽m_EeightHourStatus[3]��
	memset(m_EightHourStatus, 0, 3*sizeof(ULONG));
	m_EightHourStatus[0] = Status0;
	m_EightHourStatus[1] = Status1;
	m_EightHourStatus[2] = Status2;

	//��m_uStatus[3]�е���Ϣ���浽m_bQuarStatus[96]��
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<PER_ULONG_BIT; j++)
		{
			bool bStatus = (m_EightHourStatus[i]>>j) & 0x01;
			m_bQuarStatus[i*PER_ULONG_BIT+j] = bStatus;
		}
	}
	//����96��״̬��Ϣȷ����ѡ����ʱ������ĸ���������ʼ�ͽ���ʱ��,������б�TimeSectList��
	int nNum = 0;
	TIMESECTLIST TimeSectList;
	EnsureSectNum(TimeSectList, nNum);
	//�Ӵ��ʱ��������б��еõ�ʱ������,����ת��Ϊ���β������б�(m_cRectList)��
	if (!(m_cRectList.IsEmpty()))
	{
		m_cRectList.RemoveAll();//����б�
	}

	POSITION pos = TimeSectList.GetHeadPosition();
	while (pos != NULL)
	{
		WEEKTIMESECTION TimeSect;
		memset(&TimeSect, 0, sizeof(WEEKTIMESECTION));	
		CRect rcConvert;
		rcConvert.SetRectEmpty();

		TimeSect = TimeSectList.GetNext(pos);
		//��ʱ������ת��Ϊ����
		TimeSectToRect(TimeSect, rcConvert);
		//�����δ����б���
		m_cRectList.AddTail(rcConvert);
	}	
	RedrawWindow();
}


//��ʱ�������þ�������ʾ
void CScheduleCtrl::TimeSectToRect(WEEKTIMESECTION &TimeSect, CRect &rect)
{
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;

	int nStartHour = (TimeSect.startTimeSec)/PER_HOUR_SEC;
	int nStartMinute = (((TimeSect.startTimeSec)%PER_HOUR_SEC)/PER_MINUTE_SEC)/PER_SCALE_MINUTES; //nStartMinute��15����Ϊ��λ
	int nEndHour = (TimeSect.endTimeSec)/PER_HOUR_SEC;
	int nEndMinute = (((TimeSect.endTimeSec)%PER_HOUR_SEC)/PER_MINUTE_SEC)/PER_SCALE_MINUTES;
	//�þ�������ʾ
	rect.left = nLeftAllNoScale+(nStartHour*m_nHourScale)+(nStartMinute*m_nMinuteScale);
	rect.right = nLeftAllNoScale+(nEndHour*m_nHourScale)+(nEndMinute*m_nMinuteScale);
	rect.top = m_rcRightBottom.top+SELECT_TIME_RECT_DIS;
	rect.bottom = m_rcRightBottom.bottom-SELECT_TIME_RECT_DIS;

}
//����96��״̬��Ϣȷ����ѡ����ʱ������ĸ���������ʼ�ͽ���ʱ��,������б�TimeSectList��
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
			if(!bFlag)//(bFlag��0��Ϊ1)�õ�ʱ���������ʼ��
			{
				if (j == (DAY_QUARTER_NUM-1))//ѡ������Ϊ23:45-24:00
				{
					TimeSect.startTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
					TimeSect.endTimeSec = ((j+1)/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+((j+1)%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
				}
				else//�õ�ʱ���������ʼ��
				{
					TimeSect.startTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
						+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;//��15����Ϊ��λ
					bFlag = 1;
				}
			}
			else if (j == (DAY_QUARTER_NUM-1))//ѡ������Ľ���Ϊ24:00
			{
				TimeSect.endTimeSec = ((j+1)/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
					+((j+1)%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
			}
		}
		else
		{
			if(bFlag)//(bFlag��1��Ϊ0)�õ�ʱ������Ľ�����
			{
				TimeSect.endTimeSec = (j/ONE_HOUR_SCALE_NUM)*PER_HOUR_SEC \
					+(j%ONE_HOUR_SCALE_NUM)*PER_SCALE_MINUTES*PER_MINUTE_SEC;
				bFlag = 0;
			}
		}

		int nTimeSectSize = TimeSect.endTimeSec-TimeSect.startTimeSec;
		if (nTimeSectSize > 0)
		{
			TimeSectList.AddTail(TimeSect);//�����õ���ʱ������ӵ��б���
			nNum++;
			memset(&TimeSect, 0, sizeof(WEEKTIMESECTION));
		}
	}	
}

///(1010 add)���һ����ÿ���ӵ�״̬///////
void CScheduleCtrl::GetTimeStatus(ULONG &Status0, ULONG &Status1, ULONG &Status2)
{
	//����б�(m_cRectList)�еľ��Σ��Ӷ�ȷ��96��״̬λ
	CRect rcGet;
	int nLeftAllNoScale = m_rcLeftBottom.right + m_nLeftNoScale;
	int i=0;
	//��96��״̬λ����
	memset(m_bQuarStatus, 0, DAY_QUARTER_NUM*sizeof(bool));

	POSITION pos = m_cRectList.GetHeadPosition();	
	while (pos != NULL)
	{	
		rcGet.SetRectEmpty();
		rcGet = m_cRectList.GetNext(pos);
		//���ʱ���������ʼ�ͽ���ʱ�䣨��ʱ�̶ֿȱ�ʾ��
		int nStartHour = (rcGet.left-nLeftAllNoScale)/m_nHourScale;
		int nStartMinute = ((rcGet.left-nLeftAllNoScale)%(int)m_nHourScale)/m_nMinuteScale;
		int nEndHour = (rcGet.right-nLeftAllNoScale)/m_nHourScale;
		int nEndMinute = ((rcGet.right-nLeftAllNoScale)%(int)m_nHourScale)/m_nMinuteScale;

		//ȷ��96��״̬λ
		int nSectStart = (nStartHour*ONE_HOUR_SCALE_NUM)+nStartMinute;
		int nSectEnd = ((nEndHour*ONE_HOUR_SCALE_NUM)+nEndMinute)-1;
		//ʹ�ڸ�ʱ���������״̬λ��1
		for (i=nSectStart; i<=nSectEnd; i++)
		{	
			m_bQuarStatus[i] = 1;	
		}
	}
	
	//��m_bQuarStatus[96]��ֵ���浽m_EightHourStatus[3]��
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

	//����m_EightHourStatus[3]�е�ֵȷ��һ����ÿ���ӵ�״̬
	Status0 = m_EightHourStatus[0];
	Status1 = m_EightHourStatus[1];
	Status2 = m_EightHourStatus[2];
}
//��ӻ�ɾ���б��еľ���
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
//�ػ���ѡ�����
void CScheduleCtrl::LeaveOrUpDraw()
{
	CDC *pDC = GetDC();
	CPen *pOldPen;
	CBrush *pOldBrush;


	m_rcShowTime.left = m_rcLeftTop.left+SHOW_TIME_RECT_DIS;
	m_rcShowTime.top = m_rcLeftTop.top+SHOW_TIME_RECT_DIS;
	m_rcShowTime.right = m_rcLeftTop.right-SHOW_TIME_RECT_DIS;
	m_rcShowTime.bottom = m_rcLeftBottom.bottom-SHOW_TIME_RECT_DIS;

	//����ʾʱ��ľ���
	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&m_penSolidWhite);
	pDC->Rectangle(m_rcShowTime);
	//�ػ�һ��ֱ��
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 2, COLOR_BLACK);
	
	pOldPen = pDC->SelectObject(&penBlack);
	
	pDC->MoveTo(m_rcLeftTop.left, m_rcLeftTop.bottom);
	pDC->LineTo(m_rcLeftTop.right, m_rcLeftTop.bottom);
		
	///��m_rcLeftBottom��д�ַ���
	ShowTitle(m_sTitle);//1017

	////////
	CPen penSolidBlack;
	penSolidBlack.CreatePen(PS_SOLID, 1, COLOR_BLACK);
	//��m_rcRightBottomȫ������
	pOldBrush = pDC->SelectObject(&m_brWhite);
	pOldPen = pDC->SelectObject(&penSolidBlack);
	pDC->Rectangle(m_rcRightBottom);
	
	//���б��еľ���
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

	/*�ı�ؼ��С��ʱ�� ������Ǹ��������ǰ��λ�õľ��������б���д���ת�����µ�λ�õľ�������*/

    if ( rcDeflate.Width() == 0 ||
         rcDeflate.Height() == 0 )
    {
        return;
    }

	InitRect(rcDeflate);/**/

	SetTimeSec(&timeseclist);

	InvalidateRect(rcDeflate);
}

