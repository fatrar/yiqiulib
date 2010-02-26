/*********************************************************************
* ��Ȩ���� (C)2006, ������ͬΪ��Ƶ�������޹�˾��
* 
* �ļ����ƣ� // TextStatic.cpp
* ����ժҪ�� // ������˸
* ��ǰ�汾�� // 1.0.0.0
* ��    �ߣ� // ������
* �������ڣ� // 2006��11��16��
**********************************************************************/
#include "stdafx.h"
#include "TextStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextStatic

CTextStatic::CTextStatic()
{
	m_color=0;
	m_colorTmp=0;
	m_pCFont=NULL;
}

CTextStatic::~CTextStatic()
{
}

BEGIN_MESSAGE_MAP(CTextStatic, CStatic)
	//{{AFX_MSG_MAP(CTextStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextStatic message handlers

void CTextStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CFont *pFont, *pOldFont;
	CRect client_rect;
	CString szText;
	GetClientRect(client_rect);
	GetWindowText(szText);
	pFont = GetFont();
	if (m_pCFont!=NULL)
	{
		pOldFont = dc.SelectObject(m_pCFont);
	}
	else
	{
		pOldFont = dc.SelectObject(pFont);
	}
	
	dc.SetTextColor (m_color);

	// ��͸����������豸�ļ�
	dc.SetBkMode(TRANSPARENT);

	// ��ʾ����
	dc.DrawText(szText, client_rect, 0);

	dc.SelectObject(pOldFont);
}

void CTextStatic::OnTimer(UINT nIDEvent) 
{ 
	if (m_color==0) 
	{
		m_color=m_colorTmp;
	}
	else
	{
		m_color=0;
	}
	Invalidate();
	CStatic::OnTimer(nIDEvent);
}

void CTextStatic::SetAlarmColor(COLORREF color)
{
	m_color=color;
	m_colorTmp=color;
}

void CTextStatic::SetFont(int High,int Wide)
{
	HFONT hFont=CreateFont(High,Wide,0,0,FW_DONTCARE,false,false,false,DEFAULT_CHARSET|GB2312_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,"MS Sans Serif");
	m_pCFont=CFont::FromHandle(hFont);
}
