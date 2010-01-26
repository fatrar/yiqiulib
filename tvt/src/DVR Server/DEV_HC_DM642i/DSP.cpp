// DSP.cpp: implementation of the CDSP class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DSP.h"
#include "TiCoffFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

template<int BufSize>
BOOL FindTBuf(
    int nDevice,
    int &nIndex,
    CCriticalSection cs[],
    FRAMEBUFSTRUCT (*f)[BufSize] )
{
    AutoLockAndUnlock(cs[nDevice]);
    static int s_QucikIndex = 0;
    if (f[nDevice][s_QucikIndex].nVLostFlag == 1)
    {
        f[nDevice][s_QucikIndex].nVLostFlag = 0;
        nIndex = s_QucikIndex;
        s_QucikIndex = (s_QucikIndex+1)%BufSize;
        return TRUE;
    }

    for (int i = 0; i < BufSize; i++)
    {
        if (f[nDevice][i].nVLostFlag == 1)
        {
            f[nDevice][i].nVLostFlag = 0;
            nIndex = i;
            s_QucikIndex = (i+1)%BufSize;
            return TRUE;
        }
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
inline	void ReleaseDriverBuffer(PTVT_CAP_STATUS pST)
{
//	::InterlockedDecrement((PLONG)(&((pST)->dwReserve4)));
	static CCriticalSection	cs;

	cs.Lock();
	pST->dwReserve4--;

//<----cs作用:多线程重入；防多线程调用时(传入指针相同)，在此切换线程，两个线程分别执行减一操作后
//dwReserve4==0，这时两个线程会分别将byLock置0，可能导致驱动层刚在第一次置0时填充的BUF重新被释放

	if((pST)->dwReserve4 == 0)
	{
		(pST)->byLock = 0;
	}
	cs.Unlock();
}
#if 0
/*
函数功能 : 转变帧头4BYTE格式
参数说明 :
p4B[IN/OUT] : 要转变的4字节地址
bDirection[IN]: 转变方向(TRUE: 序号->关键帧标示，FALSE: 关键帧标示->序号)
*/
inline	void FrameHead_4B_Convert(DWORD* p4B, BOOL bDirection, DWORD dwCompress)
{
	DWORD	dw4Byte = *p4B;
	DWORD dwKey, dwNotKey;

	if(dwCompress == DWORD('TVTX'))
	{
		dwKey = 0x00000101;
		dwNotKey = 0x000001B6;
	}
	else if(dwCompress == DWORD('XVID'))
	{
		dwKey = 0x0000B001;
		dwNotKey = 0x0000B601;
	}

	if(bDirection)
	{
		if(dw4Byte & 0x01000000)
		{
			*p4B = dwKey;
		}
		else
		{
			*p4B = dwNotKey;
		}
	}
}
#endif

CDSP::CDSP()
    : m_AlarmCallBackFn(NULL)
    , m_pAlarmCallBackParm(NULL)
    , m_pIVDataSender(NULL)
    , m_pSnapShotSender(NULL)
    , m_bNetstart(FALSE)
    , m_bQuit(FALSE)
    , m_dwPrvBufSize(0)
    , m_pVideoCallBack(NULL)
    , m_pAudioCallBack(NULL)
    , m_nDeviceNum(0)
    , m_dwVideoFormat(DVRVIDEO_STANDARD_PAL)
{
    int i;
    for (i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        m_pSignal[i].bSignal = 1;

        m_ftNetLastFrameTime[i] = m_CounterTime.GetCurrentTime();
        *(ULONGLONG *)&m_ftNetLastFrameTime[i] += i * 80 * 10000;//均匀分配
        m_pSwitch[i] = 0;
        m_pAudioSwitch[i] = 1;
    }

    // Init FRAMEBUFSTRUCT
    ZeroMemory(m_pAudBuf, sizeof(m_pAudBuf));
    ZeroMemory(m_pCapBuf, sizeof(m_pCapBuf));
    ZeroMemory(m_pPrvBuf, sizeof(m_pPrvBuf));
    ZeroMemory(m_pNetBuf, sizeof(m_pNetBuf));
    ZeroMemory(m_pMobileBuf, sizeof(m_pMobileBuf));
    ZeroMemory(m_pNetBuf_RT, sizeof(m_pNetBuf_RT));  //<REC-NET>

    // Init PTVT_CAP_STATUS
    ZeroMemory(m_pDrvHeadOfNetBuf, sizeof(m_pDrvHeadOfNetBuf));
    ZeroMemory(m_pDrvHeadOfMobileBuf, sizeof(m_pDrvHeadOfMobileBuf));
    ZeroMemory(m_pDrvHeadOfCapBuf, sizeof(m_pDrvHeadOfCapBuf));
    ZeroMemory(m_pDrvHeadOfPrvBuf, sizeof(m_pDrvHeadOfPrvBuf));

    for (i = 0; i < MAX_DEVICE_NUM; i++)
    {
        m_hDevice[i] = NULL;
        m_hAudEvent[i] = NULL;
        m_hCompressEvent[i] = NULL;	//zhangzhen	2007/02/28
        m_hPrvEvent[i] = NULL;
        m_hThreadAud[i] = NULL;
        m_hThreadPrv[i] = NULL;
        m_hThreadCompressStrm[i] = NULL;	//zhangzhen	2007/02/28	
    }

    //压缩流关键帧控制变量，防意外丢帧/编码器异常(长期不送关键帧)
    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        m_bNextFrameIsKeyRcd[i]	= TRUE;	//zhangzhen	2007/03/01
        m_bNextFrameIsKeyNet[i]	= TRUE;	//zhangzhen	2007/03/01
        m_bNextFrameIsKeyMobile[i]	= TRUE;	//zhangzhen	2007/03/01
        m_nFrameCntRcd[i]	= 0;
        m_nFrameCntNet[i]	= 0;
        m_nFrameCntMobile[i]	= 0;
        m_bNextFrameIsKeyNet_RT[i]	= TRUE;	//<REC-NET>
        m_bRecordStop[i] = TRUE;	//<REC-NET>初始状态，所有通道录像全部关闭
    }

    //网络流压缩开关	Add By zhangzhen 2007/10/11
    m_dwNetChannelMask = 0;
    //初始网络帧率增量为0	Add By zhangzhen 2007/10/17
    m_nNetFrameRateInc = 0;

    // IV .. Init
    memset(m_szCurrentIVChannel, -1, sizeof(m_szCurrentIVChannel));
    m_szCurrentIVChannel[0] = 1;
}

CDSP::~CDSP()
{
}

#define PCI_DEVICE_STRING "\\\\.\\DmtiD"

BOOL CDSP::DeviceInit()
{
	int i;
	char szName[20];
	static APP_DRIVER_BUFFER_INFO stIn;
	static APP_DRIVER_BUFFER_INFO stOut;

	DWORD dwBytesReturned;

	for (i = 0; i < MAX_DEVICE_NUM; i++)
	{
		sprintf(szName, "%s%d", PCI_DEVICE_STRING, i + 1);
		m_hDevice[i] = CreateFile(
            szName, 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (m_hDevice[i] == INVALID_HANDLE_VALUE)
		{
            m_hDevice[i] = NULL;
            break;
        }

        ZeroMemory( &stIn,  sizeof(APP_DRIVER_BUFFER_INFO) );
        ZeroMemory( &stOut, sizeof(APP_DRIVER_BUFFER_INFO) );
        m_nDeviceNum++;
		stIn.m_hEvent[0] = m_hPrvEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		stIn.m_hEvent[1] = m_hCompressEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        stIn.m_hEvent[2] = NULL; // CreateEvent(NULL, TRUE, FALSE, NULL); // stIn.m_hEvent[2] 该参数未启用，为它传递一个值，只为避开驱动层的参数检查
		stIn.m_hEvent[3] = m_hAudEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		stIn.m_hEvent[4] = NULL; //CreateEvent(NULL, TRUE, FALSE, NULL);
		BOOL bRtn = ControlDriver(
            i,
			IOCTL_INITIALIZE_DRV,
			&stIn,
			sizeof(APP_DRIVER_BUFFER_INFO),
			&stOut,
			sizeof(APP_DRIVER_BUFFER_INFO),
			&dwBytesReturned);
		if (bRtn == 0)
		{
			return FALSE;
		}
	}

	if (m_nDeviceNum <= 0)
	{
        return FALSE;
    }

    CFile file;
    if (!file.Open(s_pOutFileName[m_dwVideoFormat], CFile::modeRead))
    {
        return FALSE;
    }
    file.Close();

	
    CTiCoffFile *m_pTiCoffFile[MAX_DEVICE_NUM];
	for (i = 0; i < m_nDeviceNum; i++)
	{
		m_pTiCoffFile[i] = new CTiCoffFile(m_hDevice[i]);
        m_pTiCoffFile[i]->LoadCoffFile(s_pOutFileName[m_dwVideoFormat]);
	}

	for (i = 0; i < m_nDeviceNum; i++)
	{
		m_pTiCoffFile[i]->Wait();
		delete m_pTiCoffFile[i];
	}
	return TRUE;
}

void CDSP::DeviceExit()
{
	for (int i = 0; i < m_nDeviceNum; i++)	//zhangzhen	2007/03/01
	{
        safeCloseHandle(m_hDevice[i]);
        safeCloseHandle(m_hPrvEvent[i]);
        safeCloseHandle(m_hCompressEvent[i]);
        safeCloseHandle(m_hAudEvent[i]);
	}
}

BOOL CDSP::Initialize(DWORD dwVideoFormat, CAPTURECALLBACK *pVideoCallBack, CAPTURECALLBACK *pAudioCallBack)
{
	m_dwVideoFormat = dwVideoFormat;
	m_pVideoCallBack = pVideoCallBack;
	m_pAudioCallBack = pAudioCallBack;

	if (dwVideoFormat == DVRVIDEO_STANDARD_PAL)
	{
		m_dwPrvBufSize = PRV_BUF_SIZE_PALCIF;
		m_dwMotionSize = MOTION_LENGTH_PAL;
		m_dwWidth = 352;
		m_dwHeight = 288;
	}
	else
	{
		m_dwPrvBufSize = PRV_BUF_SIZE_NTSCCIF;
		m_dwMotionSize = MOTION_LENGTH_NTSC;
		m_dwWidth = 320;
		m_dwHeight = 240;
	}

	do 
	{
		if ( !DeviceInit() )
		{
			break;
		}

		if (CreateBuffer())
		{
			return TRUE;
		}

		DestroyBuffer();
	}
	while (0);

	DeviceExit();
	return FALSE;
}

void CDSP::Terminate()
{
	DestroyBuffer();
	DeviceExit();
}

DWORD CDSP::GetDevNum()
{
	return m_nDeviceNum;
}

BOOL CDSP::SetSwitch(DWORD *pSwitch, DWORD ChannelNum)
{
	BOOL	bSwitchChged = FALSE;
	ChannelNum = min(MAX_DEVICE_NUM * CHANNEL_PER_DEVICE, ChannelNum);
	for (DWORD i = 0; i < ChannelNum; i++)
	{
		bSwitchChged = bSwitchChged || (m_pSwitch[i] != pSwitch[i]);
		m_pSwitch[i] = pSwitch[i];
	}

	//通道开关改变，刷新网络压缩帧率	Add By zhangzhen 2007/10/11
	if(bSwitchChged)
	{
		RefreshNetFrameRate();
	}

	return TRUE;
}

union BufPara
{
    DWORD dwPara;
    struct PARA
    {
        short nIndex:8;
        short nDevice:8;
        short nType:16;
    } Para; 
};

BOOL CDSP::ReleaseBuffer(DWORD isVideo, DWORD DelBufPara)
{
	int nType = DelBufPara >> 16 & 0xFF;
	int nDevice = DelBufPara >> 8 & 0xFF;
	int nIndex = DelBufPara & 0xFF;
	
	if (isVideo != 0)
	{
		if (nType == VIDEO_STREAM_PREVIEW)
		{
			ReleasePrvBuf(nDevice, nIndex);
		}
		else if (nType == VIDEO_STREAM_CAPTURE)
		{
			ReleaseCapBuf(nDevice, nIndex);
		}
		else if (nType == VIDEO_STREAM_NET)
		{
			ReleaseNetBuf(nDevice, nIndex);
		}
		else if (nType == VIDEO_STREAM_MOBILE)
		{
			ReleaseMobileBuf(nDevice, nIndex);
		}
		else if (nType == VIDEO_STREAM_NET_REALTIME)	//<REC-NET>
		{
			ReleaseNetBuf_RT(nDevice, nIndex);
		}
	}
	else
	{
		ReleaseAudBuf(nDevice, nIndex);
	}

	return TRUE;
}

BOOL CDSP::CreateWorkerThread()
{
	m_bQuit = FALSE;  
    for ( int i=0; i< m_nDeviceNum; ++i )
    {
        m_hThreadPrv[i] = CreateThread(NULL, 0, OnThreadPrv, new ThreadParm(this, i), 0, NULL);
        m_hThreadCompressStrm[i] = CreateThread(NULL, 0, OnThreadCompressStrm, new ThreadParm(this, i), 0, NULL);
        m_hThreadAud[i] = CreateThread(NULL, 0, OnThreadAud, new ThreadParm(this, i), 0, NULL);
    }

	return TRUE;
}

void CDSP::DestroyWorkerThread()
{
	DWORD dwExitCode;
	m_bQuit = TRUE;
	for (int i = 0; i < m_nDeviceNum; i++)	//zhangzhen	2007/03/01
	{
		if (m_hThreadPrv[i] != NULL)
		{	
			SetEvent(m_hPrvEvent[i]);
			WaitForSingleObject(m_hThreadPrv[i], 2000);
			if (GetExitCodeThread(m_hThreadPrv[i], &dwExitCode))
			{
				if (dwExitCode == STILL_ACTIVE)
				{
					TRACE("TerminateThread m_hThreadPrv\n");
					TerminateThread(m_hThreadPrv[i], 1);
				}
			}
			CloseHandle(m_hThreadPrv[i]);
			m_hThreadPrv[i] = NULL;
		}

		if (m_hThreadAud[i] != NULL)
		{
			SetEvent(m_hAudEvent[i]);
			WaitForSingleObject(m_hThreadAud[i], 2000);
			if (GetExitCodeThread(m_hThreadAud[i], &dwExitCode))
			{
				if (dwExitCode == STILL_ACTIVE)
				{
					TRACE("TerminateThread m_hThreadAud\n");
					TerminateThread(m_hThreadAud[i], 1);
				}
			}
			CloseHandle(m_hThreadAud[i]);
			m_hThreadAud[i] = NULL;
		}

		if (m_hThreadCompressStrm[i] != NULL)
		{
			SetEvent(m_hCompressEvent[i]);
			WaitForSingleObject(m_hThreadCompressStrm[i], 2000);
			if (GetExitCodeThread(m_hThreadCompressStrm[i], &dwExitCode))
			{
				if (dwExitCode == STILL_ACTIVE)
				{
					TRACE("TerminateThread m_hThreadCompressStrm\n");
					TerminateThread(m_hThreadCompressStrm[i], 1);
				}
			}
			CloseHandle(m_hThreadCompressStrm[i]);
			m_hThreadCompressStrm[i] = NULL;
		}
	}
}

BOOL CDSP::CreateBuffer()
{
	int j;
	for (int i = 0; i < m_nDeviceNum; i++)
	{
		for (j = 0; j < AUD_BUF_NUM; j++)
		{
			m_pAudBuf[i][j].pBuf = new BYTE[AUD_BUF_SIZE];
			m_pAudBuf[i][j].nVLostFlag = 1;
		}

		for (j = 0; j < CAP_BUF_NUM; j++)
		{
			m_pCapBuf[i][j].pBuf = NULL;
			m_pCapBuf[i][j].nVLostFlag = 1;
		}

		for (j = 0; j < PRV_BUF_NUM; j++)
		{
#ifdef PRECOPY
            m_pPrvBuf[i][j].pBuf = new BYTE[CIF_BUFF_SIZE]; //heliang fix
            m_pDrvHeadOfPrvBuf[i][j] = new TVT_CAP_STATUS; //heliang+
#else
            m_pPrvBuf[i][j].pBuf = NULL;
            m_pDrvHeadOfPrvBuf[i][j] = NULL;
#endif // PRECOPY	
			m_pPrvBuf[i][j].nVLostFlag = 1;
            
		}

		for (j = 0; j < NET_BUF_NUM; j++)
		{
			m_pNetBuf[i][j].pBuf = NULL;
			m_pNetBuf[i][j].nVLostFlag = 1;
		}

		for (j = 0; j < MOBILE_BUF_NUM; j++)
		{
			m_pMobileBuf[i][j].pBuf = NULL;
			m_pMobileBuf[i][j].nVLostFlag = 1;
		}

		for (j = 0; j < CAP_BUF_NUM; j++)	//<REC-NET>实施网络流来源于录像流，BUF数量分配相同
		{
			m_pNetBuf_RT[i][j].pBuf = new BYTE[CAP_BUF_SIZE];	//<REC-NET>
			m_pNetBuf_RT[i][j].nVLostFlag = 1;
		}
	}

	return TRUE;
}

void CDSP::DestroyBuffer()
{
	int j;
	for (int i = 0; i < m_nDeviceNum; i++)
	{
		for (j = 0; j < AUD_BUF_NUM; j++)
		{
            safeDeleteArray(m_pAudBuf[i][j].pBuf);
		}

		//<REC-NET>
		for (j = 0; j < CAP_BUF_NUM; j++)
		{
             safeDeleteArray(m_pNetBuf_RT[i][j].pBuf);
		}

#ifdef PRECOPY 
        //heliang+
        for (j = 0; j < PRV_BUF_NUM; j++)
        {
            safeDeleteArray(m_pPrvBuf[i][j].pBuf);
            m_pPrvBuf[i][j].nVLostFlag = 1;
            safeDeleteArray(m_pDrvHeadOfPrvBuf[i][j]); 
        }
#endif
	}
}

DWORD WINAPI CDSP::OnThreadAud(PVOID pParam)
{
    ThreadParm* pThreadParm = (ThreadParm*)pParam;
	pThreadParm->pthis->ProcessAud(pThreadParm->dwDeviceID);
    delete pThreadParm;
	return 0;
}

DWORD WINAPI CDSP::OnThreadPrv(PVOID pParam)
{
	ThreadParm* pThreadParm = (ThreadParm*)pParam;
	pThreadParm->pthis->ProcessPrv(pThreadParm->dwDeviceID);
    delete pThreadParm;
	return 0;
}

DWORD WINAPI CDSP::OnThreadCompressStrm(PVOID pParam)
{
	ThreadParm* pThreadParm = (ThreadParm*)pParam;
	pThreadParm->pthis->ProcessCompressStreams(pThreadParm->dwDeviceID);
	delete pThreadParm;
    return 0;
}

void CDSP::ProcessPrv(INT nDevice)
{
	while (TRUE)
	{
		DWORD dwWait = WaitForSingleObject(m_hPrvEvent[nDevice], 5000);
		//确保终止线程前，把所有参数设置到DSP
		if(m_bQuit == TRUE)	//若该卡没有等待设置的参数，结束线程
		{
			if(m_pPack[nDevice].param.size() == 0)
			{
				break;
			}
		}

		switch(dwWait)
		{
		case WAIT_TIMEOUT:
			TRACE("Devide : %d ProcessPrv WAIT_TIMEOUT\n", nDevice);
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			GetPrvData(nDevice);
			break;
		default:
			break;
		}
	}
}

void CDSP::GetPrvData(int nDevice)
{
    StartStopWatch();

    static DWORD dwStreamType = STREAM_TYPE_PRV;
    
    PBYTE pData;
    PTVT_CAP_STATUS pStatus;
    
    DWORD dwReturn;
    while (TRUE)	//取出Driver层所有BUF，避免BUF阻塞
    {
        ControlDriver(
            nDevice,
            IOCTL_VIDEO_GET_DATA_INFO,
            &dwStreamType,
            sizeof(DWORD),
            &pData,
            sizeof(DWORD),
            &dwReturn);
        if(pData == NULL)//Driver层已没有可用BUF
        {
            break;
        }

        // 先处理智能的数据，因为live数据模式如果是copy模式，马上会释放所有引用
        DoIVData(nDevice, pData);

        pStatus = (PTVT_CAP_STATUS)pData;
        SetParamToDSP(nDevice);

        pStatus->dwReserve4 = 4;	//一次处理整块卡4通道数据
        for (int nChannel = 0; nChannel < CHANNEL_PER_DEVICE; nChannel++)
        {
            GetOneChannelPreData(nChannel, nDevice, pData, pStatus);
        }
    }
}

void CDSP::GetOneChannelPreData(
    int nChannel, 
    int nDevice,
    PBYTE pData,
    PTVT_CAP_STATUS pStatus )
{
    PTVT_PREV_VBI pVBI;

    pVBI = (PTVT_PREV_VBI)(pData + CAP_STATUS_SIZE + (PREV_VBI_SIZE + CIF_BUFF_SIZE + MOTION_STATUS) * nChannel);
    SetSignal(nDevice * CHANNEL_PER_DEVICE + nChannel, pVBI->videoLoss);

    int nIndex;
    // if channel is close or video loss or is valid
    // or not enough buffer
    if ( m_pSwitch[nDevice * CHANNEL_PER_DEVICE + nChannel] == 0 ||
        pVBI->videoLoss ||
        !pVBI->byInvalid ||
        !FindPrvBuf(nDevice, nIndex) )
    {
        ReleaseDriverBuffer(pStatus);
        return;
    }    

    //数据部分指针
    BYTE* pBuf = pData + CAP_STATUS_SIZE + (PREV_VBI_SIZE + CIF_BUFF_SIZE + MOTION_STATUS) * nChannel + PREV_VBI_SIZE;
#ifdef PRECOPY      // 使用copy一份      
    memcpy(m_pPrvBuf[nDevice][nIndex].pBuf, pBuf, CIF_BUFF_SIZE); 
    memcpy(m_pDrvHeadOfPrvBuf[nDevice][nIndex], pStatus, sizeof(TVT_CAP_STATUS));	//该缓冲对应的DRIVER层BUF头
#else              // 使用数据引用
    m_pPrvBuf[nDevice][nIndex].pBuf = pBuf;
    m_pDrvHeadOfPrvBuf[nDevice][nIndex] = pStatus;
#endif // PRECOPY
    m_pPrvBuf[nDevice][nIndex].ChannelIndex = nDevice * CHANNEL_PER_DEVICE + nChannel;
    m_pPrvBuf[nDevice][nIndex].BufLen = m_dwPrvBufSize;
    m_pPrvBuf[nDevice][nIndex].nStreamID = VIDEO_STREAM_PREVIEW;
    m_pPrvBuf[nDevice][nIndex].BufferPara = VIDEO_STREAM_PREVIEW << 16 | nDevice << 8 | nIndex;

    PrintFrameRate(nDevice * CHANNEL_PER_DEVICE + nChannel, VIDEO_STREAM_PREVIEW);
    m_pVideoCallBack(&m_pPrvBuf[nDevice][nIndex]);


#ifdef PRECOPY 
    ReleaseDriverBuffer(pStatus);
#endif
}
/*
功能说明 : 处理所有压缩流
			(取代原始ProcessNet和ProcessCap两个处理函数，及以后的手机流也在此处理)
参数说明 : nDevice[IN] : 4104卡编号
返 回 值 : 空
修改记录 :
			修改人			修改日期		修改内容
-----------------------------------------------------------------------------------
			zhangzhen		2007/01/20		创建(因为DSP程序处理过程修改，需要合并压缩流)
*/
void CDSP::ProcessCompressStreams(INT nDevice)
{
	DWORD dwWait;
	while (TRUE)
	{
		dwWait = WaitForSingleObject(m_hCompressEvent[nDevice], 5000);
		if (m_bQuit == TRUE)
			break;

		switch(dwWait)
		{
		case WAIT_TIMEOUT:
			TRACE("Devide : %d ProcessCompressStreams WAIT_TIMEOUT\n", nDevice);
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			GetCompressData(nDevice);
            break;
		default:
			break;
		}
	}
}

void CDSP::GetCompressData(int nDevice)
{ 
    StartStopWatch();

    DWORD dwReturn;
    PBYTE pData;
    PTVT_CAP_STATUS pStatus;
    PTVT_REC_VBI pVBI;
    long nFrameNum = 0;	//记录从DRIVER层一次取出的帧数量

    static DWORD dwStreamType = STREAM_TYPE_CAP;
    while (TRUE)	//取出Driver层所有BUF，避免BUF阻塞
    {
        ControlDriver(nDevice,
            IOCTL_VIDEO_GET_DATA_INFO,
            &dwStreamType,
            sizeof(DWORD),
            &pData,
            sizeof(DWORD),
            &dwReturn);
        if(pData == NULL)//Driver层已没有可用BUF
        {
            break;
        }

        pStatus = (PTVT_CAP_STATUS)pData;
        pData += sizeof(TVT_CAP_STATUS);

        //统计这次取出的BUF总数(宜在底层填充BUF时统计)
        nFrameNum = 0;

        PVOID pTemp = pData;
        //for ( pVBI = (TVT_REC_VBI*)pData;
        //      pVBI->byDataType != 0xff;   	//该数据包处理完
        //      pData += pVBI->dwLen )
        //{
        //    pData += sizeof(TVT_REC_VBI);
        //    if(pVBI->byInvalid != 0)	//非法数据不统计
        //    {
        //        nFrameNum++;
        //    }
        //}

        while(TRUE)
        {
            pVBI = (TVT_REC_VBI*)pData;
            if(pVBI->byDataType == 0xff)	//该数据包处理完
            {
                break;
            }

            pData += sizeof(TVT_REC_VBI);
            if(pVBI->byInvalid == 0)	//非法数据不统计
            {
                pData += pVBI->dwLen;	//
                continue;
            }

            nFrameNum++;
            pData += pVBI->dwLen;	//下一帧
        }

        if(nFrameNum <= 0)	//空BUF
        {
            pStatus->byLock = 0;	//解锁，取下一BUF
            continue;
        }

        //在BUF状态信息头中记录该BUF中总共包含的帧数，以备在BUF释放时作减一计数
        //待该计数器为0时，说明该BUF中所有帧均已处理完，最终byLock置0，释放DRIVER层BUF
        ::InterlockedExchange((PLONG)(&(pStatus->dwReserve4)), nFrameNum);

        pData = (PBYTE)pTemp;

        for(int i = 0; i < nFrameNum; i++)
        {
            pVBI = (TVT_REC_VBI*) pData;
            if(pVBI->byDataType == 0xff)	//该数据包处理完
            {
                break;
            }

            pData += sizeof(TVT_REC_VBI);
            if(pVBI->byInvalid == 0)	//非法数据不处理
            {
                pData += pVBI->dwLen;	//现在不会执行到此，保证逻辑结构完整
                continue;
            }

            switch (pVBI->byDataType)
            {
            case 0:  //录像流                         
                CmprssStreamProcessNet_RT(nDevice, pVBI, pData, pStatus);	//<REC-NET>

                //丢帧后该BUF不再使用，即当前由pStatus管理状态的BUF数量要减一
                if(CmprssStreamProcessRcd(nDevice, pVBI, pData, pStatus) == 0)
                {
                    ReleaseDriverBuffer(pStatus);
                }
                break;
            case 1:  //网络流
                //增加对丢帧的处理，丢帧后该BUF不再使用，即当前由pStatus管理状态的BUF数量要减一
                if(CmprssStreamProcessNet(nDevice, pVBI, pData, pStatus) == 0)
                {
                    ReleaseDriverBuffer(pStatus);
                }
                break;
            case 2:  //手机流
                //增加对丢帧的处理，丢帧后该BUF不再使用，即当前由pStatus管理状态的BUF数量要减一
                if(CmprssStreamProcessMobile(nDevice, pVBI, pData, pStatus) == 0)
                {
                    ReleaseDriverBuffer(pStatus);
                }
                break;
            default:
                //这是不合法的情况，如果执行到此，也做丢帧处理
                ReleaseDriverBuffer(pStatus);
                break;
            }

            pData += pVBI->dwLen;	//下一帧
        }	 // end for		    
    }  // end while
}

void CDSP::ProcessAud(INT nDevice)
{
	DWORD dwWait;
	DWORD dwStreamType;
	DWORD dwReturn;
	PBYTE pData;
	PBYTE pAudio;
	PTVT_AUD_VBI pVBI; 
	PTVT_CAP_STATUS pStatus;
	int nIndex;
	int nChannel;
	int nBufIndex;
	int i;
	UINT nLen;

	while (TRUE)
	{
		dwWait = WaitForSingleObject(m_hAudEvent[nDevice], 5000);
		if (m_bQuit == TRUE)
			break;

		switch(dwWait)
		{
		case WAIT_TIMEOUT:
			TRACE("Devide : %d ProcessAud WAIT_TIMEOUT\n", nDevice);
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			dwStreamType = STREAM_TYPE_AUD;

			while (TRUE)	//取出Driver层所有BUF，避免BUF阻塞
			{
				ControlDriver(nDevice,
							IOCTL_VIDEO_GET_DATA_INFO,
							&dwStreamType,
							sizeof(DWORD),
							&pData,
							sizeof(DWORD),
							&dwReturn);

				if(pData == NULL)//Driver层已没有可用BUF
				{
					break;
				}

				pStatus = (PTVT_CAP_STATUS)pData;
				pVBI = (PTVT_AUD_VBI)(pData + CAP_STATUS_SIZE);
				pAudio = pData + CAP_STATUS_SIZE+sizeof(TVT_AUD_VBI);

				for (nIndex = 0; nIndex < CHANNEL_PER_DEVICE; nIndex++)
				{
					//改通道号，左左右右改成左右左右
//#define CARD4104_BNC
#ifdef CARD4104_BNC	//4104 BNC接口，音频通道号映射纠正
					if (nIndex == 1)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 2;
					}
					else if (nIndex == 2)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 1;
					}
					else
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + nIndex;
					}
#else	//4104 D型接口，音频通道号映射纠正
					if (nIndex == 0)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 1;
					}
					else if (nIndex == 1)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 3;
					}
					else if (nIndex == 2)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 0;
					}
					else if (nIndex == 3)
					{
						nChannel = nDevice * CHANNEL_PER_DEVICE + 2;
					}
#endif

					if (FindAudBuf(nDevice, nBufIndex))
					{
						//nLen = min(pStatus->dwReserve4/8, AUD_BUF_SIZE);
						nLen = min(pVBI->dwLen/8, AUD_BUF_SIZE);
						for (i = 0; i < nLen; i++)
						{
							m_pAudBuf[nDevice][nBufIndex].pBuf[i] = (*((PWORD)(pAudio + i * 8 + nIndex * 2)) + 0x8000) >> 8; 
						}

						m_pAudBuf[nDevice][nBufIndex].ChannelIndex = nChannel;
						m_pAudBuf[nDevice][nBufIndex].BufLen = nLen;
						m_pAudBuf[nDevice][nBufIndex].nStreamID = VIDEO_STREAM_PREVIEW;
						m_pAudBuf[nDevice][nBufIndex].BufferPara = VIDEO_STREAM_PREVIEW << 16 | nDevice << 8 | nBufIndex;
						
						m_pAudioCallBack(&m_pAudBuf[nDevice][nBufIndex]);
					}
					else
					{
						TRACE("no audbuf\n");
					}
				}

				pStatus->byLock = 0;
			}
			break;
		default:
			break;
		}
	}
}

/*
功能说明 : 处理录像流数据帧
参数说明 : nDeviceNo[IN]--卡号
		   pVBI[IN]	------VBI信息指针
		   pVideoData[IN]-数据部分指针
		   pST[IN]--------指向该帧BUF区在DRIVER层所属的BUF头,以便释放DRIVER层的BUF
返 回 值 : 0--失败(该帧没有生效，即没有上传到AP)	1--成功上传到AP
*/
int CDSP::CmprssStreamProcessRcd(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//通道号
	int	nIndex = 0;	//选中缓冲区编号
	int	nLen = 0;	//视频数据长度

	//正在等待关键帧，但该帧不是关键帧，丢弃
	if(m_bNextFrameIsKeyRcd[nChl] && !pVBI->byKeyFrame)
	{
		return 0;
	}

    // if channel is close or Record is Stop, return
    // <REC-NET>	录像停止状态下，录像流停止上传
    if ( m_pSwitch[nChl] == 0 || m_bRecordStop[nChl] )
    {
        //关闭状态下，一直等待关键帧，但不能强制关键帧，因为录像流已经开放到网络，录像关闭时，网络要照常传输，如果强制关键帧，将造成码流大，且画质低	<REC-NET>
        m_bNextFrameIsKeyRcd[nChl] = TRUE;
        //SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
        return 0;
    }

    // buffer not enough, return
	if ( !FindCapBuf(nDeviceNo, nIndex) )
	{
        //BUF不足，丢帧，需要一个即时关键帧，并等待下一个关键帧
        SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
        m_bNextFrameIsKeyRcd[nChl] = TRUE;
        return 0;
    }

	nLen = min(pVBI->dwLen, CAP_BUF_SIZE);
	m_pCapBuf[nDeviceNo][nIndex].pBuf = pVideoData;
	m_pDrvHeadOfCapBuf[nDeviceNo][nIndex] = pST;	//记录该BUF在DRIVER层所属的缓冲头，以便释放DRIVER层BUF

	m_pCapBuf[nDeviceNo][nIndex].ChannelIndex = nChl;
	m_pCapBuf[nDeviceNo][nIndex].BufLen = nLen;
	m_pCapBuf[nDeviceNo][nIndex].nStreamID = VIDEO_STREAM_CAPTURE;
	m_pCapBuf[nDeviceNo][nIndex].BufferPara = VIDEO_STREAM_CAPTURE << 16 | nDeviceNo << 8 | nIndex;
	m_pCapBuf[nDeviceNo][nIndex].bIsKeyFrame = pVBI->byKeyFrame;
	m_pCapBuf[nDeviceNo][nIndex].localTime = ChangeTime(pVBI->recVideoTime);
	m_pCapBuf[nDeviceNo][nIndex].FrameTime = m_pCapBuf[nDeviceNo][nIndex].localTime;
	
    if ( m_pVideoCallBack(&m_pCapBuf[nDeviceNo][nIndex]) )
    {
        m_bNextFrameIsKeyRcd[nChl] = FALSE;
    }
    else
    {
        SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
        m_bNextFrameIsKeyRcd[nChl] = TRUE;
    }

	PrintFrameRate(nChl, VIDEO_STREAM_CAPTURE);

    // 统计两个I帧之间有多少个P帧
	++m_nFrameCntRcd[nChl];
	//超过100帧没有上传关键帧，强制DSP程序发送一个即时关键帧
	//但没有必要等待关键帧，因为没有丢帧
 /* if(m_nFrameCntRcd[nChl] > 100)
	{
		SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
	} */

	if(pVBI->byKeyFrame)
	{
		m_nFrameCntRcd[nChl] = 0;
	}

	return 1;
}

/*
功能说明 : 处理网络流数据帧
参数说明 : nDeviceNo[IN]--卡号
		   pVBI[IN]	------VBI信息指针
		   pVideoData[IN]-数据部分指针
		   pST[IN]--------指向该帧BUF区在DRIVER层所属的BUF头,以便释放DRIVER层的BUF
返 回 值 : 0--失败(该帧没有生效，即没有上传到AP)	1--成功上传到AP
*/
int CDSP::CmprssStreamProcessNet(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//通道号
	int	nIndex = 0;	//选中缓冲区编号
	int	nLen = 0;	//视频数据长度
	BOOL	bFrameAccept = FALSE;	//数据被AP接收标志

	WriteTestLog("chl = %d, key = %d, len = %d\n", nChl, pVBI->byKeyFrame, pVBI->dwLen);
	//正在等待关键帧，但该帧不是关键帧，丢弃
	if(m_bNextFrameIsKeyNet[nChl] && !pVBI->byKeyFrame)
	{
//		pVBI->byKeyFrame = 1;
		return 0;
	}

    // if channel is close or net is Stop or buffer not enough, 
    // Set next Frame must I Frame, return 0
    if ( m_pSwitch[nChl] == 0 || !m_bNetstart 
         || !FindNetBuf(nDeviceNo, nIndex) ) //<REC-NET>
    {
        m_bNextFrameIsKeyNet[nChl] = TRUE;
        SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);
        return 0;  
    }

	nLen = min(pVBI->dwLen, CAP_BUF_SIZE);
	m_pNetBuf[nDeviceNo][nIndex].pBuf = pVideoData;
	m_pDrvHeadOfNetBuf[nDeviceNo][nIndex] = pST;
	m_pNetBuf[nDeviceNo][nIndex].ChannelIndex = nChl;
	m_pNetBuf[nDeviceNo][nIndex].BufLen = nLen;
	m_pNetBuf[nDeviceNo][nIndex].nStreamID = VIDEO_STREAM_NET;
	m_pNetBuf[nDeviceNo][nIndex].BufferPara = VIDEO_STREAM_NET << 16 | nDeviceNo << 8 | nIndex;
	m_pNetBuf[nDeviceNo][nIndex].bIsKeyFrame = pVBI->byKeyFrame;
	m_pNetBuf[nDeviceNo][nIndex].localTime = ChangeTime(pVBI->recVideoTime);
	m_pNetBuf[nDeviceNo][nIndex].FrameTime = m_pNetBuf[nDeviceNo][nIndex].localTime;
	PrintFrameRate(nChl, VIDEO_STREAM_NET);
	bFrameAccept = m_pVideoCallBack(&m_pNetBuf[nDeviceNo][nIndex]);

	m_nFrameCntNet[nChl]++;

	//超过100帧没有上传关键帧，强制DSP程序发送一个即时关键帧
	//但没有必要等待关键帧，因为没有丢帧
    /*if(m_nFrameCntNet[nChl] > 100)
	{
		SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//
	}*/

	if(!bFrameAccept)
	{
		SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//
		m_bNextFrameIsKeyNet[nChl] = TRUE;
	}
    else
    {
        m_bNextFrameIsKeyNet[nChl] = FALSE;
    }

	if(pVBI->byKeyFrame)
	{
		m_nFrameCntNet[nChl] = 0;
	}

	return 1;
}


/*
功能说明 : 处理手机流数据帧
参数说明 : nDeviceNo[IN]--卡号
		   pVBI[IN]	------VBI信息指针
		   pVideoData[IN]-数据部分指针
		   pST[IN]--------指向该帧BUF区在DRIVER层所属的BUF头,以便释放DRIVER层的BUF
返 回 值 : 0--失败(该帧没有生效，即没有上传到AP)	1--成功上传到AP
修改记录 :
		修改人		修改日期		修改内容
--------------------------------------------------------------
		zhangzhen	2007/09/10		增加手机流
*/
int CDSP::CmprssStreamProcessMobile(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
//	return 0;
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//通道号
	int	nIndex = 0;	//选中缓冲区编号
	int	nLen = 0;	//视频数据长度
	BOOL	bFrameAccept = FALSE;	//数据被AP接收标志

	//正在等待关键帧，但该帧不是关键帧，丢弃
	if(m_bNextFrameIsKeyMobile[nChl] && !pVBI->byKeyFrame)
	{
		return 0;
	}
	
    if ( m_pSwitch[nChl] == 0 || 
         !m_bNetstart         ||
         FindMobileBuf(nDeviceNo, nIndex) )
    {
        m_bNextFrameIsKeyMobile[nChl] = TRUE;
        SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);	//<REC-NET>
        return 0;
    }

	nLen = min(pVBI->dwLen, CAP_BUF_SIZE);
	m_pMobileBuf[nDeviceNo][nIndex].pBuf = pVideoData;
	m_pDrvHeadOfMobileBuf[nDeviceNo][nIndex] = pST;
	m_pMobileBuf[nDeviceNo][nIndex].ChannelIndex = nChl;
	m_pMobileBuf[nDeviceNo][nIndex].BufLen = nLen;
	m_pMobileBuf[nDeviceNo][nIndex].nStreamID = VIDEO_STREAM_MOBILE;
	m_pMobileBuf[nDeviceNo][nIndex].BufferPara = VIDEO_STREAM_MOBILE << 16 | nDeviceNo << 8 | nIndex;
	m_pMobileBuf[nDeviceNo][nIndex].bIsKeyFrame = pVBI->byKeyFrame;
	m_pMobileBuf[nDeviceNo][nIndex].localTime = ChangeTime(pVBI->recVideoTime);
	m_pMobileBuf[nDeviceNo][nIndex].FrameTime = m_pMobileBuf[nDeviceNo][nIndex].localTime;
	bFrameAccept = m_pVideoCallBack(&m_pMobileBuf[nDeviceNo][nIndex]);
	
    m_nFrameCntMobile[nChl]++;
	//超过100帧没有上传关键帧，强制DSP程序发送一个即时关键帧
	//但没有必要等待关键帧，因为没有丢帧
	/*if(m_nFrameCntMobile[nChl] > 100)
	{
		SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);
	}*/

	if(!bFrameAccept)
	{
		SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);
		m_bNextFrameIsKeyMobile[nChl] = TRUE;
	}
    else
    {
        m_bNextFrameIsKeyMobile[nChl] = FALSE;
    }

	if(pVBI->byKeyFrame)
	{
		m_nFrameCntMobile[nChl] = 0;
	}

	return 1;
}

/******************************************************************************
* 函数名称： BOOL CmprssStreamProcessNet_RT
* 功能描述： 处理网络流数据帧
* 输入参数： nDeviceNo	卡号
*		     pVBI VBI	信息指针
*		     pVideoData	数据部分指针
*		     pST		指向该帧BUF区在DRIVER层所属的BUF头,以便释放DRIVER层的BUF
* 输出参数： 无
* 返 回 值： 0	失败(该帧没有生效，即没有上传到AP)	
*            1	成功上传到AP
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	      修改内容
* --------------------------------------------------------------------------
* 2007/12/25           zhangzhen		<REC-NET>   
******************************************************************************/
int CDSP::CmprssStreamProcessNet_RT(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
	int nChl= nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//通道号
	int	nIndex = 0;	//选中缓冲区编号
	int	nLen = 0;	//视频数据长度

	//正在等待关键帧，但该帧不是关键帧，丢弃
	if(m_bNextFrameIsKeyNet_RT[nChl] && !pVBI->byKeyFrame)
	{
		return FALSE;
	}

    // if channel is close or net is stop, return
    if ( m_pSwitch[nChl] == 0 || !m_bNetstart )
    {
        m_bNextFrameIsKeyNet_RT[nChl] = TRUE;	//通道关闭，等待关键帧<REC-NET>	
        return FALSE;
    }

    // if find buf failed(not enough buffer), return
	if ( !FindNetBuf_RT(nDeviceNo, nIndex) )
	{
        //BUF不足，丢帧，需要一个即时关键帧，并等待下一个关键帧
        m_bNextFrameIsKeyNet_RT[nChl] = TRUE;
        return FALSE;
    }

	nLen = min(pVBI->dwLen, CAP_BUF_SIZE);
	memcpy(m_pNetBuf_RT[nDeviceNo][nIndex].pBuf, pVideoData, nLen);

	m_pNetBuf_RT[nDeviceNo][nIndex].ChannelIndex = nChl;
	m_pNetBuf_RT[nDeviceNo][nIndex].BufLen = nLen;
	m_pNetBuf_RT[nDeviceNo][nIndex].nStreamID = VIDEO_STREAM_NET_REALTIME;
	m_pNetBuf_RT[nDeviceNo][nIndex].BufferPara = VIDEO_STREAM_NET_REALTIME << 16 | nDeviceNo << 8 | nIndex;
	m_pNetBuf_RT[nDeviceNo][nIndex].bIsKeyFrame = pVBI->byKeyFrame;

	m_pNetBuf_RT[nDeviceNo][nIndex].localTime = ChangeTime(pVBI->recVideoTime);

	m_pNetBuf_RT[nDeviceNo][nIndex].FrameTime = m_pNetBuf_RT[nDeviceNo][nIndex].localTime;
	PrintFrameRate(nChl, VIDEO_STREAM_NET);//, nLen*8);

    // App accept failed, need next frame must I Frame
	if(!m_pVideoCallBack(&m_pNetBuf_RT[nDeviceNo][nIndex]))
	{
		m_bNextFrameIsKeyNet_RT[nChl] = TRUE;
	}
    else
    {
        m_bNextFrameIsKeyNet_RT[nChl] = FALSE;
    }
	
	return TRUE;
}

BOOL CDSP::CaptureStop()
{
	APP_DRIVER_BUFFER_INFO stIn;
	DWORD dwRtn;
	DestroyWorkerThread();

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	for (int i = 0; i < m_nDeviceNum; i++)
	{
		BOOL bRtn = ControlDriver(i,
				IOCTL_SHOW_STREAM_END,
				&stIn,
				sizeof(APP_DRIVER_BUFFER_INFO),
				NULL,
				0,
				&dwRtn);
		if (bRtn == 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDSP::CaptureStart()
{
    int i;
	APP_DRIVER_BUFFER_INFO stIn;
	DWORD dwRtn;
	CreateWorkerThread();

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	for (i = 0; i < m_nDeviceNum; i++)
	{
		BOOL bRtn = ControlDriver(i,
				IOCTL_SHOW_STREAM_START,
				&stIn,
				sizeof(APP_DRIVER_BUFFER_INFO),
				NULL,
				0,
				&dwRtn);
		if (bRtn == 0)
		{
			return FALSE;
		}
	}

	for (i = 0; i < CHANNEL_PER_DEVICE * m_nDeviceNum; i++)
	{
		//网络压缩流
		SetParam(PT_PCI_SET_NET_ENCODE_ISENCODE, i, 1);
		SetParam(PT_PCI_SET_NET_ENCODE_BITRATE, i, 500000);
		SetParam(PT_PCI_SET_NET_ENCODE_IINTERVAL, i, 1);
		//手机压缩流
		SetParam(PT_PCI_SET_MOB_ENCODE_ISENCODE, i, 1);
		SetParam(PT_PCI_SET_MOB_ENCODE_BITRATE, i, 500000);
		SetParam(PT_PCI_SET_MOB_ENCODE_IINTERVAL, i, 1);
		//现场压缩流
		SetParam(PT_PCI_SET_ENCODE_IINTERVAL, i, 100);
	}
	return TRUE;
}

BOOL CDSP::ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned)
{
	if (m_hDevice[nIndex] != NULL)
	{
		return DeviceIoControl(
            m_hDevice[nIndex], 
			dwIoControlCode, 
			lpInBuffer, 
			nInBufferSize,
			lpOutBuffer, 
			nOutBufferSize, 
			lpBytesReturned, 
			NULL);
	}

	return FALSE;
}

BOOL CDSP::SetParam(int nType, int nChannel, int nValue)
{
	int nDevice = nChannel/4;
	if (nDevice >= m_nDeviceNum)
		return FALSE;

    PARAMPACK pack;
	pack.chanNum = nChannel%4;
	pack.paramType = nType;
	pack.value = nValue;

    BOOL bNew = TRUE;
    AutoLockAndUnlock(m_pPack[nDevice].CS);
	for ( deque<PARAMPACK>::iterator itParam = m_pPack[nDevice].param.begin();
          itParam != m_pPack[nDevice].param.end();
          itParam++)
	{
		if ( itParam->chanNum == pack.chanNum &&
             itParam->paramType == pack.paramType)
		{
			itParam->value = pack.value;
			bNew = FALSE;
			break;
		}
	}

	if (bNew)
	{
		m_pPack[nDevice].param.push_back(pack);
	}

	return TRUE;
}

BOOL CDSP::SetParamToDSP(INT nDevice)
{
	PPARAMPACK pPack;
	PDWORD pNum;
	DWORD dwPackSize;
	DWORD dwRtn;

	TVT_AP_SET stApSet; //heliang fix

	//判断dsp程序有没有提取上次设定的参数。
	stApSet.dwAddress = PCI_VIDEO_MEMADDR_SIZE;
	ControlDriver(nDevice,
			IOCTL_GET_DSP_PARAM,
			&stApSet,
			sizeof(stApSet),
			&stApSet,
			sizeof(stApSet),
			&dwRtn);
	if(stApSet.dwValue != 0)	//如果没有则取消当次设定。
	{
		return TRUE;
	}

	m_pPack[nDevice].CS.Lock();

	dwPackSize = m_pPack[nDevice].param.size();
	if (dwPackSize != 0)
	{
        BYTE szParamData[MAX_PARAMDATA_SIZE] = {0};
		pNum = (PDWORD)&szParamData[nDevice];
		pPack = (PPARAMPACK)(&szParamData[nDevice] + sizeof(DWORD));

		if (dwPackSize > 30)
			dwPackSize = 30;
		*pNum = dwPackSize;
		
		for (int i = 0; i < dwPackSize; i++)
		{
			pPack[i] = m_pPack[nDevice].param.front();
			m_pPack[nDevice].param.pop_front();
		}

		ControlDriver(nDevice,
				IOCTL_SET_DSP_MULT_PARAM,
				szParamData,
				MAX_PARAMDATA_SIZE,
				NULL,
				0,
				&dwRtn);
	}

	m_pPack[nDevice].CS.Unlock();

	return TRUE;
}

void CDSP::CheckVideoLoss(DWORD *pStatus, DWORD dwLen)
{
	dwLen = min(MAX_CHANNEL_NUM, dwLen);
	for (int nChannel = 0; nChannel < dwLen; nChannel++)
	{
		m_pSignal[nChannel].CS.Lock();
		pStatus[nChannel] = !m_pSignal[nChannel].bSignal;
		m_pSignal[nChannel].CS.Unlock();
	}
}

void CDSP::SetSignal(int nChannel, UINT iS)
{
	BOOL	bStatusChged = FALSE;
	m_pSignal[nChannel].CS.Lock();
	bStatusChged = (m_pSignal[nChannel].bSignal != iS);
	m_pSignal[nChannel].bSignal = iS;

	//通道信号状态改变，刷新网络压缩帧率	Add By zhangzhen 2007/10/11
	if(bStatusChged)
	{
		RefreshNetFrameRate();
	}
	m_pSignal[nChannel].CS.Unlock();
}

// 用等于重载就好， /。。
ULONGLONG CDSP::ChangeTime(const SYSTIME &time)
{
	ULONGLONG ullTime;
	SYSTEMTIME sysTime;

	sysTime.wYear = time.dwYear;
	sysTime.wMonth = time.byMonth;
	sysTime.wDay = time.byDay;
	sysTime.wHour = time.byHour;
	sysTime.wMinute = time.byMinute;
	sysTime.wSecond = time.bySecond;
	sysTime.wMilliseconds = time.wMilliSecond;
	sysTime.wDayOfWeek = 0;

	BOOL b = SystemTimeToFileTime(&sysTime, (FILETIME *)&ullTime);
	return ullTime;
}

// 这三个函数可以用一个模板函数搞定（或许Define），等我先将锁改为智能锁，先实现算法优化
BOOL CDSP::FindNetBuf(INT nDevice, INT &nIndex)
{
    // 因为找下一个可用的buf索引一般都为下一个，所以在这做算法优化，下同
	return FindTBuf<NET_BUF_NUM>(nDevice, nIndex,m_pNetBufCS,m_pNetBuf);
}

//<REC-NET>
BOOL CDSP::FindNetBuf_RT(INT nDevice, INT &nIndex)
{
    return FindTBuf<CAP_BUF_NUM>(nDevice, nIndex,m_pNetBufCS_RT,m_pNetBuf_RT);
}

BOOL CDSP::FindMobileBuf(INT nDevice, INT &nIndex)
{
    return FindTBuf<MOBILE_BUF_NUM>(nDevice, nIndex,m_pMobileBufCS,m_pMobileBuf);
}

BOOL CDSP::FindAudBuf(INT nDevice, INT &nIndex)
{
    return FindTBuf<AUD_BUF_NUM>(nDevice, nIndex,m_pAudBufCS,m_pAudBuf);
}

BOOL CDSP::FindCapBuf(INT nDevice, INT &nIndex)
{
    return FindTBuf<CAP_BUF_NUM>(nDevice, nIndex,m_pCapBufCS,m_pCapBuf);
}

BOOL CDSP::FindPrvBuf(INT nDevice, INT &nIndex)
{
    return FindTBuf<PRV_BUF_NUM>(nDevice, nIndex,m_pPrvBufCS,m_pPrvBuf);
}

void CDSP::ReleaseAudBuf(INT nDevice, INT nIndex)
{
    AutoLockAndUnlock(m_pAudBufCS[nDevice]);
    FRAMEBUFSTRUCT& frame = m_pAudBuf[nDevice][nIndex];
    if ( frame.nVLostFlag == 0)
    {
        m_pAudBuf[nDevice][nIndex].nVLostFlag = 1;
    }
    else
    {
        // TRACE  ERROR []
    }
}

void CDSP::ReleaseCapBuf(INT nDevice, INT nIndex)
{
    AutoLockAndUnlock(m_pCapBufCS[nDevice]);
    if(m_pCapBuf[nDevice][nIndex].nVLostFlag == 0)
    {
        ReleaseDriverBuffer(m_pDrvHeadOfCapBuf[nDevice][nIndex]);//zhangzhen 2007/02/09
        m_pDrvHeadOfCapBuf[nDevice][nIndex] = NULL;
        m_pCapBuf[nDevice][nIndex].pBuf = NULL;
        m_pCapBuf[nDevice][nIndex].nVLostFlag = 1;
    }
}

void CDSP::ReleasePrvBuf(INT nDevice, INT nIndex)
{
    AutoLockAndUnlock(m_pPrvBufCS[nDevice]);
    if(m_pPrvBuf[nDevice][nIndex].nVLostFlag == 0)
    {
#ifdef PRECOPY 
#else
        ReleaseDriverBuffer(m_pDrvHeadOfPrvBuf[nDevice][nIndex]);//zhangzhen 2007/02/09
        m_pDrvHeadOfPrvBuf[nDevice][nIndex] = NULL;
        m_pPrvBuf[nDevice][nIndex].pBuf = NULL;
#endif
        m_pPrvBuf[nDevice][nIndex].nVLostFlag = 1;
    }
}

void CDSP::ReleaseNetBuf(INT nDevice, INT nIndex)
{
	AutoLockAndUnlock(m_pNetBufCS[nDevice]);
	if(m_pNetBuf[nDevice][nIndex].nVLostFlag == 0)
	{
		ReleaseDriverBuffer(m_pDrvHeadOfNetBuf[nDevice][nIndex]);//zhangzhen 2007/02/09
		m_pDrvHeadOfNetBuf[nDevice][nIndex] = NULL;
		m_pNetBuf[nDevice][nIndex].pBuf = NULL;
		m_pNetBuf[nDevice][nIndex].nVLostFlag = 1;
	}
}

//<REC-NET>
void CDSP::ReleaseNetBuf_RT(INT nDevice, INT nIndex)
{
	AutoLockAndUnlock(m_pNetBufCS_RT[nDevice]);
	if(m_pNetBuf_RT[nDevice][nIndex].nVLostFlag == 0)
	{
		m_pNetBuf_RT[nDevice][nIndex].nVLostFlag = 1;
	}
    else
    {
        // TRACE  ERROR []
    }
}

void CDSP::ReleaseMobileBuf(INT nDevice, INT nIndex)
{
	AutoLockAndUnlock(m_pMobileBufCS[nDevice]);
	if(m_pMobileBuf[nDevice][nIndex].nVLostFlag == 0)
	{
		ReleaseDriverBuffer(m_pDrvHeadOfMobileBuf[nDevice][nIndex]);//zhangzhen 2007/02/09
		m_pDrvHeadOfMobileBuf[nDevice][nIndex] = NULL;
		m_pMobileBuf[nDevice][nIndex].pBuf = NULL;
		m_pMobileBuf[nDevice][nIndex].nVLostFlag = 1;
	}
}

BOOL CDSP::Password()
{
	BOOL EncryptStatus = TRUE;
	BYTE random = 0;
	BYTE ReadEncryptBuf = 0;
	DWORD dwtime = 0;

	dwtime = (unsigned)time(NULL);
	srand(dwtime);

	for (int j = 0; j < m_nDeviceNum; j++)
	{
		for (BYTE i = 0; i <= 10; i++)
		{	
			random = rand();
			random %= 128;
			
			ReadEncryptBuf = Encrypt(j, random);
			
			int tmp = (BYTE)(abs((int)(sin((double)random) * 255)) + 81);
			
			if(ReadEncryptBuf != tmp) 
			{
				EncryptStatus = FALSE;
				break;
			}
		}
	}

	TRACE("Password : %d\n", EncryptStatus);
	return TRUE;//EncryptStatus;
}

BYTE CDSP::Encrypt(INT nDevice, BYTE byRandom)
{
	DWORD dwRtn;
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	stIn.m_ulRegValue = byRandom;
	
	ControlDriver(nDevice,
		IOCTL_GET_DEVICE_PASSWORD,
		&stIn,
		sizeof(APP_DRIVER_BUFFER_INFO),
		&stOut,
		sizeof(APP_DRIVER_BUFFER_INFO),
		&dwRtn);

	return stOut.m_ulRegValue;
}

BOOL CDSP::SetAudioSwitch(DWORD dwChannel, LONG lStatus)
{
	if (dwChannel >= MAX_CHANNEL_NUM)
	{
		return FALSE;
	}

	m_pAudioSwitch[dwChannel] = lStatus;

	return TRUE;
}

/*
函数功能 : 刷新网络帧率
参数说明 : 无
返 回 值 : 未定义，返回0
修改记录 :
		修改人		修改日期		修改内容
----------------------------------------------------------
		zhangzhen	2007/10/11		创建
*/
DWORD CDSP::RefreshNetFrameRate()
{
	int	i = 0, j = 0;
	int	nChlNum = 0;	//有效通道计数器
	int	nchl = 0;	//通道号
	int	nFrameRate = 0;	//理论帧率
	int	nMaxFrameRate = 0;

	for(i = 0; i < m_nDeviceNum; i++)
	{
		if(m_bNetstart)
		{
			//计算帧率
			nChlNum = 0;
			for(j = 0; j < CHANNEL_PER_DEVICE; j++)
			{
				nchl = i*CHANNEL_PER_DEVICE+j;
				if((m_dwNetChannelMask & (1<<(nchl)))	//网络通道连接
					&& m_pSwitch[nchl]			//通道本身打开
					&& !m_pSignal[nchl].bSignal)			//通道有信号
				{
					nChlNum++;	//有效通道计数器+1
				}
			}

			//设定所有打开通道的帧率
			if(nChlNum != 0)
			{
				nMaxFrameRate = NET_FRAME_RATE_TOTAL / nChlNum;
				nFrameRate = (nMaxFrameRate>>1) + m_nNetFrameRateInc;
			}

			if(nFrameRate < 1)
			{
				nFrameRate = 1;
			}
			if(nFrameRate > nMaxFrameRate)
			{
				nFrameRate = nMaxFrameRate;
			}

			for(j = 0; j < CHANNEL_PER_DEVICE; j++)
			{
				nchl = i*CHANNEL_PER_DEVICE+j;
				if((m_dwNetChannelMask & (1<<(nchl)))	//网络通道连接
					&& m_pSwitch[nchl]			//通道本身打开
					&& !m_pSignal[nchl].bSignal)			//通道有信号
				{
					//设定网络帧率
					if(nFrameRate > 25)
					{
						nFrameRate = 25;
					}
					SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, nFrameRate);
					SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 1);
				}
				else	//无效通道关闭
				{
					//设定网络帧率
					SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, 0);
					SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 0);
				}
			}
		}
		else
		{
			//设定所有通道帧率为0
			SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, 0);
			SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 0);
		}
	}

	return 0;
}

/*
函数功能 : 提升网络帧率
参数说明 : inc[IN]--网络帧率增量
返 回 值 : 未定义
修改记录 : 
		修改人		修改日期		修改内容
--------------------------------------------------------
		张振		2007/10/17		创建
*/
DWORD CDSP::NetFrameRateInc(int inc)
{
	m_nNetFrameRateInc += inc;	//帧率增量累加

	if(m_nNetFrameRateInc > (NET_FRAME_RATE_TOTAL>>1))
	{
		m_nNetFrameRateInc = (NET_FRAME_RATE_TOTAL>>1);
	}

	if(m_nNetFrameRateInc < -(NET_FRAME_RATE_TOTAL>>1))
	{
		m_nNetFrameRateInc = -(NET_FRAME_RATE_TOTAL>>1);
	}

	return RefreshNetFrameRate();
}

// end of file  old -> 2114