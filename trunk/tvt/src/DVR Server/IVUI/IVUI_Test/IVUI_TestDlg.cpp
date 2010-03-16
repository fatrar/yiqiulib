// IVUI_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI_Test.h"
#include "IVUI_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIVUI_TestDlg dialog

CIVUI_TestDlg::CIVUI_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVUI_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIVUI_TestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIVUI_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIVUI_TestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIVUI_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CIVUI_TestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_TEST, &CIVUI_TestDlg::OnBnClickedTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIVUI_TestDlg message handlers

BOOL CIVUI_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);

    HWND hWnd = m_hWnd;
    m_pISnapShotSender = SnapShotWnd::CreateSnapShotWnd(m_hWnd, rect.Width());

	//m_IVSwtichTab.Create(NULL, "" , WS_CHILD|WS_VISIBLE, rect, this, WM_USER);
	//m_IVSwtichTab.Init(this, rect);
	//m_IVSwtichTab.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIVUI_TestDlg::OnPaint() 
{
    CPaintDC dc(this);
	//if (IsIconic())
	//{
	//	 // device context for painting

	//	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

	//	// Center icon in client rectangle
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// Draw the icon
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialog::OnPaint();
	//}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIVUI_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIVUI_TestDlg::OnBnClickedTest()
{
    CFileDialog Dlg(TRUE);
    if ( IDOK == Dlg.DoModal() )
    {
        CString strPath = Dlg.GetPathName();

        //打开硬盘中的图形文件 
        HANDLE hFile=CreateFile(
            strPath,
            GENERIC_READ, 
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL); 
        if (hFile==INVALID_HANDLE_VALUE) 
        {
            return;
        }

        DWORD dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数
        if (dwFileSize == INVALID_FILE_SIZE) 
            return; 

        BYTE* pTemBuf = new BYTE[dwFileSize];
        DWORD dwByteRead = 0;
        BOOL bRc = ReadFile(hFile,pTemBuf,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区 
        CloseHandle(hFile);//关闭打开的文件 
        
        m_pISnapShotSender->OnSnapShotSend(0, 0, 0,pTemBuf, dwFileSize);

        delete[] pTemBuf;
    }
}
