
// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDlg dialog




CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
    , m_dwCommond(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //m_pDrawer = new CRectangleDrawer();
    //m_pDrawer = new CPolygonDrawer();
    //m_pDrawer = new CLineDrawer();
    m_pDrawer = new CArrowLineDrawer();
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_Commond_EDIT, m_dwCommond);
	DDV_MinMaxUInt(pDX, m_dwCommond, 0, 3);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_Commond_EDIT, &CTestDlg::OnEnChangeCommondEdit)
    ON_BN_CLICKED(IDC_Send_BUTTON, &CTestDlg::OnBnClickedSendButton)
END_MESSAGE_MAP()


// CTestDlg message handlers
BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    CRect rect;
    GetClientRect(&rect);
    rect.bottom /= 2;
    m_pDrawer->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 10246);
    m_pDrawer->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint()
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestDlg::OnEnChangeCommondEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CTestDlg::OnBnClickedSendButton()
{
    UpdateData();
    m_pDrawer->SendCommond((IDrawer::DrawCommond)m_dwCommond, NULL, NULL);
}
