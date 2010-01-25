// IVSwitchTab.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "IVSwitchTab.h"

enum
{
    BT_WIDTH = 80,
    BT_HEIGHT =40,
    BT_START_X = 40,
    BT_START_Y = 40,

    
    BT_START_ID = WM_USER,
};


// CIVSwitchTab dialog

IMPLEMENT_DYNAMIC(CIVSwitchTab, CDialog)

CIVSwitchTab::CIVSwitchTab(CWnd* pParent /*=NULL*/)
	: CDialog(CIVSwitchTab::IDD, pParent)
{

}

CIVSwitchTab::~CIVSwitchTab()
{
}

void CIVSwitchTab::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RULE, m_TabBt[0]);
    DDX_Control(pDX, IDC_ALARMOUT, m_TabBt[1]);
    DDX_Control(pDX, IDC_SCHUDULE, m_TabBt[2]);
}


BEGIN_MESSAGE_MAP(CIVSwitchTab, CDialog)
    ON_COMMAND_RANGE(BT_START_ID, BT_START_ID+TAB_BT_NUM, &CIVSwitchTab::ClickTabBt)
END_MESSAGE_MAP()
// CIVSwitchTab message handlers


BOOL CIVSwitchTab::Init( CWnd* pWnd, const CRect& rect )
{
    int nDistanceBt = (rect.Width() - 2*BT_START_X-BT_WIDTH)/(TAB_BT_NUM-1);	
    int i = 0;
    for (; i<TAB_BT_NUM; ++i)
    {
        int nOffset = i*nDistanceBt;
        m_TabBt[i].MoveWindow(
            CRect(BT_START_X+nOffset, BT_START_Y, BT_START_X+BT_WIDTH+nOffset,BT_START_Y+BT_HEIGHT));
    }

    return TRUE;
}

void CIVSwitchTab::ClickTabBt( UINT ID/*, WPARAM w, LPARAM l*/ )
{
    int j = ID-BT_START_ID;
    for (int i=0; i<TAB_BT_NUM; ++i)
    {
        if ( i != j )
        {
             m_TabBt[i].SetCheck(FALSE);
        }
    }
    TRACE("%d", ID);	

    switch (j)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        // log.. err
        break;
    }
}
