// WindowsTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WindowsTest.h"
#include "WindowsTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowsTestDlg dialog




CWindowsTestDlg::CWindowsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWindowsTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //m_pDrawer = new CRectangleDrawer();
    //m_pDrawer = new CPolygonDrawer();
    //m_pDrawer = new CLineDrawer();
    m_pDrawer = DrawerFactory::CreateDrawer(IDrawer_ArrowLine);
}

void CWindowsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowsTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CWindowsTestDlg message handlers

BOOL CWindowsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    CRect rect;
    GetClientRect(&rect);
    m_pDrawer->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 10246);
    m_pDrawer->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWindowsTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWindowsTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

