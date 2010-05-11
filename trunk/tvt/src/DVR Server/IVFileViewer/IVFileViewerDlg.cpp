// IVFileViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVFileViewer.h"
#include "IVFileViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIVFileViewerDlg dialog




CIVFileViewerDlg::CIVFileViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIVFileViewerDlg::IDD, pParent)
    , m_strStartTime(_T(""))
    , m_strEndTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIVFileViewerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_DataList);
    DDX_Text(pDX, IDC_StartTime, m_strStartTime);
    DDX_Text(pDX, IDC_EndTime, m_strEndTime);
}

BEGIN_MESSAGE_MAP(CIVFileViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOPEN, &CIVFileViewerDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CIVFileViewerDlg message handlers

BOOL CIVFileViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIVFileViewerDlg::OnPaint()
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
HCURSOR CIVFileViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL FileTimeFormat(CString& str, const FILETIME& time)
{
    SYSTEMTIME syt;
   if ( !FileTimeToSystemTime(&time, &syt) )
   {
       return FALSE;
   }

   str.Format(
        _T("%d-%d-%d %d:%d:%d.%d"),
        syt.wYear,
        syt.wMonth,
        syt.wDay,
        syt.wHour,
        syt.wMinute,
        syt.wSecond,
        syt.wMilliseconds );
    return TRUE;
}

void CIVFileViewerDlg::OnBnClickedOpen()
{
    CFileDialog Dlg(TRUE);
    Dlg.DoModal();

    if ( m_Reader.is_open() )
    {
        m_Reader.close();
        m_Reader.clear();
        m_DataList.ResetContent();
    }

    m_Reader.open(Dlg.GetPathName(), ios::binary|ios::in);
    if ( !m_Reader.is_open() )
    {
        AfxMessageBox(_T("File Can`t Open!"));
        return;
    }

    ParseFile();

    UpdateData(FALSE);
}

BOOL CIVFileViewerDlg::ParseFile()
{
    IVFileHeadFrist FirstHead;
    m_Reader.read((char*)&FirstHead, sizeof(FirstHead));
    if ( FirstHead.FileFlag != g_dwIVFileOK )
    {
        AfxMessageBox(_T("File Open Failed!"));
        return FALSE;
    }

    switch ( FirstHead.Version )
    {
    case IVFile_Version_1_0:
        return LoadHead<IVFile_Version_1_0>();
    case IVFile_Version_2_0:
        return LoadHead<IVFile_Version_2_0>();
    default:
        AfxMessageBox(_T("File Open Failed!"));
        return FALSE;
    }
}

template<IVFileVersionDefine Version>
BOOL CIVFileViewerDlg::LoadHead()
{
    IVFileHead<Version> Head;
    m_Reader.read(
        (char*)&Head+sizeof(IVFileHeadFrist),
        sizeof(Head)-sizeof(IVFileHeadFrist));

    BOOL bRc = FileTimeFormat(m_strStartTime,  Head.BeginTime);
    if ( !bRc )
    {
        AfxMessageBox(_T("Start Time Error!"));
        return FALSE;
    }

    bRc = FileTimeFormat(m_strEndTime,  Head.EndTime);
    if ( !bRc )
    {
        AfxMessageBox(_T("End Time Error!"));
        return FALSE;
    }

    return PushDataTolist<Version>(Head);
}

template<>
BOOL CIVFileViewerDlg::PushDataTolist(
    IVFileHead<IVFile_Version_1_0>& Head )
{
    CString strTmp;
    BOOL bRc = FALSE;
    for ( size_t i = 0; i< Head.dwIndexNum; ++i )
    {
        CFileTime Time = Head.BeginTime;
        __int64 nTmp = Head.DataIndex[i].TimeOffset*__int64(10000);
        Time += nTmp;
        bRc = FileTimeFormat(strTmp, Time);
        if ( !bRc )
        {
            AfxMessageBox(_T("Parse File failed at Data Head!"));
            return FALSE;
        }
        m_DataList.InsertString(i, strTmp);
    }

    return TRUE;
}

template<>
BOOL CIVFileViewerDlg::PushDataTolist(
    IVFileHead<IVFile_Version_2_0>& Head )
{
    CString strTmp;
    BOOL bRc = FALSE;
    for ( size_t i = 0; i< Head.wIndexNum; ++i )
    {
        CFileTime Time = Head.BeginTime;
        __int64 nTmp = Head.DataIndex[i].TimeOffset*__int64(10000);
        Time += nTmp;
        bRc = FileTimeFormat(strTmp, Time);
        if ( !bRc )
        {
            AfxMessageBox(_T("Parse File failed at Data Head!"));
            return FALSE;
        }
        m_DataList.InsertString(i, strTmp);
    }

    return TRUE;
}