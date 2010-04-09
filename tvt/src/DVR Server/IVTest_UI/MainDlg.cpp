/**CPP*************************************************************************
 File            : maindlg.CPP
 Subsystem       : 
 Function Name(s): Cmaindlg
 Author          : YiQiu
 Date            : 2010-3-25  
 Time            : 17:24
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "resource.h"
#include "MainDlg.h"
#include "VideoStdDLg.h"


static CMainDlg* g_This;

CMainDlg::CMainDlg()
    : m_pDeviceManager(NULL)
    , m_Hinstance(NULL)
    , m_pSnapShotSender(NULL)
    , m_dwVideoFormat(0)
    , m_dwChannelCount(Max_Channel)
{
    g_This = this;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
        {
            return TRUE;
        }
    }
    return CWindow::IsDialogMessage(pMsg);
}

LRESULT CMainDlg::OnInitDialog(
    UINT /*uMsg*/, WPARAM /*wParam*/, 
    LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    MoveWindow(0, 0, 1024, 768);
    CenterWindow();

    // set icons
    //HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
    //    IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    //SetIcon(hIcon, TRUE);
    //HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
    //    IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    //SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    Init();
    InitPlayer();
    return TRUE;
}

LRESULT CMainDlg::OnDestroy(
    UINT /*uMsg*/, WPARAM /*wParam*/,
    LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    if ( m_pDeviceManager )
    {
        m_pDeviceManager->VideoCaptureStop();
        m_pDeviceManager->DeviceFree();
        m_pDeviceManager = NULL;
        ::FreeLibrary(m_Hinstance);
    }
    return 0;
}

LRESULT CMainDlg::OnClose(void)
{
    DestroyWindow();
    ::PostQuitMessage(IDOK);
    return 1;
}

BOOL CMainDlg::VideoComing( FRAMEBUFSTRUCT *bufs )
{
    if ( g_This == NULL || 
         g_This->m_pDeviceManager == NULL)
    {
        return FALSE;
    }

    // ÔÝÊ±Ö»¹Ülive stream
    if (bufs->nStreamID != VIDEO_STREAM_PREVIEW)
    {
         return FALSE;
    }
    
    g_This->m_PlayerWnd[bufs->ChannelIndex].VideoComing(
        g_This->m_dwVideoFormat,
        bufs);
    g_This->m_pDeviceManager->ReleaseBuffer(
        1, bufs->BufferPara);
    return TRUE;
}

BOOL CMainDlg::AudioComing( FRAMEBUFSTRUCT *bufs )
{
    return FALSE;
}

void CMainDlg::Init()
{
    CVideoStdDLg StdDlg;
    StdDlg.DoModal();

    BITMAPINFOHEADER RecBmpHeader;
    if (StdDlg.m_bSignal == 0)
    {
        m_dwVideoFormat = DVRVIDEO_STANDARD_PAL;
        RecBmpHeader.biWidth = 352;
        RecBmpHeader.biHeight = 288;
    }
    else
    {
        m_dwVideoFormat = DVRVIDEO_STANDARD_NTSC;   
        RecBmpHeader.biWidth = 352;
        RecBmpHeader.biHeight = 240;
    }

    /**
    *@note 1. Load Library
    */
    m_Hinstance = ::LoadLibrary(_T("DEV_HC_DM642i.dll"));
    if ( m_Hinstance == NULL )
    {
        MessageBox(_T("Not Found DEV_HC_DM642i.dll!"));
        return;
    }

    /**
    *@note 2. Get Device Object and Init Device
    */
    m_pDeviceManager = (CDeviceManager*)::GetProcAddress(
        m_Hinstance, "g_Dev_Object");
    assert(m_pDeviceManager != NULL);

    DWORD dwType;
    BOOL bRc = m_pDeviceManager->DeviceInitial(
        m_dwVideoFormat, VideoComing, AudioComing,
        m_hWnd, &dwType);
    if (!bRc) 
    {
        return;
    }

    /**
    *@note 3.  Get IV operator Object and Init IV Device 
    *      3.1 Get IV Device Setter
    */
    GeIVDeviceSetterFn f1 = 
        (GeIVDeviceSetterFn)::GetProcAddress(
        m_Hinstance, g_szIVDeviceFuncName[GeIVDeviceSetter_Index] );
    IIVDeviceSetter* pIVDeviceSetter = f1();

    /**
    *@note 3.2 Use IV Device Setter, Init IV Alarm Callback and Statistic Callback
    */
    pIVDeviceSetter->SetIVDataCallBack(
        IVLiveFactory::GetDataSender(),
        IVLiveFactory::GetStatisticFresher() );

    /**
    *@note 3.3 Create SnapShot Window, And  Use IV Device Setter Set to IV Device
    */
    m_pSnapShotSender = SnapShotWnd::CreateSnapShotWnd(m_hWnd);
    pIVDeviceSetter->SetSnapShotCallBack(m_pSnapShotSender);

    /**
    *@note 4 Get IVDeviceBase2 Interface, And Set To IVUI
    */
    GetIVDeviceBase2Fn f2 = 
        (GetIVDeviceBase2Fn)::GetProcAddress(
        m_Hinstance, g_szIVDeviceFuncName[GetIVDeviceBase2_Index]);
    IVUIFactory::SetIVOpeator(f2());
    
    /**
    *@note 5 Load IV Config And Set To Device
    */
    IVUIFactory::InitIVConfig();


    /**
    *@note 6 General Device Init
    */
    //m_card_infor.resPassWord = m_pDeviceManager->Password();
    m_dwChannelCount = m_pDeviceManager->GetCardOpt(DVRCARDOPT_CHANNEL_NUM);
    DWORD szSwitch[16] = {0};
    for (DWORD i = 0; i < m_dwChannelCount; i++)
    {
        szSwitch[i] = 1;
    }	

    m_pDeviceManager->SetSwitch(szSwitch, 4);
    m_pDeviceManager->VideoCaptureStart(0, RecBmpHeader);

    for (DWORD i = 0; i < m_dwChannelCount; i++)
    {
        m_pDeviceManager->SetChannelStatus(CHSTATUS_SET_AUDIO_ENABLE, i, 0);
    }
}

void CMainDlg::InitPlayer()
{
    for (int i=0; i<Max_Channel; ++i)
    {
        m_PlayerWnd[i].Init(
            *this,
            g_pVideoRect[m_dwVideoFormat][i],
            g_pFormat[m_dwVideoFormat][0],
            g_pFormat[m_dwVideoFormat][1] );
    }
}


LRESULT CMainDlg::OnConfigIvconfig(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    m_CIVCfgDlg.DoModal();
    return 0;
}


// End of file


