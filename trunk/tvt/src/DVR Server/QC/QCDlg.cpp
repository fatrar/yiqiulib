// QCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QC.h"
#include "QCDlg.h"
//#include "ioctlV.h"
//#include "overlay.h"
#include "SignalDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIRSTINVIEW TRUE
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

int g_card_id;
CQCDlg *main;

//#define UI_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQCDlg dialog

CQCDlg::CQCDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQCDlg::IDD, pParent)
{
    DebugOut("CQCDlg:CQCDlg()\n");
    main=this;

    //{{AFX_DATA_INIT(CQCDlg)

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CQCDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQCDlg, CDialog)
    //{{AFX_MSG_MAP(CQCDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_FINISH, OnFinish)
    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_Show_IV_Config, &CQCDlg::OnBnClickedShowIvConfig)
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQCDlg message handlers

BOOL CQCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    //CMenu* pSysMenu = GetSystemMenu(FALSE);
    //if (pSysMenu != NULL)
    //{
    //    CString strAboutMenu;
    //    strAboutMenu.LoadString(IDS_ABOUTBOX);
    //    if (!strAboutMenu.IsEmpty())
    //    {
    //        pSysMenu->AppendMenu(MF_SEPARATOR);
    //        pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    //    }
    //}
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    //SetIcon(m_hIcon, FALSE);		// Set small icon
    
    int xScreen=0,yScreen=0; //djx 07/07/13
    xScreen = GetSystemMetrics(SM_CXSCREEN);
    yScreen = GetSystemMetrics(SM_CYSCREEN);
    xScreen>1024?xScreen=-1:xScreen=0;
    yScreen>768?yScreen=-1:yScreen=0;
    MoveWindow(xScreen, yScreen, 1024, 768);
  
#ifdef UI_DEBUG
    return TRUE;
#endif // UI_DEBUG
    
    InitialVar();

    g_card_id = m_card_infor.card_type_id = DVR_CARD_TD4104i;
                 
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
        RecBmpHeader.biWidth = 352;
        RecBmpHeader.biHeight = 240;
    }
       
    /*加载匹配的dll*/
    m_DSPDLL = NULL;
    m_MyDSP = NULL;   
      
    CString strDllPath = m_ExePath + "DEV_HC_DM642i.dll";
    m_DSPDLL = ::LoadLibrary((char*)(LPCTSTR)strDllPath);
    
    /*加载完Dll后的操作*/
    m_MyDSP = (CDeviceManager *)::GetProcAddress(m_DSPDLL, "g_Dev_Object");
    ASSERT(m_MyDSP != NULL);
    
    DWORD dwType;
    BOOL bresu = m_MyDSP->DeviceInitial(m_dwVideoFormat, VideoCALLBACKFUNC,AudioCALLBACKFUNC, m_hWnd, &dwType);
    if (!bresu) 
    {
        return FALSE;
    }
    
    m_pSnapShotSender = SnapShotWnd::CreateSnapShotWnd(m_hWnd);

    GeIVDeviceSetterFn f1 = 
        (GeIVDeviceSetterFn)::GetProcAddress(
        m_DSPDLL, g_szIVDeviceFuncName[GeIVDeviceSetter_Index] );
    IIVDeviceSetter* pIVDeviceSetter = f1();
    pIVDeviceSetter->SetIVDataCallBack( IVLiveFactory::GetDataSender() );
    pIVDeviceSetter->SetSnapShotCallBack(m_pSnapShotSender);

    GetIVDeviceBase2Fn f2 = 
        (GetIVDeviceBase2Fn)::GetProcAddress(
        m_DSPDLL, g_szIVDeviceFuncName[GetIVDeviceBase2_Index]);;
    SetIVOpeator(f2());

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
    m_MyDSP->SetSwitch(pDSPSwitch, 4);
  
    m_MyDSP->VideoCaptureStart(0,RecBmpHeader);

    for (i = 0; i < m_Vchannel_Num; i++)
    {
        m_MyDSP->SetChannelStatus(CHSTATUS_SET_AUDIO_ENABLE, i, 0);
    }

    UpdateData(FALSE);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQCDlg::OnPaint() 
{
    CPaintDC dc(this);
    //if (IsIconic())
    //{
    //    CPaintDC dc(this); // device context for painting
    //    
    //    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
    //    
    //    // Center icon in client rectangle
    //    int cxIcon = GetSystemMetrics(SM_CXICON);
    //    int cyIcon = GetSystemMetrics(SM_CYICON);
    //    CRect rect;
    //    GetClientRect(&rect);
    //    int x = (rect.Width() - cxIcon + 1) / 2;
    //    int y = (rect.Height() - cyIcon + 1) / 2;
    //    
    //    // Draw the icon
    //    dc.DrawIcon(x, y, m_hIcon);
    //}
    //else
    //{
    //    CDialog::OnPaint();

#ifdef UI_DEBUG
        return;
#endif // UI_DEBUG

        if (m_card_infor.card_type_id != DVR_CARD_TD3316)
        {
            m_ddraw->DrawBack();
        }
        else
            m_ddraw->DrawOverlayBack();
    //}
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
#ifdef UI_DEBUG
    ReleaseIVConfigDlg();
    CDialog::OnOK();
    return;
#endif // UI_DEBUG

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
    
    ReleaseIVConfigDlg();
    CDialog::OnOK();
}

void CQCDlg::InitialVar()
{   
    m_ddraw = new CDirectDraw(m_hWnd);
    m_ddraw->IniDirectDraw();
    m_dwVideoFormat=DVRVIDEO_STANDARD_PAL;
    char szBuf[1024] = {0};
    DWORD len = 1024;
    if(GetModuleFileName(NULL,szBuf,len)!=0)
    {
        m_ExePath=szBuf;
        m_ExePath=m_ExePath.Left(m_ExePath.ReverseFind ('\\')+1);
    }  
}

BOOL CQCDlg::AudioCALLBACKFUNC(FRAMEBUFSTRUCT *bufs)
{
    // 暂时不处理声音
    return FALSE;
}

BOOL CQCDlg::VideoCALLBACKFUNC(FRAMEBUFSTRUCT *bufs)
{
    if (main->m_MyDSP == NULL)
    {
        return FALSE;
    }

    // 暂时只管live stream
    if (bufs->nStreamID == VIDEO_STREAM_PREVIEW)
    {
        // 这样显示效率不高，因为没有必要每次都算区域，因为只有在移动的时候才算
        // 那么要获取CQCDlg的OnSize消息，纠正实际的Rect
        // []  heliang
        CRect Rect;
        FILETIME* time = (FILETIME*)&bufs->localTime;
        main->GetClientRect(&Rect);
        main->ClientToScreen(&Rect);
        main->m_ddraw->ShowDSP(
            Rect, *time,
            bufs->ChannelIndex, bufs->pBuf, main->m_card_infor.video_channel_number,YUV422); 
        main->m_MyDSP->ReleaseBuffer(1, bufs->BufferPara);
        return TRUE;
	}

    return FALSE;
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

void CQCDlg::OnDestroy() 
{
    CDialog::OnDestroy();
#ifdef UI_DEBUG
    return;
#endif // UI_DEBUG
 
	// heliang -
    if (m_card_infor.card_type_id == DVR_CARD_TD4104 ||
		m_card_infor.card_type_id == DVR_CARD_TD4104i )
    {
        m_ddraw->FreeDSPBack();
    }
    
    delete m_ddraw;  
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

void CQCDlg::OnBnClickedShowIvConfig()
{
    CfgDlg.DoModal();
}

void CQCDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    TRACE("OnRButtonDown");
    CDialog::OnRButtonDown(nFlags, point);
}
