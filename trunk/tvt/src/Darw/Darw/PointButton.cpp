// PointButton.cpp : implementation file
//

/********************************************************************
	created:	2009/12/26
	created:	26:12:2009   9:44
	filename: 	f:\code\Darw\Darw\PointButton.cpp
	file path:	f:\code\Darw\Darw
	file base:	PointButton
	file ext:	cpp
	author:		yiqiu
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Darw.h"
#include "PointButton.h"
#include "DarwCommon.h"


// CPointButton

IMPLEMENT_DYNAMIC(CPointButton, CButton)

CPointButton::CPointButton(IMsgTranslator* pMsgTranslator):m_pMsgTranslator(pMsgTranslator)
{

}

CPointButton::~CPointButton()
{
}


BEGIN_MESSAGE_MAP(CPointButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CPointButton::OnLButtonDown( UINT nFlags, CPoint point )
{
	ClientToScreen(&point);
	m_pMsgTranslator->Translate(
		MouseMsg_LButtonDown, 
		point, GetDlgCtrlID() );
}

void CPointButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	ClientToScreen(&point);
	m_pMsgTranslator->Translate(
		MouseMsg_LButtonUp, 
		point, GetDlgCtrlID() );
}

void CPointButton::OnMouseMove( UINT nFlags, CPoint point )
{
	ClientToScreen(&point);
	m_pMsgTranslator->Translate(
		MouseMsg_MouseMove,
		point, GetDlgCtrlID() );
}
// CPointButton message handlers


