/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:49
	filename: 	f:\code\Darw\Darw\LineDrawer.cpp
	file path:	f:\code\Darw\Darw
	file base:	LineDrawer
	file ext:	cpp
	author:		heliang
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "LineDrawer.h"
#include "PointButton.h"

CLineDrawer::CLineDrawer(void)
	: m_bIsOK(false)
	, m_IsDargPointBt(false)
	, m_nDargBtIndex(-1)
{
	m_Button[0] = new CPointButton(this);
	m_Button[1] = new CPointButton(this);
}

CLineDrawer::~CLineDrawer(void)
{
	Release();
	delete m_Button[0];
	delete m_Button[1];
}

void CLineDrawer::OnMouseDown( CDC* pdc, CPoint& point, CRect* pLockRect )
{
	if ( m_bDrawing )
	{
		// Log
		TRACE( CString("m_bIsStartDraw is True! function name is ") + __FUNCDNAME__ );
		Release();
	}

	m_bIsOK = m_bDrawing = true;
	if ( pLockRect )
	{
		LockCursor(pLockRect);
	}

	Create(pdc->GetWindow(), point);
	m_Point[0] = m_Point[1] = point;
}

void CLineDrawer::OnMouseMove( CDC* pdc, CPoint& point)
{
	if ( !m_bDrawing ) return;
	
	m_Point[1] = point;
	pdc->GetWindow()->Invalidate();
}

void CLineDrawer::OnMouseUp(CDC* pdc, CPoint& point)
{
	if ( m_IsDargPointBt )
	{
		ASSERT(m_nDargBtIndex<=1&&m_nDargBtIndex>=0);
		CRect rect;
		m_Button[m_nDargBtIndex]->MoveWindow(CalculateRect(point.x, point.y, rect));
		m_Point[m_nDargBtIndex] = point;
		UnLockCursor();
		m_IsDargPointBt = false;
		m_nDargBtIndex = -1;
	}

	if ( m_bDrawing ) 
	{
		CRect rect;
		m_Button[1]->MoveWindow(CalculateRect(point.x, point.y, rect));
		m_Point[1] = point;
		UnLockCursor();
		m_bDrawing = false;
	}
	//MM_HIENGLISH
	
	pdc->GetWindow()->Invalidate();
}

void CLineDrawer::OnPaint(CDC* pdc)
{
	if (!m_bIsOK)
	{
		return;
	}

	pdc->MoveTo(m_Point[0]);
	pdc->LineTo(m_Point[1]);
}

void CLineDrawer::Release()
{
	m_Button[0]->DestroyWindow();
	m_Button[1]->DestroyWindow();
}

void CLineDrawer::Create(CWnd* pWnd, CPoint& point)
{
	CRect rect;
	CalculateRect(point.x, point.y, rect);
	if ( m_Button[0]->m_hWnd == NULL )
	{
		m_Button[0]->Create(
			"", WS_CHILD|WS_VISIBLE,
			rect,
			pWnd,
			MSG_0 );
	}
	else
	{
		m_Button[0]->MoveWindow(&rect);
	}

	if ( m_Button[1]->m_hWnd == NULL )
	{
		m_Button[1]->Create(
			"", WS_CHILD|WS_VISIBLE,
			rect,
			pWnd,
			MSG_1 );
	}
	else
	{
		m_Button[1]->MoveWindow(&rect);
	}
}

void CLineDrawer::Translate(
	MouseMsg msg,
	CPoint& point,
	int nID )
{
	switch (msg)
	{
	case MouseMsg_LButtonDown:
	{
		if ( m_bDrawing ) return;

		if ( m_IsDargPointBt ) { ASSERT(false); m_IsDargPointBt=false; }
		else
		{
			m_IsDargPointBt = true;
			m_nDargBtIndex = nID-MSG_0;
			ASSERT(m_nDargBtIndex<=1&&m_nDargBtIndex>=0);

			CWnd* pParentWnd = m_Button[m_nDargBtIndex]->GetParent();
			CRect rect;
			pParentWnd->GetClientRect(&rect);
			pParentWnd->ClientToScreen(&rect);
			LockCursor(&rect);
		}
		break;
	}
	case MouseMsg_LButtonUp:
	{
		if ( m_bDrawing )
		{
			int nIndex = nID-MSG_0;
			if ( )
			{
			}
			CRect rect;
			m_Button[1]->MoveWindow(CalculateRect(point.x, point.y, rect));
			m_Point[1] = point;
			UnLockCursor();
			m_bDrawing = false;
			pdc->GetWindow()->Invalidate();
		}
		else
		{
			if ( m_IsDargPointBt )
			{
				// if darg to other button
				int nIndex = nID-MSG_0;
				ASSERT(nIndex<=1&&nIndex>=0);
				if (nIndex!=m_nDargBtIndex )
				{
					CWnd* pParentWnd = m_Button[m_nDargBtIndex]->GetParent();
					ScreenToClient(pParentWnd->m_hWnd, &point);
					m_Point[m_nDargBtIndex] = point;
					CRect rect;
					m_Button[m_nDargBtIndex]->MoveWindow(CalculateRect(point.x, point.y, rect));
					pParentWnd->Invalidate();
				}

				m_IsDargPointBt = false;
				m_nDargBtIndex = -1;
				UnLockCursor();
			}
			else
			{
				ASSERT(false);
			}
		}
		
		break;
	}
	case MouseMsg_MouseMove:
		break;
	}
}