// QCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QC.h"
#include "QCDlg.h"
#include "ioctlV.h"
#include "overlay.h"
#include "SignalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define FIRSTINVIEW TRUE
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include <atlbase.h>

int CQCDlg::m_View;
PBYTE CQCDlg::m_pDecodedData;
int g_card_id;
CQCDlg *main;
class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQCDlg dialog
BOOL CQCDlg::m_bPreview = TRUE;//播放现场流标志 zld 2009/4/29 4408
BOOL CQCDlg::m_bRec = FALSE;//播放录像流标志 zld 2009/4/29 4408
BOOL CQCDlg::m_bNet = FALSE;//播放网络流标志 zld 2009/4/29 4408
CCriticalSection* CQCDlg::m_CodeLock = new CCriticalSection;
//HANDLE g_event;
HANDLE g_eventRec; //zld 2009/4/29 4408
HANDLE g_eventNet;//zld 2009/4/29 4408
HANDLE g_eventRec_Format;//zld 2009/4/29 4408
HANDLE g_eventNet_Format;//zld 2009/4/29 4408
HANDLE g_eventPreview;//zld 2009/4/29 4408
PBYTE CQCDlg::m_pDecodedData_Rec = NULL;//zld 2009/4/29 4408
PBYTE CQCDlg::m_pDecodedData_Net = NULL;//zld 2009/4/29 4408
int CQCDlg::m_Vchannel_Num = 0;//zld 2009/4/29 4408

static BOOL g_binitNet = TRUE;//by chenlong
static BOOL g_binitRec = TRUE;//by chenlong

CQCDlg::CQCDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQCDlg::IDD, pParent)
{
    main=this;
    //{{AFX_DATA_INIT(CQCDlg)
    m_view_card_id = _T("");
    m_ch1 = FALSE;
    m_ch2 = FALSE;
    m_ch3 = FALSE;
    m_ch4 = FALSE;

    m_view_card_type = _T("");
    m_RadioView = -1;
    m_nCurrentChnanel = 0;
    m_VideoSize = 0; //by chenlong
    m_hLossThread = NULL;//by chenlong
    m_RECWidth = 0;//by chenlong
    m_RECHeight = 0;//by chenlong
    m_NetWidth = 0;//by chenlong
    m_NetHeight = 0;//by chenlong
    m_D1ViewRadio = 0;//by chenlong
    m_bFresh = FALSE;//by chenlong
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRun = TRUE;
	m_CodecMgrH_Rec = new CCodecMgr[16];
	if (m_CodecMgrH_Rec == NULL)
	{
		AfxMessageBox("CQCDlg::CQCDlg m_CodecMgrH_Rec = NULL!");
	}
	m_CodecMgrH_Net = new CCodecMgr[16];
	if (m_CodecMgrH_Net == NULL)
	{
		AfxMessageBox("CQCDlg::CQCDlg m_CodecMgrH_Net = NULL!");
	}
	/*初始化m_bLoss by chenlong*/
    for (int n = 0; n < 16; n++)
    {
        m_bLoss[n] = 0;
    }
	g_eventRec = CreateEvent(NULL,FALSE,TRUE,NULL);
	g_eventNet = CreateEvent(NULL,FALSE,TRUE,NULL);
	g_eventPreview = CreateEvent(NULL,FALSE,TRUE,NULL);
	g_eventRec_Format= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_eventNet_Format= CreateEvent(NULL,FALSE,FALSE,NULL);
	
}

void CQCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CQCDlg)

    DDX_Control(pDX, IDC_CH4, m_cCh4);
    DDX_Control(pDX, IDC_CH3, m_cCh3);
    DDX_Control(pDX, IDC_CH2, m_cCh2);

    DDX_Control(pDX, IDC_CH1, m_cCh1);
    DDX_Text(pDX, IDC_CARD_ID_VIEW, m_view_card_id);
    DDX_Check(pDX, IDC_CH1, m_ch1);
    DDX_Check(pDX, IDC_CH2, m_ch2);
    DDX_Check(pDX, IDC_CH3, m_ch3);
    DDX_Check(pDX, IDC_CH4, m_ch4);

    DDX_Text(pDX, IDC_CARD_TYPE_VIEW, m_view_card_type);
    DDX_Radio(pDX, IDC_RADIO_PREVIEW, m_RadioView);
    DDX_Text(pDX, IDC_CURRENTCHANNEL, m_nCurrentChnanel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQCDlg, CDialog)
//{{AFX_MSG_MAP(CQCDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_FINISH, OnFinish)
ON_BN_CLICKED(IDC_CH1, OnCh1)
ON_BN_CLICKED(IDC_CH2, OnCh2)
ON_BN_CLICKED(IDC_CH3, OnCh3)
ON_BN_CLICKED(IDC_CH4, OnCh4)
ON_WM_TIMER()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_RADIO_CAPTURE, OnRadioCapture)
ON_BN_CLICKED(IDC_RADIO_PREVIEW, OnRadioPreview)
ON_BN_CLICKED(IDC_VIEWMODE1, OnViewmode1)
ON_BN_CLICKED(IDC_VIEWMODE16, OnViewmode16)
ON_BN_CLICKED(IDC_BTN_NEXTVIEW, OnBtnNextview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQCDlg message handlers

BOOL CQCDlg::OnInitDialog()
{
    
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    
    int xScreen=0,yScreen=0; //djx 07/07/13
    xScreen = GetSystemMetrics(SM_CXSCREEN);
    yScreen = GetSystemMetrics(SM_CYSCREEN);
    xScreen>1024?xScreen=-1:xScreen=0;
    yScreen>768?yScreen=-1:yScreen=0;
    MoveWindow(xScreen, yScreen, 1024, 768);
    
    //画面切换按钮
    GetDlgItem(IDC_BMP_VIEWMODE1)->ShowWindow(FALSE);
    GetDlgItem(IDC_BMP_VIEWMODE4)->ShowWindow(FALSE);
    GetDlgItem(IDC_BMP_VIEWMODE16)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE1)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE4)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE16)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_NEXTVIEW)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_CURRENTCHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_CURRENTCHANNEL)->ShowWindow(FALSE);
    
    InitialVar();

    if ((!m_card_infor.resSubId) && m_havePciDvrCard)
    {
        m_bCanDoSectionTest = FALSE;
    }
    
    if ((!m_card_infor.resDriver) && (m_havePciDvrCard || m_haveUsbDvrCard))
    {
        m_bCanDoSectionTest = FALSE;
    }
    g_card_id = m_card_infor.card_type_id = DVR_CARD_TD4104i;
               
    
    //long lMode = m_DB.GetMode();
    
    CSignalDlg sDlg;
    
    //sDlg.m_bSignal = lMode;		
    sDlg.DoModal();
    
    BITMAPINFOHEADER RecBmpHeader;
    
    if (sDlg.m_bSignal == 0)
    {
        m_dwVideoFormat = DVRVIDEO_STANDARD_PAL;
        RecBmpHeader.biWidth = 352;
        RecBmpHeader.biHeight = 288;
    }
    else
    {
        m_dwVideoFormat = DVRVIDEO_STANDARD_NTSC;
        if (m_card_infor.card_type_id != DVR_CARD_TD4108) //djx 2008/7/22 4108
        {
            RecBmpHeader.biWidth = 352;
        }
        else
        {
            RecBmpHeader.biWidth = 320;
        }
        RecBmpHeader.biHeight = 240;
    }
    
   // m_DB.SaveMode(sDlg.m_bSignal);
    
    LoadVideoRadio(0, 16, TRUE);//by chenlong
    
    /*加载匹配的dll*/
    m_DSPDLL = NULL;
    m_MyDSP = NULL;   
     
    
    if (!LoadMatchLibrary(m_card_infor.card_type_id))
    {
        return FALSE;
    }
    
    /*加载完Dll后的操作*/
    m_MyDSP = (CDeviceManager *)::GetProcAddress(m_DSPDLL, "g_Dev_Object");
    ASSERT(m_MyDSP != NULL);
    
    DWORD dwType;
    BOOL bresu = m_MyDSP->DeviceInitial(m_dwVideoFormat, VideoCALLBACKFUNC,AudioCALLBACKFUNC, m_hWnd, &dwType);
    if (!bresu) 
    {
        return FALSE;
    }
    
    m_card_infor.resPassWord = m_MyDSP->Password();
    
    m_Vchannel_Num = m_MyDSP->GetCardOpt(DVRCARDOPT_CHANNEL_NUM);
    DWORD pDSPSwitch[16];
    int i;
    for (i = 0; i < m_Vchannel_Num; i++)
    {
        pDSPSwitch[i] = 1;
    }	

    m_card_infor.video_channel_number = 4;
    m_ddraw->InitDSPBack(m_dwVideoFormat, 4);
    
    UpdateData(FALSE);
    m_card_infor.disname = "TD4104i";
    bresu = m_MyDSP->VideoCaptureStart(0, RecBmpHeader);
    
    if (!bresu) 
    {
        return FALSE;
    }
    m_MyDSP->SetSwitch(pDSPSwitch, 4);
  
    
    for (i = 0; i < m_Vchannel_Num; i++)
    {
        m_MyDSP->SetChannelStatus(CHSTATUS_SET_AUDIO_ENABLE, i, 0);
    }

    
    RefreshMainControl();
    VideoChannelControl(m_card_infor.video_channel_number);
    RefreshChannelCheckStatus(0);
    UpdateData(FALSE);
    m_bCanDoSectionTest = TRUE;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

/*加载匹配的DLL by chenlong*/
BOOL CQCDlg::LoadMatchLibrary(int nId)
{                 
    CString strDllPath = m_ExePath + "DEV_HC_DM642i.dll";
    m_DSPDLL = ::LoadLibrary((char*)(LPCTSTR)strDllPath);
    if (m_DSPDLL == NULL)
    {
        DWORD dw = GetLastError();
        CString str;
        str.Format("Can not find the %s", str);
        MessageBox(str);
        return FALSE;
    }
    return TRUE;
}

/*加载视频控件by chenlong*/
void CQCDlg::LoadVideoRadio(int nBegin, int nEnd, BOOL bShow)
{
    for (int n = nBegin; n < nEnd; n++)
    {
        GetDlgItem(IDC_CH1 + n)->ShowWindow(bShow);
    }
}

void CQCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQCDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        
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
        if (m_card_infor.card_type_id != DVR_CARD_TD3316)
        {
            m_ddraw->DrawBack();
        }
        else
            m_ddraw->DrawOverlayBack();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQCDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//响应结束测试
void CQCDlg::OnFinish() 
{
    int	i = 0;
    
    UpdateData(TRUE);

    m_bRun = FALSE;
       
    if (m_hLossThread != NULL)
    {
        TerminateThread(m_hLossThread,1);
        CloseHandle(m_hLossThread);
        m_hLossThread = NULL;
    }
    
	/*以下三个变量是为了加快退出的速度 by chenlong*/
    m_bPreview = FALSE;
    m_bRec = FALSE;
    m_bNet = FALSE;
 
    /*增加了3304S,3308S by chenlong*/
    if (m_card_infor.card_type_id == DVR_CARD_TD4104 ||
		m_card_infor.card_type_id == DVR_CARD_TD4104i ||
        m_card_infor.card_type_id == DVR_CARD_TD4408)//zld 2009/4/24 4408
    {
        /*判断m_MyDSP by chenlong*/
        if (m_MyDSP != NULL)
        {
            m_MyDSP->VideoCaptureStop();
            m_MyDSP->DeviceFree();
            m_MyDSP = NULL;
            ::FreeLibrary(m_DSPDLL);
        }
        m_ddraw->FreeDSPBack();       
    }
    
    {
        m_card_infor.resResult = TRUE;
    }
    
    if (m_havePciDvrCard||m_haveUsbDvrCard)
    {
        VideoCaptureStop();
    }
    KillTimer(ON_TIME_AUTOCALLMONITOR);
    
    //m_TestCard.FreeDevice();
    
    delete []m_CodecMgrH_Net;
    m_CodecMgrH_Net = NULL;
    delete []m_CodecMgrH_Rec;
    m_CodecMgrH_Rec = NULL;
    delete m_CodeLock;
    m_CodeLock = NULL;
    CloseHandle(g_eventRec);
    CloseHandle(g_eventNet);
    CloseHandle(g_eventPreview);
    CloseHandle(g_eventRec_Format);
    CloseHandle(g_eventNet_Format);
    CDialog::OnOK();
}

void CQCDlg::VideoChannelControl(int ch_num)
{  
    int cnt = 4;     
    if ((cnt--) > ch_num)
    {
        m_cCh4.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh3.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh2.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) >= ch_num)
    {
        m_cCh1.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
}

void CQCDlg::OnCh1() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    
    RefreshChannelCheckStatus(0); 
}

void CQCDlg::OnCh2() 
{
    if (!m_bCapturing)
    {
        return ;
    }

    RefreshChannelCheckStatus(1);
}

void CQCDlg::OnCh3() 
{
    if (!m_bCapturing)
    {
        return ;
    }

    RefreshChannelCheckStatus(2);  
}

void CQCDlg::OnCh4() 
{
    if (!m_bCapturing)
    {
        return ;
    }

    RefreshChannelCheckStatus(3);
}

void CQCDlg::InitialVar()
{
    for (int i = 0; i < 16; i++)
        m_ChShow[i] = TRUE;
    
    m_ddraw = new CDirectDraw(m_hWnd);
    m_ddraw->IniDirectDraw();
    m_ChChecked[0]=m_ch1=FIRSTINVIEW;
    m_ChChecked[1]=m_ch2=FIRSTINVIEW;
    m_ChChecked[2]=m_ch3=FIRSTINVIEW;
    m_ChChecked[3]=m_ch4=FIRSTINVIEW;
    m_bCanDoSectionTest=FALSE;
    m_bCapturing=FALSE;
    m_havePciDvrCard=FALSE;
    m_haveUsbDvrCard=FALSE;
    m_haveAlarmCard=FALSE;
    m_dwVideoFormat=DVRVIDEO_STANDARD_PAL;
    m_video_size_type=DVRVIDEO_SIZE_320x240;
    char szBuf[1024] = {0};
    DWORD len = 1024;	
    //得到执行档所在路径
    if(GetModuleFileName(NULL,szBuf,len)!=0)
    {
        m_ExePath=szBuf;
        m_ExePath=m_ExePath.Left(m_ExePath.ReverseFind ('\\')+1);
    }
    
}

void CQCDlg::RefreshChannelCheckStatus(int index)
{
    if ((m_havePciDvrCard || m_haveUsbDvrCard))
    {
        //heliang -
       // m_TestCard.SetChannelOrder(m_card_infor.video_channel_number, (DWORD *)m_ChChecked);
        RedrawWindow();
    }
    //	TRACE("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",m_ChChecked[0],m_ChChecked[1],m_ChChecked[2],m_ChChecked[3],m_ChChecked[4],m_ChChecked[5],m_ChChecked[6],m_ChChecked[7],m_ChChecked[8],m_ChChecked[9],m_ChChecked[10],m_ChChecked[11],m_ChChecked[12],m_ChChecked[13],m_ChChecked[14],m_ChChecked[15]);
}

BOOL CQCDlg::AudioCALLBACKFUNC(FRAMEBUFSTRUCT *bufs)
{
    main->m_MyDSP->ReleaseBuffer(0, bufs->BufferPara);
    return TRUE;
}
CCriticalSection g_CodeLock;

BOOL CQCDlg::VideoCALLBACKFUNC(FRAMEBUFSTRUCT *bufs)
{
    if (main->m_MyDSP == NULL)
    {
        return FALSE;
    }

    if (bufs->nStreamID == VIDEO_STREAM_PREVIEW)
    {
        main->m_ddraw->ShowDSP(bufs->ChannelIndex, bufs->pBuf, main->m_card_infor.video_channel_number,YUV422); 
        main->m_MyDSP->ReleaseBuffer(1, bufs->BufferPara);
	}
	else if (bufs->nStreamID == VIDEO_STREAM_CAPTURE)//zld 2009/4/24 4408 添加录像流功能
	{
		if (!m_bRec)
		{
			if (bufs->ChannelIndex >= 0 && bufs->ChannelIndex < 16)
			{
				main->m_MyDSP->ReleaseBuffer(1, bufs->BufferPara);
			}
			
			return FALSE;
		}
	}
	else if (bufs->nStreamID == VIDEO_STREAM_NET)//zld 2009/4/24 4408 添加网络流测试功能
	{
		if (!m_bNet)
		{
			main->m_MyDSP->ReleaseBuffer(1, bufs->BufferPara);
			return FALSE;
		}
    }
    else
    {
        return FALSE;
    } 

    return TRUE;
}

void CQCDlg::FillAviHead(int bmptype)
{
    memset(&bitmapinfohead, 0, sizeof(BITMAPINFOHEADER));
    bitmapinfohead.biSize = sizeof(BITMAPINFOHEADER);
    switch (bmptype) {
    case 0:
        bitmapinfohead.biWidth = 352;//nWidth;
        bitmapinfohead.biHeight = 288;//nHeight;
        break;
    case 1:
        bitmapinfohead.biWidth = 320;//nWidth;
        bitmapinfohead.biHeight = 240;//nHeight;
        break;
    case 2:
        bitmapinfohead.biWidth = 352;//nWidth;
        bitmapinfohead.biHeight = 240;//nHeight;
        break;
    case 3:
        bitmapinfohead.biWidth = 640;//nWidth;
        bitmapinfohead.biHeight = 480;//nHeight;
        break;
    default:
        bitmapinfohead.biWidth = 640;//nWidth;
        bitmapinfohead.biHeight = 480;//nHeight;
    }
    
    bitmapinfohead.biPlanes = 1;
    bitmapinfohead.biBitCount = 24;
    //bitmapinfohead.biCompression =mmioFOURCC('M','P','4','2');// uVideoMode;
    //bitmapinfohead.biCompression =mmioFOURCC('T','V','T','A');// uVideoMode;
    bitmapinfohead.biCompression =mmioFOURCC('Y','U','Y','2');// uVideoMode;
    //	memcpy(&strfVideo.biCompression, "divx", 4);
    bitmapinfohead.biSizeImage = bitmapinfohead.biWidth*bitmapinfohead.biHeight*bitmapinfohead.biBitCount/8;
    bitmapinfohead.biXPelsPerMeter = 0;
    bitmapinfohead.biYPelsPerMeter = 0;
    bitmapinfohead.biClrUsed = 0;
    bitmapinfohead.biClrImportant = 0;
}

BOOL CQCDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
        {
            //::TranslateMessage(pMsg);
            //::DispatchMessage(pMsg);
            return TRUE;
        }
    }	
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CQCDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    // TODO: Add your specialized code here and/or call the base class
    return CDialog::OnCommand(wParam, lParam);
}


void CQCDlg::OnTimer(UINT nIDEvent) 
{
    /*增加了新的time事件 by chenlong*/
   /* if (nIDEvent == ON_TIME_AUTOCALLMONITOR)
    {
        CallMonitor(TRUE);
    }*/
    if (nIDEvent == ON_TIME_CHECKSINGAL)
    {
        /*if (m_TestCard.GetInvalidateFlag(m_TestCard.m_pVideoDriver[0]->GetDeviceNumber()))
        {
            Invalidate(FALSE);
        }*/
    }

    CDialog::OnTimer(nIDEvent);
}

//根据卡的型号，刷新主窗口的显示
void CQCDlg::RefreshMainControl()
{
    m_view_card_type = m_card_infor.disname;	//Card Type窗口	
    
    if (!this->m_haveAlarmCard)
    {
        if (m_card_infor.card_id != "")
        {
            m_view_card_id = m_card_infor.card_id.Mid(22, m_card_infor.card_id.GetLength() - 22);
        }   
    }
}

BOOL CQCDlg::VideoCaptureStart()
{
    //if ((m_havePciDvrCard||m_haveUsbDvrCard) && m_bCanDoSectionTest && !m_bCapturing)
    //{
    //    m_bCapturing = TRUE;//m_video_size_type
    //    SetTimer(ON_TIME_CHECKSINGAL, 200, NULL);//创建新的time事件
    //    return m_TestCard.VideoCaptureStart(m_video_size_type,bitmapinfohead);
    //}
    //else
        return TRUE;
}

void CQCDlg::VideoCaptureStop()
{
   /* if ((m_havePciDvrCard || m_haveUsbDvrCard) && m_bCanDoSectionTest && m_bCapturing)
    {
        m_bCapturing = FALSE;
        m_TestCard.VideoCaptureStop();
    }*/
}

BOOL CQCDlg::GetUserInfor(CString user_name)
{
    user_name="";
    return FALSE;
}

/*获得通道数 by chenlong*/
int CQCDlg::GetChannelNum(int nId)
{
    return 4;
}

/*初始化codecMgr by chenlong*/
void CQCDlg::FormatCodecMgrH(int nIndex, int codecType)
{
    if (codecType == 0)
    {
        ZeroMemory(&m_CodecMgrH_Rec[nIndex].m_BmpU,sizeof(m_CodecMgrH_Rec[nIndex].m_BmpU));
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biWidth=m_CodecMgrH_Rec[nIndex].cs.cx;
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biHeight=m_CodecMgrH_Rec[nIndex].cs.cy;
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biPlanes=1;	
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biBitCount=16;	
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biSizeImage=m_CodecMgrH_Rec[nIndex].cs.cx*m_CodecMgrH_Rec[nIndex].cs.cy*m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biBitCount/8;
        m_CodecMgrH_Rec[nIndex].m_BmpU.bmiHeader.biCompression=mmioFOURCC('H','2','6','4');
        m_CodecMgrH_Rec[nIndex].m_BmpC = m_CodecMgrH_Rec[nIndex].m_BmpU;
        m_CodecMgrH_Rec[nIndex].m_BmpC.bmiHeader.biCompression=BI_RGB;
        m_CodecMgrH_Rec[nIndex].m_BmpC.bmiHeader.biBitCount=32;
        m_CodecMgrH_Rec[nIndex].m_BmpC.bmiHeader.biSizeImage=m_CodecMgrH_Rec[nIndex].cs.cx*m_CodecMgrH_Rec[nIndex].cs.cy*m_CodecMgrH_Rec[nIndex].m_BmpC.bmiHeader.biBitCount/8;
        m_CodecMgrH_Rec[nIndex].InitCodecH(1);
    }
    else if (codecType == 1)
    {
        ZeroMemory(&m_CodecMgrH_Net[nIndex].m_BmpU,sizeof(m_CodecMgrH_Net[nIndex].m_BmpU));
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biWidth=m_CodecMgrH_Net[nIndex].cs.cx;
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biHeight=m_CodecMgrH_Net[nIndex].cs.cy;
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biPlanes=1;	
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biBitCount=16;	
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biSizeImage=m_CodecMgrH_Net[nIndex].cs.cx*m_CodecMgrH_Net[nIndex].cs.cy*m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biBitCount/8;
        m_CodecMgrH_Net[nIndex].m_BmpU.bmiHeader.biCompression=mmioFOURCC('H','2','6','4');
        m_CodecMgrH_Net[nIndex].m_BmpC = m_CodecMgrH_Net[nIndex].m_BmpU;
        m_CodecMgrH_Net[nIndex].m_BmpC.bmiHeader.biCompression=BI_RGB;
        m_CodecMgrH_Net[nIndex].m_BmpC.bmiHeader.biBitCount=32;
        m_CodecMgrH_Net[nIndex].m_BmpC.bmiHeader.biSizeImage=m_CodecMgrH_Net[nIndex].cs.cx*m_CodecMgrH_Net[nIndex].cs.cy*m_CodecMgrH_Net[nIndex].m_BmpC.bmiHeader.biBitCount/8;
        m_CodecMgrH_Net[nIndex].InitCodecH(1);
    }
}

void CQCDlg::OnDestroy() 
{
    //m_TestCard.DeviceFreeVideo();
    //m_TestCard.PciDeviceFree();

    CDialog::OnDestroy();
    
	// heliang -
    if (m_card_infor.card_type_id == DVR_CARD_TD4104 ||
		m_card_infor.card_type_id == DVR_CARD_TD4104i ||
        m_card_infor.card_type_id == DVR_CARD_TD4108 || //djx 2008/7/22 4108
        m_card_infor.card_type_id == DVR_CARD_TD4116)
    {
        m_ddraw->FreeDSPBack();
    }
    
    KillTimer(ON_TIME_CHECKSINGAL);
    delete m_ddraw;
    // TODO: Add your message handler code here
    
}

/*优化了点击“录象数据”按钮的处理 by chenlong*/
void CQCDlg::OnRadioCapture() 
{
    if (m_bRec)
    {
        return;
    }
    
	// heliang -
    GetDlgItem(IDC_BMP_VIEWMODE1)->ShowWindow(FALSE);
    GetDlgItem(IDC_BMP_VIEWMODE4)->ShowWindow(FALSE);
    GetDlgItem(IDC_BMP_VIEWMODE16)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE1)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE4)->ShowWindow(FALSE);
    GetDlgItem(IDC_VIEWMODE16)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_NEXTVIEW)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_CURRENTCHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_CURRENTCHANNEL)->ShowWindow(FALSE);
    
    UpdateData(TRUE);
    m_View = m_RadioView;

    m_ddraw->ShowOverlay(FALSE);		
}

/*优化了点击“现场数据”按钮的处理 by chenlong*/
void CQCDlg::OnRadioPreview() 
{
    if (m_bPreview)
    {
        return;
    }

	// heliang -
    GetDlgItem(IDC_BMP_VIEWMODE1)->ShowWindow(TRUE);
    GetDlgItem(IDC_BMP_VIEWMODE16)->ShowWindow(TRUE);
    
    GetDlgItem(IDC_VIEWMODE1)->ShowWindow(TRUE);
    GetDlgItem(IDC_VIEWMODE16)->ShowWindow(TRUE);
    
    ((CButton*)GetDlgItem(IDC_VIEWMODE16))->SetCheck(1);
    ((CButton*)GetDlgItem(IDC_VIEWMODE1))->SetCheck(0);
    
    OnViewmode16();
}

DWORD WINAPI CQCDlg::LossThread(PVOID pParam)
{
    /*方便以后增加新条件，内部改为if, 增加了4408的条件用来刷新界面 by chenlong*/
    // heiang -
    return 0;
}


void CQCDlg::OnViewmode1() 
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_STATIC_CURRENTCHANNEL)->ShowWindow(TRUE);
    GetDlgItem(IDC_CURRENTCHANNEL)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_NEXTVIEW)->ShowWindow(TRUE);
    UpdateData(TRUE);
    m_View = m_RadioView;
    
    BYTE array[18];
    array[0] = 0;
    array[1] = m_nCurrentChnanel;
    
    for (int i = 2; i < 18; i++)
        array[i] = i - 2;
    
    m_ddraw->ShowOverlay(TRUE);
    m_ddraw->DrawOverlayBack();
}

void CQCDlg::OnViewmode16() 
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_STATIC_CURRENTCHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_CURRENTCHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_NEXTVIEW)->ShowWindow(FALSE);
    
    UpdateData(TRUE);
    m_View = m_RadioView;
    
    BYTE array[18];
    array[0] = 7;
    array[1] = 1;
    
    for (int i = 2; i < 18; i++)
        array[i] = i - 2;
    
    m_ddraw->ShowOverlay(TRUE);
    m_ddraw->DrawOverlayBack();
}

void CQCDlg::OnBtnNextview() 
{
    // TODO: Add your control notification handler code here
    m_nCurrentChnanel++;
    if (m_nCurrentChnanel > 16)
    {
        m_nCurrentChnanel = 1;
    }
    
    UpdateData(FALSE);
    
    OnViewmode1();
}

void CQCDlg::FreshAcceptButton() //djx
{
    m_card_infor.resResult = TRUE;
    
    if (m_card_infor.card_type_id == DVR_CARD_TDNULL)
        m_card_infor.resResult = FALSE;
}


VOID CQCDlg::YUV420_YUV422Pack(void *pDst, void *pSrc, unsigned int nWidth, unsigned int nHeight, unsigned int nPitch, INT src420Subtype)
{
    unsigned int i = 0;
    unsigned int j = 0;
    ////2009/02/27---
    int BufWidth;
    if(nWidth == 352)
    {
        BufWidth = 384;
    }
    else if(nWidth == 176)
    {
        BufWidth = 192;
    }
    else
    {
        BufWidth = nWidth;
    }
    
    
    if (src420Subtype==1)
    {
        BYTE* pY = (BYTE*)pSrc;
        BYTE* pU = pY+BufWidth/*384*/*nHeight/*288*/;//2009/02/27
        BYTE* pV = pY+BufWidth/*384*/*nHeight/*288*/+BufWidth/*384*/*nHeight/*288*//4;//2009/02/27
        BYTE * pSurf=(BYTE*)pDst;
        for(i=0;i<nHeight/*288*/;i++)//2009/02/27
        {
            for(j=0;j<nWidth/*352*//2;j++)
            {
                pSurf[j*4] =	pY[j*2];
                pSurf[j*4+1] =	pU[j*2+1];
                pSurf[j*4+2] =	pY[j*2+1];
                pSurf[j*4+3] =	pU[j*2];
            }
            pSurf += nPitch;//352*2;
            pY += BufWidth/*384*/;
            if(i&1)
            {
                pU += BufWidth/*384*/;
                pV += BufWidth/*384*/;
            }
        }	
    }
    else if (src420Subtype==0)
    {
        
        BYTE *pY = (BYTE*)pSrc;
        BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
        BYTE *pV = (BYTE*)pU + nWidth * nHeight * 1 / 4;
        
        long iEDXValue=0;
        _asm 
        {
            mov eax,1
            cpuid
            mov iEDXValue,edx
        }
        
        if((iEDXValue&0x800000) == 0)//CPU不支持MMX,直接转换
        {
            BYTE *pYUV422Pack = (BYTE*)pDst;
            
            for(i = 0; i < nHeight; i++)
            {
                for(j = 0; j < nWidth/2; j++)
                {
                    *pYUV422Pack++ = *pY++;
                    *pYUV422Pack++ = *pU++;
                    *pYUV422Pack++ = *pY++;
                    *pYUV422Pack++ = *pV++;
                }
                
                pYUV422Pack += nPitch - nWidth*2;
                
                if(i%2 == 0)	//偶数行，UV分量分别要向前回一行，因为下一个奇数行要共享本行的UV分量
                {
                    pU -= nWidth/2;
                    pV -= nWidth/2;
                }
            }
        }
        else//CPU支持MMX，用MMX指令集转换
        {
            unsigned int count = nWidth*2/32;
            unsigned int diff = nPitch -nWidth*2;
            unsigned int line_bytes_uv = nWidth/2;
            __asm 
            {
                mov edi, pDst;
                mov esi, pY;
                mov eax, pU;
                mov edx, pV;
                mov ebx, nHeight
LOOP_H:
                mov ecx, count;
LOOP_W:
                movq mm0, [esi + 0];
                movq mm2, [esi + 8];
                movq mm4, [eax];
                movq mm6, [edx];
                movq mm1, mm0;
                movq mm3, mm2;
                movq mm5, mm4;
                
                PUNPCKLBW MM4, MM6;
                PUNPCKHBW MM5, MM6;
                
                PUNPCKLBW MM0, MM4;
                PUNPCKHBW MM1, MM4;
                PUNPCKLBW MM2, MM5;
                PUNPCKHBW MM3, MM5;
                
                movq [edi +  0], mm0;
                movq [edi +  8], mm1;
                movq [edi + 16], mm2;
                movq [edi + 24], mm3;
                
                add esi, 16;
                add edi, 32;
                add eax, 8;
                add	edx, 8;
                dec ecx;
                jnz LOOP_W;
                add edi,diff;
                test ebx, 0x1
                    jnz	YU_NEXT_LINE		;奇数行与其前面一偶数行共享UV分量
                    sub	eax, line_bytes_uv
                    sub edx, line_bytes_uv
YU_NEXT_LINE:
                dec ebx;
                jnz LOOP_H;
                emms;
            }
        }
    }
}

void CQCDlg::VideoFormatChange(DWORD videoformat)
{
    if (m_dwVideoFormat == videoformat)
    {
        return;
    }
    m_dwVideoFormat = videoformat;
    
    int width = 352;
    int height = 0;
    DWORD size33XX = 0;
    
    if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
    {
        height = 288;
        size33XX = DVRVIDEO_SIZE_352x288;
    }
    else
    {
        height = 240;
        size33XX = DVRVIDEO_SIZE_320x240;
    }
    
	// heliang -
   if (m_card_infor.card_type_id == DVR_CARD_TD4104 ||
		m_card_infor.card_type_id == DVR_CARD_TD4104i ||
        m_card_infor.card_type_id == DVR_CARD_TD3308)//zld 2009/4/30 3308
    {
        BITMAPINFOHEADER RecBmpHeader;
        RecBmpHeader.biWidth = width;
        RecBmpHeader.biHeight = height;
        DWORD pSwitch[16];
        if (m_bRec)
        {
            for (int i = 0; i < m_Vchannel_Num; i++)
            {
                pSwitch[i] = 1;
            }
        }
        else
        {
            for (int i = 0; i < m_Vchannel_Num; i++)
            {
                pSwitch[i] = 0;
            }
        }
        
        m_MyDSP->VideoCaptureStop();
        m_MyDSP->DeviceFree();
        ::FreeLibrary(m_DSPDLL);
        
        m_DSPDLL = NULL;
        m_MyDSP = NULL;
        
        if (!LoadMatchLibrary(m_card_infor.card_type_id))
        {
            return;
        }
        
        m_MyDSP = (CDeviceManager *)::GetProcAddress(m_DSPDLL, "g_Dev_Object");
        ASSERT(m_MyDSP != NULL);
        
        int nChannelNum = GetChannelNum(m_card_infor.card_type_id);
        
        m_ddraw->FreeDSPBack();
        if (m_card_infor.card_type_id == DVR_CARD_TD4108) //djx 2008/7/22 4108
        {
            m_ddraw->InitDSPD1Back(m_dwVideoFormat, nChannelNum);
        }
        else
        {
            m_ddraw->InitDSPBack(m_dwVideoFormat, nChannelNum);
        }
        
        DWORD dwType;
        m_MyDSP->DeviceInitial(m_dwVideoFormat, VideoCALLBACKFUNC, AudioCALLBACKFUNC, m_hWnd, &dwType);	
        m_card_infor.resPassWord = m_MyDSP->Password();
        
        m_MyDSP->VideoCaptureStart(0, RecBmpHeader);
           
        m_MyDSP->SetSwitch(pSwitch, nChannelNum);
    }
   // heliang -
}




