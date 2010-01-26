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
CMyWaveOut CQCDlg::m_WaveOut;
int CQCDlg::m_AudioChannel;
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
    m_res_install_driver = _T("");
    m_res_pass_word = _T("");
    m_res_sub_id = _T("");
    m_view_card_id = _T("");
    m_ch1 = FALSE;
    m_ch10 = FALSE;
    m_ch11 = FALSE;
    m_ch12 = FALSE;
    m_ch13 = FALSE;
    m_ch14 = FALSE;
    m_ch15 = FALSE;
    m_ch16 = FALSE;
    m_ch2 = FALSE;
    m_ch3 = FALSE;
    m_ch4 = FALSE;
    m_ch5 = FALSE;
    m_ch6 = FALSE;
    m_ch7 = FALSE;
    m_ch8 = FALSE;
    m_ch9 = FALSE;
    m_view_card_type = _T("");
    m_sn = _T("");
    m_Audio = -1;
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
	DDX_Control(pDX, IDC_COMBO_D1, m_ComboD1);
    DDX_Control(pDX, IDC_PROGRESS_SIZECHANGE, m_ProSizeChange);
    DDX_Control(pDX, IDC_BTN_APPLY, m_BtnApply);
    DDX_Control(pDX, IDC_3316FPJ, m_TextFPJ); //djx
    DDX_Control(pDX, IDC_ALARM_RELAY, m_c_alarm_relay_capture);
    DDX_Control(pDX, IDC_ALARM_OUT, m_c_alarm_out_capture);
    DDX_Control(pDX, IDC_ALARM_IN, m_c_alarm_in_capture);
    DDX_Control(pDX, IDC_AUTO_DWELL_CAPTURE, m_c_auto_dwell_capture);
    DDX_Control(pDX, IDC_ACCEPT, m_C_accept);
    DDX_Control(pDX, IDC_REJECT, m_C_reject);
    DDX_Control(pDX, IDC_CARD_SERIAL_NUMBER, m_C_sn);
    DDX_Control(pDX, IDC_VIDEO_FORMAT_NTSC, m_SetVideoFormatNtsc);
    DDX_Control(pDX, IDC_VIDEO_FORMAT_PAL, m_SetVideoFormatPal);
    DDX_Control(pDX, IDC_LABLE_SWITCH, m_switch);
    DDX_Control(pDX, IDC_DISPLAY, m_display);
    DDX_Control(pDX, IDC_ALARM_TEST_BUTTON, m_calarm_test);
    DDX_Control(pDX, IDC_RESET, m_reset);
    DDX_Control(pDX, IDC_DISPLAY_RES, m_c_display_res);
    DDX_Control(pDX, IDC_RESET_RES, m_c_reset_res);
    DDX_Control(pDX, IDC_ALARM_RELAY_RES, m_c_alarm_relay_res);
    DDX_Control(pDX, IDC_ALARM_OUT_RES, m_c_alarm_out_res);
    DDX_Control(pDX, IDC_AUTO_DWELL_RES, m_c_auto_dwell_res);
    DDX_Control(pDX, IDC_ALARM_IN_RES, m_c_alarm_in_res);
    DDX_Control(pDX, IDC_CH11, m_cCh11);
    DDX_Control(pDX, IDC_CH12, m_cCh12);
    DDX_Control(pDX, IDC_CH9, m_cCh9);
    DDX_Control(pDX, IDC_CH8, m_cCh8);
    DDX_Control(pDX, IDC_CH7, m_cCh7);
    DDX_Control(pDX, IDC_CH6, m_cCh6);
    DDX_Control(pDX, IDC_CH5, m_cCh5);
    DDX_Control(pDX, IDC_CH4, m_cCh4);
    DDX_Control(pDX, IDC_CH3, m_cCh3);
    DDX_Control(pDX, IDC_CH2, m_cCh2);
    DDX_Control(pDX, IDC_CH16, m_cCh16);
    DDX_Control(pDX, IDC_CH15, m_cCh15);
    DDX_Control(pDX, IDC_CH14, m_cCh14);
    DDX_Control(pDX, IDC_CH13, m_cCh13);
    DDX_Control(pDX, IDC_CH10, m_cCh10);
    DDX_Control(pDX, IDC_CH1, m_cCh1);
    DDX_Control(pDX, IDC_DWELL_LIST, m_list_dwell);
    DDX_Text(pDX, IDC_INSTRALL_DRIVER_RES, m_res_install_driver);
    DDX_Text(pDX, IDC_PASS_WORD_RES, m_res_pass_word);
    DDX_Text(pDX, IDC_SUBSYSTEM_ID_RES, m_res_sub_id);
    DDX_Text(pDX, IDC_CARD_ID_VIEW, m_view_card_id);
    DDX_Check(pDX, IDC_CH1, m_ch1);
    DDX_Check(pDX, IDC_CH10, m_ch10);
    DDX_Check(pDX, IDC_CH11, m_ch11);
    DDX_Check(pDX, IDC_CH12, m_ch12);
    DDX_Check(pDX, IDC_CH13, m_ch13);
    DDX_Check(pDX, IDC_CH14, m_ch14);
    DDX_Check(pDX, IDC_CH15, m_ch15);
    DDX_Check(pDX, IDC_CH16, m_ch16);
    DDX_Check(pDX, IDC_CH2, m_ch2);
    DDX_Check(pDX, IDC_CH3, m_ch3);
    DDX_Check(pDX, IDC_CH4, m_ch4);
    DDX_Check(pDX, IDC_CH5, m_ch5);
    DDX_Check(pDX, IDC_CH6, m_ch6);
    DDX_Check(pDX, IDC_CH7, m_ch7);
    DDX_Check(pDX, IDC_CH8, m_ch8);
    DDX_Check(pDX, IDC_CH9, m_ch9);
    DDX_Text(pDX, IDC_CARD_TYPE_VIEW, m_view_card_type);
    DDX_Text(pDX, IDC_CARD_SERIAL_NUMBER, m_sn);
    DDX_Radio(pDX, IDC_RADIO1, m_Audio);
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
ON_BN_CLICKED(IDC_CH5, OnCh5)
ON_BN_CLICKED(IDC_CH6, OnCh6)
ON_BN_CLICKED(IDC_CH7, OnCh7)
ON_BN_CLICKED(IDC_CH8, OnCh8)
ON_BN_CLICKED(IDC_CH9, OnCh9)
ON_BN_CLICKED(IDC_CH10, OnCh10)
ON_BN_CLICKED(IDC_CH11, OnCh11)
ON_BN_CLICKED(IDC_CH12, OnCh12)
ON_BN_CLICKED(IDC_CH13, OnCh13)
ON_BN_CLICKED(IDC_CH14, OnCh14)
ON_BN_CLICKED(IDC_CH15, OnCh15)
ON_BN_CLICKED(IDC_CH16, OnCh16)
ON_WM_TIMER()
ON_CBN_CLOSEUP(IDC_DWELL_LIST, OnCloseupDwellList)
ON_BN_CLICKED(IDC_ALARM_IN_RES, OnAlarmInRes)
ON_BN_CLICKED(IDC_ALARM_OUT_RES, OnAlarmOutRes)
ON_BN_CLICKED(IDC_ALARM_RELAY_RES, OnAlarmRelayRes)
ON_BN_CLICKED(IDC_AUTO_DWELL_RES, OnAutoDwellRes)
ON_BN_CLICKED(IDC_RESET_RES, OnResetRes)
ON_BN_CLICKED(IDC_DISPLAY_RES, OnDisplayTestRes)
ON_BN_CLICKED(IDC_RESET, OnReset)
ON_BN_CLICKED(IDC_ALARM_TEST_BUTTON, OnAlarmTestButton)
ON_BN_CLICKED(IDC_DISPLAY, OnDisplay)
ON_BN_CLICKED(IDC_VIDEO_FORMAT_PAL, OnVideoFormatPal)
ON_BN_CLICKED(IDC_VIDEO_FORMAT_NTSC, OnVideoFormatNtsc)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
ON_BN_CLICKED(IDC_RADIO14, OnRadio14)
ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
ON_BN_CLICKED(IDC_RADIO_CAPTURE, OnRadioCapture)
ON_BN_CLICKED(IDC_RADIO_PREVIEW, OnRadioPreview)
ON_BN_CLICKED(IDC_VIEWMODE1, OnViewmode1)
ON_BN_CLICKED(IDC_VIEWMODE16, OnViewmode16)
ON_BN_CLICKED(IDC_BTN_NEXTVIEW, OnBtnNextview)
ON_BN_CLICKED(IDC_BUTTON_PTZ, OnButtonPtz)//djx 2007/12/07
ON_BN_CLICKED(IDC_TEST_PTZ_RES, OnTestPtzRes)
ON_BN_CLICKED(IDC_AUDIO_RES, OnAudioRes)
ON_BN_CLICKED(IDC_RADIO_NET, OnRadioNet)
ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
ON_CBN_SELCHANGE(IDC_COMBO_D1, OnSelchangeComboD1)
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
        m_SetVideoFormatPal.SetCheck(BST_CHECKED);
        m_SetVideoFormatNtsc.SetCheck(BST_UNCHECKED);
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
        m_SetVideoFormatPal.SetCheck(BST_UNCHECKED);
        m_SetVideoFormatNtsc.SetCheck(BST_CHECKED);
    }
    
   // m_DB.SaveMode(sDlg.m_bSignal);
    
    LoadVideoRadio(0, 16, TRUE);//by chenlong
    m_switch.ShowWindow(TRUE);
    m_display.EnableWindow(TRUE);
    
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
    
    GetDlgItem(IDC_STATIC_YINPING)->ShowWindow(TRUE);
    LoadAudioRadio(0, 4, TRUE);//by chenlong
    m_Audio = 0;
    UpdateData(FALSE);
    m_card_infor.disname = "TD4104i";
    bresu = m_MyDSP->VideoCaptureStart(0, RecBmpHeader);
    
    if (!bresu) 
    {
        return FALSE;
    }
    m_MyDSP->SetSwitch(pDSPSwitch, 4);
  
    
    /*43L不需要音频by chenlong*/
    if (m_lpData[0] != '8' && m_lpData[0] != '9')
    {
        for (i = 0; i < m_Vchannel_Num; i++)
        {
            m_MyDSP->SetChannelStatus(CHSTATUS_SET_AUDIO_ENABLE, i, 1);
        }
    }
    else
    {
        for (i = 0; i < m_Vchannel_Num; i++)
        {
            m_MyDSP->SetChannelStatus(CHSTATUS_SET_AUDIO_ENABLE, i, 0);
        }
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

/*加载音频控件by chenlong*/
void CQCDlg::LoadAudioRadio(int nBegin, int nEnd, BOOL bShow)
{
    for (int n = nBegin; n < nEnd; n++)
    {
        GetDlgItem(IDC_RADIO1 + n)->ShowWindow(bShow);
    }
}

/*加载分辨率切换控件by chenlong*/
void CQCDlg::InitChangeVideoSize(int nID)
{
    GetDlgItem(IDC_STATIC_VIDEOFORMAT)->ShowWindow(TRUE);
    GetDlgItem(IDC_RADIO_CIF)->ShowWindow(TRUE);
    GetDlgItem(IDC_RADIO_D1)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_APPLY)->ShowWindow(TRUE);
    GetDlgItem(IDC_COMBO_D1)->ShowWindow(TRUE);
    GetDlgItem(IDC_COMBO_D1)->EnableWindow(FALSE);
    
    if (m_VideoSize == 0)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_CIF))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_D1))->SetCheck(FALSE);
    }
    else if (m_VideoSize == 1)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_CIF))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_RADIO_D1))->SetCheck(TRUE);
    }
    
    if (nID == IDC_RADIO_PREVIEW)
    {
        GetDlgItem(IDC_STATIC_VIDEOFORMAT)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_CIF)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_D1)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
    }
    else if(nID == IDC_RADIO_CAPTURE)
    {
        GetDlgItem(IDC_STATIC_VIDEOFORMAT)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_CIF)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_D1)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
    }
    else if (nID == IDC_RADIO_NET)
    {
        GetDlgItem(IDC_STATIC_VIDEOFORMAT)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_CIF)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_D1)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
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
    
    //m_C_accept.SetCheck(m_card_infor.resResult);
    //m_C_reject.SetCheck(!m_card_infor.resResult);
    //djx 08/3/3 以前是自动判断，如果有没通过的测试项则自动为拒绝，即使手动点了接收也是如此。现以手动选择为准。
    DWORD sta = 0x0003 & m_C_accept.GetState();
    m_card_infor.resResult=sta;
    
    
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
    CString str;
    if (ch_num > 0)
        m_list_dwell.AddString("自动");
    
    for (int i = 0; i < ch_num; i++)
    { 
        str.Format("%s%d", "通道 ", i + 1);
        m_list_dwell.AddString(str);
    }
    
    int cnt = 16;
    if ((cnt--) > ch_num)
    {
        m_cCh16.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh15.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh14.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh13.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh12.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh11.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh10.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh9.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh8.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh7.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh6.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    }
    else
    {
        return ;
    }
    
    if ((cnt--) > ch_num)
    {
        m_cCh5.ShowWindow(FALSE);
        m_ChShow[cnt] = FALSE;
    } 
    else
    {
        return ;
    }
    
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
        m_switch.ShowWindow(FALSE);
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
    
    if (HaveView())
        RefreshChannelCheckStatus(0);
    else
    {
        m_ch1=!m_ch1;
        UpdateData(FALSE);
    }
    
}

void CQCDlg::OnCh2() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(1);
    else
    {
        m_ch2=!m_ch2;
        UpdateData(FALSE);
    }
    
}

void CQCDlg::OnCh3() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(2);
    else
    {
        m_ch3=!m_ch3;
        UpdateData(FALSE);
    }
    
}

void CQCDlg::OnCh4() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(3);
    else
    {
        m_ch4=!m_ch4;
        UpdateData(FALSE);
    }
    
}

void CQCDlg::OnCh5() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(4);
    else
    {
        m_ch5=!m_ch5;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh6() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(5);
    else
    {
        m_ch6=!m_ch6;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh7() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(6);
    else
    {
        m_ch7=!m_ch7;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh8() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(7);
    else
    {
        m_ch8=!m_ch8;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh9() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(8);
    else
    {
        m_ch9=!m_ch9;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh10() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(9);
    else
    {
        m_ch10=!m_ch10;
        UpdateData(FALSE);
    }	
}

void CQCDlg::OnCh11() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(10);
    else
    {
        m_ch11=!m_ch11;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh12() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(11);
    else
    {
        m_ch12=!m_ch12;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh13() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(12);
    else
    {
        m_ch13=!m_ch13;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh14() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(13);
    else
    {
        m_ch14=!m_ch14;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh15() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(14);
    else
    {
        m_ch15=!m_ch15;
        UpdateData(FALSE);
    }
}

void CQCDlg::OnCh16() 
{
    if (!m_bCapturing)
    {
        return ;
    }
    if (HaveView())
        RefreshChannelCheckStatus(15);
    else
    {
        m_ch16=!m_ch16;
        UpdateData(FALSE);
    }
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
    m_ChChecked[4]=m_ch5=FIRSTINVIEW;
    m_ChChecked[5]=m_ch6=FIRSTINVIEW;
    m_ChChecked[6]=m_ch7=FIRSTINVIEW;
    m_ChChecked[7]=m_ch8=FIRSTINVIEW;
    m_ChChecked[8]=m_ch9=FIRSTINVIEW;
    m_ChChecked[9]=m_ch10=FIRSTINVIEW;
    m_ChChecked[10]=m_ch11=FIRSTINVIEW;
    m_ChChecked[11]=m_ch12=FIRSTINVIEW;
    m_ChChecked[12]=m_ch13=FIRSTINVIEW;
    m_ChChecked[13]=m_ch14=FIRSTINVIEW;
    m_ChChecked[14]=m_ch15=FIRSTINVIEW;
    m_ChChecked[15]=m_ch16=FIRSTINVIEW;
    m_c_reset_res.SetWindowText("√");
    m_Alarmtest=FALSE;
    m_bCanDoSectionTest=FALSE;
    m_bCapturing=FALSE;
    m_havePciDvrCard=FALSE;
    m_haveUsbDvrCard=FALSE;
    m_haveAlarmCard=FALSE;
    m_dwVideoFormat=DVRVIDEO_STANDARD_PAL;
    m_video_size_type=DVRVIDEO_SIZE_320x240;
    m_SetVideoFormatPal.SetCheck(1);
    m_C_accept.SetCheck(1);
    m_C_sn.SetLimitText(20);
    char szBuf[1024];
    memset(szBuf, 0, 1024);
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
	// heliang -
    if (bufs->ChannelIndex == m_AudioChannel)
    {
        m_WaveOut.WriteBuf(bufs->pBuf, bufs->BufLen);
    }  
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

/*D1下根据当前的组数显示对应的通道 by chenlong*/
BOOL CQCDlg::D1Filter(int nIndex)
{
    switch (m_D1ViewRadio)
    {
    case 0:
        {
            if (nIndex != 0 && nIndex != 1 && nIndex != 2 && nIndex != 3)
            {
                return FALSE;
            }
        }
        break;
    case 1:
        {
            if (nIndex != 4 && nIndex != 5 && nIndex != 6 && nIndex != 7)
            {
                return FALSE;
            }
        }
        break;
    case 2:
        {
            if (nIndex != 8 && nIndex != 9 && nIndex != 10 && nIndex != 11)
            {
                return FALSE;
            }
        }
        break;
    case 3:
        {
            if (nIndex != 12 && nIndex != 13 && nIndex != 14 && nIndex != 15)
            {
                return FALSE;
            }
        }
        break;
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

BOOL CQCDlg::HaveView()
{
    BOOL ret = FALSE;
    
    UpdateData(TRUE);
    m_ChChecked[0]=m_ch1;   
    m_ChChecked[1]=m_ch2;    
    m_ChChecked[2]=m_ch3;    
    m_ChChecked[3]=m_ch4;
    m_ChChecked[4]=m_ch5;   
    m_ChChecked[5]=m_ch6;    
    m_ChChecked[6]=m_ch7;    
    m_ChChecked[7]=m_ch8;
    m_ChChecked[8]=m_ch9;   
    m_ChChecked[9]=m_ch10;   
    m_ChChecked[10]=m_ch11;  
    m_ChChecked[11]=m_ch12;
    m_ChChecked[12]=m_ch13; 
    m_ChChecked[13]=m_ch14;  
    m_ChChecked[14]=m_ch15;  
    m_ChChecked[15]=m_ch16;
    
    return TRUE;
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

void CQCDlg::OnCloseupDwellList() 
{
    // TODO: Add your control notification handler code here
    if ((m_havePciDvrCard || m_haveUsbDvrCard) && m_bCanDoSectionTest)
    {
        if (0 == m_list_dwell.GetCurSel())
        {
            SetTimer(ON_TIME_AUTOCALLMONITOR, 2000, NULL);
        }
        else if (m_list_dwell.GetCurSel() > 0)
        {
            KillTimer(ON_TIME_AUTOCALLMONITOR);
            //CallMonitor(FALSE);
        }
        else 
            KillTimer(ON_TIME_AUTOCALLMONITOR);
    }
}

void CQCDlg::OnAlarmInRes() 
{
AfxMessageBox("heliang -");
}

void CQCDlg::OnAlarmOutRes() 
{
    AfxMessageBox("heliang -");
}

void CQCDlg::OnAlarmRelayRes() 
{
    AfxMessageBox("heliang -");
}

void CQCDlg::OnAutoDwellRes() 
{
AfxMessageBox("heliang -");
}

void CQCDlg::OnResetRes() 
{
    AfxMessageBox("heliang -");
}

void CQCDlg::OnDisplayTestRes() 
{
    CString str;
    m_c_display_res.GetWindowText(str);
    if (str=="√")
    {
        m_c_display_res.SetWindowText("X");
        m_card_infor.resSetSwitch=FALSE;
    }
    else if (str=="X"||str=="")
    {
        m_c_display_res.SetWindowText("√");
        m_card_infor.resSetSwitch=TRUE;
    }
}


void CQCDlg::OnReset() 
{
    //m_TestCard.ResetComputer();
    AfxMessageBox("测试自动重启功能!");
    RedrawWindow();
    BeginWaitCursor();
    Sleep(5000);
    EndWaitCursor();
    m_reset.EnableWindow(FALSE);
    m_c_reset_res.SetWindowText("X");
    m_card_infor.resReset=FALSE;
    AfxMessageBox("自动重启失败!");
}

void CQCDlg::OnAlarmTestButton() 
{   
    AfxMessageBox("heliang -");
}

//djx 2007/12/07
void CQCDlg::OnButtonPtz() 
{
    AfxMessageBox("PTZ Dlg Remove! heliang -!");
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
        
        m_res_sub_id = m_card_infor.resSubId ? "√" : "X";			//系统ID烧录窗口
        m_res_install_driver = m_card_infor.resDriver ? "√" : "X";	//驱动安装窗口
        
        m_res_pass_word = m_card_infor.resPassWord ? "√" : "X";	//加密测试窗口
       
    }
  
    if (m_Audio != -1) //djx 08/3/3 添加音频测试结果
    {
        GetDlgItem(IDC_AUDIO)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_AUDIO_RES)->ShowWindow(SW_SHOW);
    }
}

void CQCDlg::OnDisplay() 
{  
     AfxMessageBox("heliang -");
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

//选择PAL制式的处理
/*修改了制式选择的处理方式 by chenlong*/
void CQCDlg::OnVideoFormatPal()
{
    VideoFormatChange(DVRVIDEO_STANDARD_PAL);
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

//选择NTSC制式的处理
/*修改了制式选择的处理方式 by chenlong*/
void CQCDlg::OnVideoFormatNtsc()
{
    VideoFormatChange(DVRVIDEO_STANDARD_NTSC);
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

void CQCDlg::OnRadio1() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio10() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio11() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio12() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio13() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio14() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio15() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio16() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio2() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio3() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio4() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio5() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio6() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio7() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio8() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
}

void CQCDlg::OnRadio9() 
{
    UpdateData(TRUE);
    m_AudioChannel = m_Audio;
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

void CQCDlg::OnTestPtzRes() 
{
    AfxMessageBox("helang-");
}


void CQCDlg::FreshAcceptButton() //djx
{
    m_card_infor.resResult = TRUE;
    
    if (m_card_infor.card_type_id == DVR_CARD_TDNULL)
        m_card_infor.resResult = FALSE;
    
    m_C_accept.SetCheck(m_card_infor.resResult);
    m_C_reject.SetCheck(!m_card_infor.resResult);
}

//djx 08/3/3
void CQCDlg::OnAudioRes() 
{
    CString str;
    GetDlgItem(IDC_AUDIO_RES)->GetWindowText(str);
    if (str=="√")
    {
        GetDlgItem(IDC_AUDIO_RES)->SetWindowText("X");
        m_card_infor.resAudio=FALSE;
    }
    else if (str=="X"||str=="")
    {
        GetDlgItem(IDC_AUDIO_RES)->SetWindowText("√");
        m_card_infor.resAudio=TRUE;
    }
    
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

/*优化了点击“网络数据”按钮的处理 by chenlong*/
void CQCDlg::OnRadioNet() 
{
    if (m_bNet)
    {
        return;
    }
}

/*点击Apply按钮的处理 by chenlong*/
void CQCDlg::OnBtnApply() 
{
    int nVideoSize = 0;
    if (((CButton*)GetDlgItem(IDC_RADIO_CIF))->GetCheck())
    {
        nVideoSize = 0;
    }
    else
    {
        nVideoSize = 1;
    }
    
    if (m_VideoSize == nVideoSize)
    {
        return;
    }

    if (OnChangeVideoSize(nVideoSize))
    {
        InitComboD1(m_VideoSize);
    }
}

/*初始化D1分辨率下的部分控件by chenlong*/
void CQCDlg::InitComboD1(int videosize)
{
    if (m_lpData[0] == '0' || m_lpData[0] == '1' || videosize != 1)
    {
        m_ComboD1.EnableWindow(FALSE);
        return;
    }

    m_ComboD1.EnableWindow(TRUE);
    m_ComboD1.ResetContent();
    CString str("");
    for (int i = 0; i < m_Vchannel_Num / 4; i++)
    {
        str.Format("第%d组", i);
        m_ComboD1.AddString(str);
    }
    m_ComboD1.SetCurSel(0);
    m_D1ViewRadio = 0;
}

/*分辨率切换的处理 by chenlong*/
BOOL CQCDlg::OnChangeVideoSize(int videosize)
{
    if (m_bPreview || m_bNet)
    {
        return FALSE;
    }
    
    m_BtnApply.EnableWindow(FALSE);
    m_ProSizeChange.ShowWindow(TRUE);
    m_ProSizeChange.SetRange(0,100);
    BOOL brec = FALSE;
    if (m_bRec)
    {
        brec = m_bRec;
        m_bRec = FALSE;
    }
    
    if (m_hLossThread != NULL)
    {
        SuspendThread(m_hLossThread);
    }
    m_ProSizeChange.SetPos(5);
    
    BITMAPINFOHEADER RecBmpHeader;
    
    m_ProSizeChange.SetPos(10);  
    if (brec)
    {
        WaitForSingleObject(g_eventRec,INFINITE);
        if (m_pDecodedData_Rec != NULL)
        {
            delete [] m_pDecodedData_Rec;
            m_pDecodedData_Rec = NULL;
        }
        
        int width = 0;
        int height = 0;
        if (videosize == 0)
        {
            width = 352;
            if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
            {
//                TRACE("352*288\n");
                height = 288;
            }
            else
            {
//                TRACE("352*240\n");
                height = 240;
            }
            m_pDecodedData_Rec = new BYTE[352*395*4];
        }
        else if (videosize == 1)
        {
            width = 704;
            if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
            {
//                TRACE("704*576\n");
                height = 576;
            }
            else
            {
//                TRACE("352*480\n");
                height = 480;
            }
            m_pDecodedData_Rec = new BYTE[720*576*4];
            
        }
        else
        {
            SetEvent(g_eventRec);
            m_ProSizeChange.SetPos(100);
            return FALSE;
        }
        
        if (m_pDecodedData_Rec == NULL)
        {
            SetEvent(g_eventRec);
            m_ProSizeChange.SetPos(100);
            return FALSE;
        }
        m_ProSizeChange.SetPos(15);
        RecBmpHeader.biWidth = width;
        RecBmpHeader.biHeight = height;
        
        int i;
        for (i = 0; i < m_Vchannel_Num; i++)
        {
            m_CodecMgrH_Rec[i].DestroyCodecV();
            m_CodecMgrH_Rec[i].SetFormat(CSize(width, height));
            FormatCodecMgrH(i, 0);
        }
        m_ProSizeChange.SetPos(35);
        m_ddraw->FreeDSPBack();
        if (videosize == 0)
        {
            m_ddraw->InitBack(m_dwVideoFormat,FALSE,/*8*/m_Vchannel_Num);
        }
        else if (videosize == 1)
        {
            m_ddraw->InitBack(m_dwVideoFormat,TRUE,/*8*/m_Vchannel_Num);
        }
        else
        {
            SetEvent(g_eventRec);
            m_ProSizeChange.SetPos(100);
            return FALSE;
        }
        m_ProSizeChange.SetPos(55);
        DWORD dwType;
        BOOL fg = m_MyDSP->DeviceInitial(m_dwVideoFormat, VideoCALLBACKFUNC, AudioCALLBACKFUNC, m_hWnd, &dwType);
        if (!fg)
        {
            SetEvent(g_eventRec);
            return FALSE;
        }
        m_card_infor.resPassWord = m_MyDSP->Password();
        m_ProSizeChange.SetPos(65);
		/*录象数据下，不同分辨率的帧率不同by chenlong*/
        if (videosize == 0)
        {
            for (int n = 0; n < m_Vchannel_Num; n++)
            {
                m_MyDSP->SetChannelStatus(CHSTATUS_SET_REC_RESOLUTION, n, 0);
            }
            if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
            {
                for (i=0;i</*8*/m_Vchannel_Num;i++)
                {
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_NEXTISKEYFRAME,i,1);
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_RECFRATE,i,25);
                }
            }
            else
            {
                for (i=0;i</*8*/m_Vchannel_Num;i++)
                {
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_NEXTISKEYFRAME,i,1);
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_RECFRATE,i,30);
                }
            }
        }
        else if (videosize == 1)
        {
            for (int n = 0; n < m_Vchannel_Num; n++)
            {
                m_MyDSP->SetChannelStatus(CHSTATUS_SET_REC_RESOLUTION, n, 1);
            }
            if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
            {
                for (i=0;i</*8*/m_Vchannel_Num;i++)
                {
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_NEXTISKEYFRAME,i,1);
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_RECFRATE,i,8);
                }
            }
            else
            {
                for (i=0;i</*8*/m_Vchannel_Num;i++)
                {
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_NEXTISKEYFRAME,i,1);
                    m_MyDSP->SetChannelStatus(CHSTATUS_SET_RECFRATE,i,10);
                }
            }
        }
        else
        {
            SetEvent(g_eventRec);
            m_ProSizeChange.SetPos(100);
            return FALSE;
        }
        m_ProSizeChange.SetPos(85);
        
        if (m_hLossThread != NULL)
        {
            ResumeThread(m_hLossThread);
        }
        
        SetEvent(g_eventRec);
        Sleep(1500);
        m_VideoSize = videosize;
        m_RECWidth = width;
        m_RECHeight = height;
        m_bRec = TRUE;
        m_BtnApply.EnableWindow(TRUE);
        m_ProSizeChange.SetPos(100);
        m_ProSizeChange.ShowWindow(FALSE);
    }
    return TRUE;
}

/*视频制式变更的处理 by chenlong*/
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
/*D1下,组别变换的响应 by chenlong*/
void CQCDlg::OnSelchangeComboD1() 
{
    m_D1ViewRadio = m_ComboD1.GetCurSel();
    m_bFresh = TRUE;
}





















