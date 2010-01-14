// IVSwtichTab.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI_Test.h"
#include "IVSwtichTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
	BT_WIDTH = 80,
	BT_HEIGHT =40,
	BT_START_X = 40,
	BT_START_Y = 40,
	
	BT_START_ID = WM_USER,
};

/////////////////////////////////////////////////////////////////////////////
// CIVSwtichTab

CIVSwtichTab::CIVSwtichTab()
{
}

CIVSwtichTab::~CIVSwtichTab()
{
}


BEGIN_MESSAGE_MAP(CIVSwtichTab, CWnd)
	//{{AFX_MSG_MAP(CIVSwtichTab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_COMMAND_RANGE(BT_START_ID, BT_START_ID+TAB_BT_NUM, ClickBt)
		//ON_BN_CLICKED(BT_START_ID, OnNcHitTest2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//ON_COMMAND_RANGE(BT_START_ID, BT_START_ID+TAB_BT_NUM, ClickBt)
//ON_BN_CLICKED(BT_START_ID, ClickBt)
		//ON_BN_CLICKED(BT_START_ID, OnNcHitTest)

//	ON_BN_CLICKED_RANGE()
BOOL CIVSwtichTab::Init(CWnd* pWnd, const CRect& rect)
{
	int nDistanceBt = (rect.Width() - 2*BT_START_X-BT_WIDTH)/(TAB_BT_NUM-1);	
	int i = 0;
	for (; i<TAB_BT_NUM; ++i)
	{
		int nOffset = i*nDistanceBt;
		m_TabBt[i].Create(
			"aaa",
			WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			CRect(BT_START_X+nOffset, BT_START_Y, BT_START_X+BT_WIDTH+nOffset,BT_START_Y+BT_HEIGHT),
			this,
			BT_START_ID+i);
	}

	//ShowWindow(SW_SHOW);
	return TRUE;
}

void CIVSwtichTab::ClickBt( UINT ID, WPARAM w, LPARAM l )
{
	for (int i=0; i<TAB_BT_NUM; ++i)
	{
		m_TabBt[i].EnableWindow(TRUE);
	}
	m_TabBt[ID-BT_START_ID].EnableWindow(FALSE);
	TRACE("%d", ID);	
}

void CIVSwtichTab::OnNcHitTest2()
{
	TRACE("DDDD");
}

/////////////////////////////////////////////////////////////////////////////
// CIVSwtichTab message handlers
