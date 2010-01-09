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

//<----cs����:���߳����룻�����̵߳���ʱ(����ָ����ͬ)���ڴ��л��̣߳������̷ֱ߳�ִ�м�һ������
//dwReserve4==0����ʱ�����̻߳�ֱ�byLock��0�����ܵ�����������ڵ�һ����0ʱ����BUF���±��ͷ�

	if((pST)->dwReserve4 == 0)
	{
		(pST)->byLock = 0;
	}
	cs.Unlock();
}
#if 0
/*
�������� : ת��֡ͷ4BYTE��ʽ
����˵�� :
p4B[IN/OUT] : Ҫת���4�ֽڵ�ַ
bDirection[IN]: ת�䷽��(TRUE: ���->�ؼ�֡��ʾ��FALSE: �ؼ�֡��ʾ->���)
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
{
	VariableInit();
}

CDSP::~CDSP()
{

}

#define PCI_DEVICE_STRING "\\\\.\\DmtiD"

BOOL CDSP::DeviceInit()
{
	int i;
	char szName[20];
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

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
            break;
        }

		m_nDeviceNum++;
		stIn.m_hEvent[0] = m_hPrvEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		stIn.m_hEvent[1] = stIn.m_hEvent[2] =  // stIn.m_hEvent[2] �ò���δ���ã�Ϊ������һ��ֵ��ֻΪ�ܿ�������Ĳ������
            m_hCompressEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		stIn.m_hEvent[3] = m_hAudEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
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
    else
    {
        file.Close();
    }
	
    CTiCoffFile *m_pTiCoffFile[MAX_DEVICE_NUM];
	for (i = 0; i < m_nDeviceNum; i++)
	{
		m_pTiCoffFile[i] = new CTiCoffFile(m_hDevice[i]);
		if (DVRVIDEO_STANDARD_PAL == m_dwVideoFormat)
			m_pTiCoffFile[i]->LoadCoffFile(OUT_FILE_NAME_PAL);
		else
			m_pTiCoffFile[i]->LoadCoffFile(OUT_FILE_NAME_NTSC);
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
	for (int i = 0; i < /*MAX_DEVICE_NUM*/m_nDeviceNum; i++)	//zhangzhen	2007/03/01
	{
		if (m_hDevice[i] != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hDevice[i]);
			m_hDevice[i] = INVALID_HANDLE_VALUE;
		}

		if (m_hPrvEvent[i] != NULL)
		{
			CloseHandle(m_hPrvEvent[i]);
			m_hPrvEvent[i] = NULL;
		}

		if (m_hCompressEvent[i] != NULL)
		{
			CloseHandle(m_hCompressEvent[i]);
			m_hCompressEvent[i] = NULL;
		}

		if (m_hAudEvent[i] != NULL)
		{
			CloseHandle(m_hAudEvent[i]);
			m_hAudEvent[i] = NULL;
		}
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

	//ͨ�����ظı䣬ˢ������ѹ��֡��	Add By zhangzhen 2007/10/11
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
        ThreadParm* pParm = new ThreadParm(this, i);
        m_hThreadPrv[i] = CreateThread(NULL, 0, OnThreadPrv, pParm, 0, NULL);
        m_hThreadCompressStrm[i] = CreateThread(NULL, 0, OnThreadCompressStrm, pParm, 0, NULL);
        m_hThreadAud[i] = CreateThread(NULL, 0, OnThreadAud, pParm, 0, NULL);
    }

	return TRUE;
}

void CDSP::DestroyWorkerThread()
{
	int i;
	DWORD dwExitCode;

	m_bQuit = TRUE;

	for (i = 0; i < /*MAX_DEVICE_NUM*/m_nDeviceNum; i++)	//zhangzhen	2007/03/01
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
	int i;
	int j;

	for (i = 0; i < m_nDeviceNum; i++)
	{
		m_pParamData[i] = new BYTE[MAX_PARAMDATA_SIZE];
		if (m_pParamData[i] == NULL)
		{
//			AfxMessageBox("create m_pParamData error");
			return FALSE;
		}

		for (j = 0; j < AUD_BUF_NUM; j++)
		{
			m_pAudBuf[i][j].pBuf = new BYTE[AUD_BUF_SIZE];
			if (m_pAudBuf[i][j].pBuf == NULL)
			{
//				AfxMessageBox("create m_pAudBuf error");
				return FALSE;
			}
			else
			{
				m_pAudBuf[i][j].nVLostFlag = 1;
			}
		}

		for (j = 0; j < CAP_BUF_NUM; j++)
		{
			m_pCapBuf[i][j].pBuf = NULL;
			m_pCapBuf[i][j].nVLostFlag = 1;
		}

		for (j = 0; j < PRV_BUF_NUM; j++)
		{
			m_pPrvBuf[i][j].pBuf = NULL;
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

		for (j = 0; j < CAP_BUF_NUM; j++)	//<REC-NET>ʵʩ��������Դ��¼������BUF����������ͬ
		{
			m_pNetBuf_RT[i][j].pBuf = new BYTE[CAP_BUF_SIZE];	//<REC-NET>
			m_pNetBuf_RT[i][j].nVLostFlag = 1;
		}
	}

	return TRUE;
}

void CDSP::DestroyBuffer()
{
	int i;
	int j;

	for (i = 0; i < /*MAX_DEVICE_NUM*/m_nDeviceNum; i++)	//zhangzhen	2007/03/01
	{
		if (m_pParamData[i] != NULL)
		{
			delete [] m_pParamData[i];
			m_pParamData[i] = NULL;
		}

		for (j = 0; j < AUD_BUF_NUM; j++)
		{
			if (m_pAudBuf[i][j].pBuf != NULL)
			{
				delete [] m_pAudBuf[i][j].pBuf;
				m_pAudBuf[i][j].pBuf = NULL;
			}
		}

		//<REC-NET>
		for (j = 0; j < CAP_BUF_NUM; j++)
		{
			if (m_pNetBuf_RT[i][j].pBuf != NULL)
			{
				delete [] m_pNetBuf_RT[i][j].pBuf;
				m_pNetBuf_RT[i][j].pBuf = NULL;
			}
		}
	}
}

void CDSP::VariableInit()
{
	int i;
	int j;

	m_bNetstart = FALSE;
	m_bQuit = FALSE;
	m_dwPrvBufSize = 0;

	m_pVideoCallBack = NULL;
	m_pAudioCallBack = NULL;
	m_nDeviceNum = 0;

	m_dwVideoFormat = DVRVIDEO_STANDARD_PAL;

	for (i = 0; i < MAX_CHANNEL_NUM; i++)
	{
		m_pSignal[i].bSignal = 1;
	}

	for (i = 0; i < MAX_CHANNEL_NUM; i++)
	{
		m_ftNetLastFrameTime[i] = m_CounterTime.GetCurrentTime();
		*(ULONGLONG *)&m_ftNetLastFrameTime[i] += i * 80 * 10000;//���ȷ���
		m_pSwitch[i] = 0;
		m_pAudioSwitch[i] = 1;
	}

	for (i = 0; i < MAX_DEVICE_NUM; i++)
	{
		m_pParamData[i] = NULL;

		m_hDevice[i] = INVALID_HANDLE_VALUE;

		m_hAudEvent[i] = NULL;
		m_hCompressEvent[i] = NULL;	//zhangzhen	2007/02/28
		m_hPrvEvent[i] = NULL;

		m_hThreadAud[i] = NULL;
		m_hThreadPrv[i] = NULL;
		m_hThreadCompressStrm[i] = NULL;	//zhangzhen	2007/02/28

		for (j = 0; j < AUD_BUF_NUM; j++)
		{
			memset(&m_pAudBuf[i][j], 0, sizeof(FRAMEBUFSTRUCT));
		}

		for (j = 0; j < CAP_BUF_NUM; j++)
		{
			memset(&m_pCapBuf[i][j], 0, sizeof(FRAMEBUFSTRUCT));
			m_pDrvHeadOfCapBuf[i][j] = NULL;
		}

		for (j = 0; j < PRV_BUF_NUM; j++)
		{
			memset(&m_pPrvBuf[i][j], 0, sizeof(FRAMEBUFSTRUCT));
		}

		for (j = 0; j < NET_BUF_NUM; j++)
		{
			memset(&m_pNetBuf[i][j], 0, sizeof(FRAMEBUFSTRUCT));
			m_pDrvHeadOfNetBuf[i][j] = NULL;
		}

		for (j = 0; j < MOBILE_BUF_NUM; j++)
		{
			memset(&m_pMobileBuf[i][j], 0, sizeof(FRAMEBUFSTRUCT));
			m_pDrvHeadOfMobileBuf[i][j] = NULL;
		}
		
		//<REC-NET>
		for (j = 0; j < CAP_BUF_NUM; j++)
		{
			memset(&m_pNetBuf_RT[i][j], 0, sizeof(FRAMEBUFSTRUCT));
		}
	}

	//ѹ�����ؼ�֡���Ʊ����������ⶪ֡/�������쳣(���ڲ��͹ؼ�֡)
	for(i = 0; i < MAX_CHANNEL_NUM; i++)
	{
		m_bNextFrameIsKeyRcd[i]	= TRUE;	//zhangzhen	2007/03/01
		m_bNextFrameIsKeyNet[i]	= TRUE;	//zhangzhen	2007/03/01
		m_bNextFrameIsKeyMobile[i]	= TRUE;	//zhangzhen	2007/03/01
		m_nFrameCntRcd[i]	= 0;
		m_nFrameCntNet[i]	= 0;
		m_nFrameCntMobile[i]	= 0;
		m_bNextFrameIsKeyNet_RT[i]	= TRUE;	//<REC-NET>
		m_bRecordStop[i] = TRUE;	//<REC-NET>��ʼ״̬������ͨ��¼��ȫ���ر�
	}

	//������ѹ������	Add By zhangzhen 2007/10/11
	m_dwNetChannelMask = 0;
	//��ʼ����֡������Ϊ0	Add By zhangzhen 2007/10/17
	m_nNetFrameRateInc = 0;
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
	DWORD dwWait;
	DWORD dwStreamType;
	DWORD dwReturn;
	PBYTE pData;
	PTVT_CAP_STATUS pStatus;
	PTVT_PREV_VBI pVBI;
	int nChannel;
	int nIndex;

	while (TRUE)
	{
		dwWait = WaitForSingleObject(m_hPrvEvent[nDevice], 5000);

		//ȷ����ֹ�߳�ǰ�������в������õ�DSP
		if(m_bQuit == TRUE)	//���ÿ�û�еȴ����õĲ����������߳�
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
			break;
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			dwStreamType = STREAM_TYPE_PRV;

			while (TRUE)	//ȡ��Driver������BUF������BUF����
			{
				ControlDriver(nDevice,
							IOCTL_VIDEO_GET_DATA_INFO,
							&dwStreamType,
							sizeof(DWORD),
							&pData,
							sizeof(DWORD),
							&dwReturn);

				if(pData == NULL)//Driver����û�п���BUF
				{
					break;
				}
				
				pStatus = (PTVT_CAP_STATUS)pData;

				SetParamToDSP(nDevice);

				pStatus->dwReserve4 = 4;	//һ�δ������鿨4ͨ������

				for (nChannel = 0; nChannel < CHANNEL_PER_DEVICE; nChannel++)
				{
					pVBI = (PTVT_PREV_VBI)(pData + CAP_STATUS_SIZE + (PREV_VBI_SIZE + CIF_BUFF_SIZE + MOTION_STATUS) * nChannel);
					SetSignal(nDevice * CHANNEL_PER_DEVICE + nChannel, pVBI->videoLoss);

					if (m_pSwitch[nDevice * CHANNEL_PER_DEVICE + nChannel] == 1 && !pVBI->videoLoss && pVBI->byInvalid)
					{
						if (FindPrvBuf(nDevice, nIndex))
						{
							//���ݲ���ָ��
							m_pPrvBuf[nDevice][nIndex].pBuf = pData + CAP_STATUS_SIZE + (PREV_VBI_SIZE + CIF_BUFF_SIZE + MOTION_STATUS) * nChannel + PREV_VBI_SIZE;
							m_pDrvHeadOfPrvBuf[nDevice][nIndex] = pStatus;	//�û����Ӧ��DRIVER��BUFͷ

							m_pPrvBuf[nDevice][nIndex].ChannelIndex = nDevice * CHANNEL_PER_DEVICE + nChannel;
							m_pPrvBuf[nDevice][nIndex].BufLen = m_dwPrvBufSize;
							m_pPrvBuf[nDevice][nIndex].nStreamID = VIDEO_STREAM_PREVIEW;
							m_pPrvBuf[nDevice][nIndex].BufferPara = VIDEO_STREAM_PREVIEW << 16 | nDevice << 8 | nIndex;

							PrintFrameRate(nDevice * CHANNEL_PER_DEVICE + nChannel, VIDEO_STREAM_PREVIEW);
							m_pVideoCallBack(&m_pPrvBuf[nDevice][nIndex]);
						}
						else
						{
							ReleaseDriverBuffer(pStatus);
							TRACE("no prvbuf\n");
						}
					}
					else
					{
						ReleaseDriverBuffer(pStatus);
					}
				}
/*//zhangzhen 2007/02/09
				while(pStatus->dwReserve4 != 0)	//�ȴ�������ȫ��������
				{
					if(m_bQuit)
					{
						break;
					}

					Sleep(1);
				}

				pStatus->byLock = 0;
*/			}
/*//zhangzhen 2007/02/09
			if(!m_bQuit)
			{
				ResetEvent(m_hPrvEvent[nDevice]);
			}
*/
			break;
		default:
			break;
		}
	}
}

/*
����˵�� : ��������ѹ����
			(ȡ��ԭʼProcessNet��ProcessCap���������������Ժ���ֻ���Ҳ�ڴ˴���)
����˵�� : nDevice[IN] : 4104�����
�� �� ֵ : ��
�޸ļ�¼ :
			�޸���			�޸�����		�޸�����
-----------------------------------------------------------------------------------
			zhangzhen		2007/01/20		����(��ΪDSP����������޸ģ���Ҫ�ϲ�ѹ����)
*/
void CDSP::ProcessCompressStreams(INT nDevice)
{
	DWORD dwWait;
	DWORD dwStreamType;
	DWORD dwReturn;
	PBYTE pData;
	PTVT_CAP_STATUS pStatus;
	PTVT_REC_VBI pVBI;

	long	nFrameNum = 0;	//��¼��DRIVER��һ��ȡ����֡����
	PVOID	pTemp = NULL;	//��ʱָ��

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
			break;
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			dwStreamType = STREAM_TYPE_CAP;

			while (TRUE)	//ȡ��Driver������BUF������BUF����
			{
				ControlDriver(nDevice,
							IOCTL_VIDEO_GET_DATA_INFO,
							&dwStreamType,
							sizeof(DWORD),
							&pData,
							sizeof(DWORD),
							&dwReturn);

				if(pData == NULL)//Driver����û�п���BUF
				{
					break;
				}
			
				pStatus = (PTVT_CAP_STATUS)pData;
				pData += sizeof(TVT_CAP_STATUS);
				
				//ͳ�����ȡ����BUF����(���ڵײ����BUFʱͳ��)
				nFrameNum = 0;

				pTemp = pData;
				while(TRUE)
				{
					pVBI = (TVT_REC_VBI*) pData;

					if(pVBI->byDataType == 0xff)	//�����ݰ�������
					{
						break;
					}

					pData += sizeof(TVT_REC_VBI);

					if(pVBI->byInvalid == 0)	//�Ƿ����ݲ�ͳ��
					{
						pData += pVBI->dwLen;	//
						continue;
					}

					nFrameNum++;

					pData += pVBI->dwLen;	//��һ֡
				}

				if(nFrameNum <= 0)	//��BUF
				{
					pStatus->byLock = 0;	//������ȡ��һBUF
					continue;
				}

				//��BUF״̬��Ϣͷ�м�¼��BUF���ܹ�������֡�����Ա���BUF�ͷ�ʱ����һ����
				//���ü�����Ϊ0ʱ��˵����BUF������֡���Ѵ����꣬����byLock��0���ͷ�DRIVER��BUF
				::InterlockedExchange((PLONG)(&(pStatus->dwReserve4)), nFrameNum);

				pData = (PBYTE)pTemp;
				
				for(int i = 0; i < nFrameNum; i++)
				{
					pVBI = (TVT_REC_VBI*) pData;

					if(pVBI->byDataType == 0xff)	//�����ݰ�������
					{
						break;
					}

					pData += sizeof(TVT_REC_VBI);

					if(pVBI->byInvalid == 0)	//�Ƿ����ݲ�����
					{
						pData += pVBI->dwLen;	//���ڲ���ִ�е��ˣ���֤�߼��ṹ����
						continue;
					}

					if(pVBI->byDataType == 0)	//¼����
					{
						CmprssStreamProcessNet_RT(nDevice, pVBI, pData, pStatus);	//<REC-NET>

						//��֡���BUF����ʹ�ã�����ǰ��pStatus����״̬��BUF����Ҫ��һ
						if(CmprssStreamProcessRcd(nDevice, pVBI, pData, pStatus) == 0)
						{
							ReleaseDriverBuffer(pStatus);
						}
					}
					else if(pVBI->byDataType == 1)	//������
					{
						//���ӶԶ�֡�Ĵ�����֡���BUF����ʹ�ã�����ǰ��pStatus����״̬��BUF����Ҫ��һ
						if(CmprssStreamProcessNet(nDevice, pVBI, pData, pStatus) == 0)
						{
							ReleaseDriverBuffer(pStatus);
						}
					}
					else if(pVBI->byDataType == 2)	//�ֻ���
					{
						//���ӶԶ�֡�Ĵ�����֡���BUF����ʹ�ã�����ǰ��pStatus����״̬��BUF����Ҫ��һ
						if(CmprssStreamProcessMobile(nDevice, pVBI, pData, pStatus) == 0)
						{
							ReleaseDriverBuffer(pStatus);
						}
					}
					else
					{
						//���ǲ��Ϸ�����������ִ�е��ˣ�Ҳ����֡����
						ReleaseDriverBuffer(pStatus);
					}
	
					pData += pVBI->dwLen;	//��һ֡
				}

//zhangzhen 2007/02/09
				//�ȴ�ȡ��������BUF�������꣬Ȼ��ȡ����һ��
/*				while(pStatus->dwReserve4 != 0)
				{
					if(m_bQuit)
					{
						break;
					}
					Sleep(1);
				}

				pStatus->byLock = 0;
*/			}

//zhangzhen 2007/02/09
/*			if(!m_bQuit)
			{
				ResetEvent(m_hCompressEvent[nDevice]);
			}
*/
			break;
		default:
			break;
		}
	}
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
			break;
		case WAIT_FAILED:
			break;		
		case WAIT_OBJECT_0:
			dwStreamType = STREAM_TYPE_AUD;

			while (TRUE)	//ȡ��Driver������BUF������BUF����
			{
				ControlDriver(nDevice,
							IOCTL_VIDEO_GET_DATA_INFO,
							&dwStreamType,
							sizeof(DWORD),
							&pData,
							sizeof(DWORD),
							&dwReturn);

				if(pData == NULL)//Driver����û�п���BUF
				{
					break;
				}

				pStatus = (PTVT_CAP_STATUS)pData;
				pVBI = (PTVT_AUD_VBI)(pData + CAP_STATUS_SIZE);
				pAudio = pData + CAP_STATUS_SIZE+sizeof(TVT_AUD_VBI);

				for (nIndex = 0; nIndex < CHANNEL_PER_DEVICE; nIndex++)
				{
					//��ͨ���ţ��������Ҹĳ���������
//#define CARD4104_BNC
#ifdef CARD4104_BNC	//4104 BNC�ӿڣ���Ƶͨ����ӳ�����
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
#else	//4104 D�ͽӿڣ���Ƶͨ����ӳ�����
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
/*//zhangzhen 2007/02/09
			if(!m_bQuit)
			{
				ResetEvent(m_hAudEvent[nDevice]);
			}
*/
			break;
		default:
			break;
		}
	}
}

/*
����˵�� : ����¼��������֡
����˵�� : nDeviceNo[IN]--����
		   pVBI[IN]	------VBI��Ϣָ��
		   pVideoData[IN]-���ݲ���ָ��
		   pST[IN]--------ָ���֡BUF����DRIVER��������BUFͷ,�Ա��ͷ�DRIVER���BUF
�� �� ֵ : 0--ʧ��(��֡û����Ч����û���ϴ���AP)	1--�ɹ��ϴ���AP
*/
int CDSP::CmprssStreamProcessRcd(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//ͨ����
	int	nIndex = 0;	//ѡ�л��������
	int	nLen = 0;	//��Ƶ���ݳ���
	BOOL	bFrameAccept = FALSE;	//���ݱ�AP���ձ�־

	//���ڵȴ��ؼ�֡������֡���ǹؼ�֡������
	if(m_bNextFrameIsKeyRcd[nChl] && !pVBI->byKeyFrame)
	{
		return 0;
	}

	m_bNextFrameIsKeyRcd[nChl] = FALSE;

	if (m_pSwitch[nChl] == 1 && !m_bRecordStop[nChl])	//<REC-NET>	¼��ֹͣ״̬�£�¼����ֹͣ�ϴ�
	{
		if (FindCapBuf(nDeviceNo, nIndex))
		{
			nLen = min(pVBI->dwLen, CAP_BUF_SIZE);

			m_pCapBuf[nDeviceNo][nIndex].pBuf = pVideoData;

			m_pDrvHeadOfCapBuf[nDeviceNo][nIndex] = pST;	//��¼��BUF��DRIVER�������Ļ���ͷ���Ա��ͷ�DRIVER��BUF

			m_pCapBuf[nDeviceNo][nIndex].ChannelIndex = nChl;
			m_pCapBuf[nDeviceNo][nIndex].BufLen = nLen;
			m_pCapBuf[nDeviceNo][nIndex].nStreamID = VIDEO_STREAM_CAPTURE;
			m_pCapBuf[nDeviceNo][nIndex].BufferPara = VIDEO_STREAM_CAPTURE << 16 | nDeviceNo << 8 | nIndex;
			m_pCapBuf[nDeviceNo][nIndex].bIsKeyFrame = pVBI->byKeyFrame;

			m_pCapBuf[nDeviceNo][nIndex].localTime = ChangeTime(pVBI->recVideoTime);

			m_pCapBuf[nDeviceNo][nIndex].FrameTime = m_pCapBuf[nDeviceNo][nIndex].localTime;
			bFrameAccept = m_pVideoCallBack(&m_pCapBuf[nDeviceNo][nIndex]);

			PrintFrameRate(nChl, VIDEO_STREAM_CAPTURE);

			m_nFrameCntRcd[nChl]++;

			//����100֡û���ϴ��ؼ�֡��ǿ��DSP������һ����ʱ�ؼ�֡
			//��û�б�Ҫ�ȴ��ؼ�֡����Ϊû�ж�֡
/*			if(m_nFrameCntRcd[nChl] > 100)
			{
				SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
			}
*/
			if(!bFrameAccept)
			{
				SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
				m_bNextFrameIsKeyRcd[nChl] = TRUE;
			}

			if(pVBI->byKeyFrame)
			{
				m_nFrameCntRcd[nChl] = 0;
			}
		}
		else
		{
			//BUF���㣬��֡����Ҫһ����ʱ�ؼ�֡�����ȴ���һ���ؼ�֡
			SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
			m_bNextFrameIsKeyRcd[nChl] = TRUE;
			TRACE("no capbuf\n");

			return 0;
		}
	}
	else
	{
		//�ر�״̬�£�һֱ�ȴ��ؼ�֡��������ǿ�ƹؼ�֡����Ϊ¼�����Ѿ����ŵ����磬¼��ر�ʱ������Ҫ�ճ����䣬���ǿ�ƹؼ�֡��������������һ��ʵ�	<REC-NET>
		m_bNextFrameIsKeyRcd[nChl] = TRUE;
//		SetParam(PT_PCI_SET_IMMED_IFRAME, nChl, 1);
		return 0;
	}

	return 1;
}

/*
����˵�� : ��������������֡
����˵�� : nDeviceNo[IN]--����
		   pVBI[IN]	------VBI��Ϣָ��
		   pVideoData[IN]-���ݲ���ָ��
		   pST[IN]--------ָ���֡BUF����DRIVER��������BUFͷ,�Ա��ͷ�DRIVER���BUF
�� �� ֵ : 0--ʧ��(��֡û����Ч����û���ϴ���AP)	1--�ɹ��ϴ���AP
*/
int CDSP::CmprssStreamProcessNet(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//ͨ����
	int	nIndex = 0;	//ѡ�л��������
	int	nLen = 0;	//��Ƶ���ݳ���
	BOOL	bFrameAccept = FALSE;	//���ݱ�AP���ձ�־

	WriteTestLog("chl = %d, key = %d, len = %d\n", nChl, pVBI->byKeyFrame, pVBI->dwLen);
	//���ڵȴ��ؼ�֡������֡���ǹؼ�֡������
	if(m_bNextFrameIsKeyNet[nChl] && !pVBI->byKeyFrame)
	{
//		pVBI->byKeyFrame = 1;
		return 0;
	}

#if 0
//*	�ظ�֡׷��
	static	DWORD dwLastNo = 0;

	if(((*(DWORD*)pVideoData)<<8) == (dwLastNo<<8))
	{
		::WriteTestLog("0x%x", *(DWORD*)pVideoData);
	}
	dwLastNo = *(DWORD*)pVideoData;

	FrameHead_4B_Convert((DWORD*)pVideoData, TRUE, (DWORD)'TVTX');
//*/
#endif

	m_bNextFrameIsKeyNet[nChl] = FALSE;

//<REC-NET>	if (m_pSwitch[nChl] == 1 && m_bNetstart == TRUE)
	if (m_pSwitch[nChl] == 1 && m_bNetstart == TRUE/* && (m_dwNetChannelMask & (1<<nChl))*/)	//<REC-NET>
	{
		if (FindNetBuf(nDeviceNo, nIndex))
		{
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

			//����100֡û���ϴ��ؼ�֡��ǿ��DSP������һ����ʱ�ؼ�֡
			//��û�б�Ҫ�ȴ��ؼ�֡����Ϊû�ж�֡
/*			if(m_nFrameCntNet[nChl] > 100)
			{
				SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//
			}
*/
			if(!bFrameAccept)
			{
				SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//
				m_bNextFrameIsKeyNet[nChl] = TRUE;
			}

			if(pVBI->byKeyFrame)
			{
				m_nFrameCntNet[nChl] = 0;
			}
		}
		else
		{
			//BUF���㣬��֡����Ҫһ����ʱ�ؼ�֡�����ȴ���һ���ؼ�֡
			SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//
			m_bNextFrameIsKeyNet[nChl] = TRUE;
			TRACE("no netbuf\n");

			return 0;
		}
	}
	else
	{
		m_bNextFrameIsKeyNet[nChl] = TRUE;
		SetParam(PT_PCI_SET_NET_IMMED_IFRAME, nChl, 1);//ͨ���رգ��ȴ��ؼ�֡��û��Ҫǿ�ƹؼ�֡<REC-NET>	

		return 0;
	}

	return 1;
}


/*
����˵�� : �����ֻ�������֡
����˵�� : nDeviceNo[IN]--����
		   pVBI[IN]	------VBI��Ϣָ��
		   pVideoData[IN]-���ݲ���ָ��
		   pST[IN]--------ָ���֡BUF����DRIVER��������BUFͷ,�Ա��ͷ�DRIVER���BUF
�� �� ֵ : 0--ʧ��(��֡û����Ч����û���ϴ���AP)	1--�ɹ��ϴ���AP
�޸ļ�¼ :
		�޸���		�޸�����		�޸�����
--------------------------------------------------------------
		zhangzhen	2007/09/10		�����ֻ���
*/
int CDSP::CmprssStreamProcessMobile(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
//	return 0;
	int nChl = nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//ͨ����
	int	nIndex = 0;	//ѡ�л��������
	int	nLen = 0;	//��Ƶ���ݳ���
	BOOL	bFrameAccept = FALSE;	//���ݱ�AP���ձ�־

	//���ڵȴ��ؼ�֡������֡���ǹؼ�֡������
	if(m_bNextFrameIsKeyMobile[nChl] && !pVBI->byKeyFrame)
	{
//		pVBI->byKeyFrame = 1;
		return 0;
	}
	
//	WriteTestLog("Mobile Enter\n");

	m_bNextFrameIsKeyMobile[nChl] = FALSE;

	if (m_pSwitch[nChl] == 1 && m_bNetstart == TRUE)
	{
		if (FindMobileBuf(nDeviceNo, nIndex))
		{
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

//			PrintFrameRate(nChl, VIDEO_STREAM_CAPTURE);

			m_nFrameCntMobile[nChl]++;

			//����100֡û���ϴ��ؼ�֡��ǿ��DSP������һ����ʱ�ؼ�֡
			//��û�б�Ҫ�ȴ��ؼ�֡����Ϊû�ж�֡
/*
			if(m_nFrameCntMobile[nChl] > 100)
			{
				SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);
			}
*/
			if(!bFrameAccept)
			{
				SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);
				m_bNextFrameIsKeyMobile[nChl] = TRUE;
			}

			if(pVBI->byKeyFrame)
			{
				m_nFrameCntMobile[nChl] = 0;
			}
		}
		else
		{
			//BUF���㣬��֡����Ҫһ����ʱ�ؼ�֡�����ȴ���һ���ؼ�֡
			SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);
			m_bNextFrameIsKeyMobile[nChl] = TRUE;
			TRACE("no Mobilebuf\n");
//	WriteTestLog("Mobile No Buf\n");

			return 0;
		}
	}
	else
	{
		m_bNextFrameIsKeyMobile[nChl] = TRUE;
		SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, nChl, 1);	//<REC-NET>
//	WriteTestLog("Mobile not start\n");

		return 0;
	}

	return 1;
}

/******************************************************************************
* �������ƣ� BOOL CmprssStreamProcessNet_RT(int nDeviceNo, const PTVT_REC_VBI const pVBI, 
					const PBYTE const pVideoData, PTVT_CAP_STATUS const pST)
* ���������� ��������������֡
* ��������� nDeviceNo	����
*		     pVBI VBI	��Ϣָ��
*		     pVideoData	���ݲ���ָ��
*		     pST		ָ���֡BUF����DRIVER��������BUFͷ,�Ա��ͷ�DRIVER���BUF
* ��������� ��
* �� �� ֵ�� 0	ʧ��(��֡û����Ч����û���ϴ���AP)	
*            1	�ɹ��ϴ���AP
* ��������   �汾��    ������ 
* �޸�����   �汾��    �޸���	      �޸�����
* --------------------------------------------------------------------------
* 2007/12/25           zhangzhen		<REC-NET>   
******************************************************************************/
int CDSP::CmprssStreamProcessNet_RT(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST)
{	
	int nChl= nDeviceNo * CHANNEL_PER_DEVICE + pVBI->byChannel;	//ͨ����
	int	nIndex = 0;	//ѡ�л��������
	int	nLen = 0;	//��Ƶ���ݳ���
	BOOL	bFrameAccept = FALSE;	//���ݱ�AP���ձ�־

	//���ڵȴ��ؼ�֡������֡���ǹؼ�֡������
	if(m_bNextFrameIsKeyNet_RT[nChl] && !pVBI->byKeyFrame)
	{
		return FALSE;
	}

	m_bNextFrameIsKeyNet_RT[nChl] = FALSE;

	if (m_pSwitch[nChl] == 1 && m_bNetstart == TRUE)
	{
		if (FindNetBuf_RT(nDeviceNo, nIndex))
		{
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

			bFrameAccept = m_pVideoCallBack(&m_pNetBuf_RT[nDeviceNo][nIndex]);

			if(!bFrameAccept)
			{
				m_bNextFrameIsKeyNet_RT[nChl] = TRUE;
			}
		}
		else
		{
			//BUF���㣬��֡����Ҫһ����ʱ�ؼ�֡�����ȴ���һ���ؼ�֡
			m_bNextFrameIsKeyNet_RT[nChl] = TRUE;
			WriteTestLog("C:\\Net_RT.txt", "no netbuf\n");

			return FALSE;
		}
	}
	else
	{
		m_bNextFrameIsKeyNet_RT[nChl] = TRUE;	//ͨ���رգ��ȴ��ؼ�֡<REC-NET>	
		WriteTestLog("C:\\Net_RT.txt", "net closed\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CDSP::CaptureStop()
{
	int i;
	APP_DRIVER_BUFFER_INFO stIn;
	DWORD dwRtn;
	BOOL bRtn;

	DestroyWorkerThread();

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	for (i = 0; i < m_nDeviceNum; i++)
	{
		bRtn = ControlDriver(i,
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
	BOOL bRtn;

	CreateWorkerThread();

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	for (i = 0; i < m_nDeviceNum; i++)
	{
		bRtn = ControlDriver(i,
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
		//����ѹ����
		SetParam(PT_PCI_SET_NET_ENCODE_ISENCODE, i, 1);
		SetParam(PT_PCI_SET_NET_ENCODE_BITRATE, i, 500000);
		SetParam(PT_PCI_SET_NET_ENCODE_IINTERVAL, i, 1);
		//�ֻ�ѹ����
		SetParam(PT_PCI_SET_MOB_ENCODE_ISENCODE, i, 1);
		SetParam(PT_PCI_SET_MOB_ENCODE_BITRATE, i, 500000);
		SetParam(PT_PCI_SET_MOB_ENCODE_IINTERVAL, i, 1);
		//�ֳ�ѹ����
		SetParam(PT_PCI_SET_ENCODE_IINTERVAL, i, 100);
	}
	return TRUE;
}

BOOL CDSP::ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned)
{
	if (m_hDevice[nIndex] != NULL)
	{
		BOOL bRtn = DeviceIoControl(m_hDevice[nIndex], 
					dwIoControlCode, 
					lpInBuffer, 
					nInBufferSize,
					lpOutBuffer, 
					nOutBufferSize, 
					lpBytesReturned, 
					NULL);
		return bRtn;
	}

	return FALSE;
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
	m_pAudBuf[nDevice][nIndex].nVLostFlag = 1;
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
		ReleaseDriverBuffer(m_pDrvHeadOfPrvBuf[nDevice][nIndex]);//zhangzhen 2007/02/09
		m_pDrvHeadOfPrvBuf[nDevice][nIndex] = NULL;
		m_pPrvBuf[nDevice][nIndex].pBuf = NULL;
		m_pPrvBuf[nDevice][nIndex].nVLostFlag = 1;
	}
}

BOOL CDSP::SetParam(int nType, int nChannel, int nValue)
{
	PARAMPACK pack;
	deque <PARAMPACK>::iterator itParam;
	BOOL bNew = TRUE;
	int nDevice = nChannel/4;

	if (nDevice >= m_nDeviceNum)
		return FALSE;

	pack.chanNum = nChannel%4;
	pack.paramType = nType;
	pack.value = nValue;

	m_pPack[nDevice].CS.Lock();	
	for (itParam = m_pPack[nDevice].param.begin(); itParam != m_pPack[nDevice].param.end(); itParam++)
	{
		if (itParam->chanNum == pack.chanNum && itParam->paramType == pack.paramType)
		{
			itParam->value = pack.value;
			bNew = FALSE;
			break;
		}
	}

	if (bNew == TRUE)
	{
		m_pPack[nDevice].param.push_back(pack);
	}
	m_pPack[nDevice].CS.Unlock();

	return TRUE;
}

BOOL CDSP::SetParamToDSP(INT nDevice)
{
	PPARAMPACK pPack;
	PDWORD pNum;
	DWORD dwPackSize;
	DWORD dwRtn;
	int i;

	_TVT_AP_SET stApSet;

	//�ж�dsp������û����ȡ�ϴ��趨�Ĳ�����
	stApSet.dwAddress = PCI_VIDEO_MEMADDR_SIZE;
	ControlDriver(nDevice,
			IOCTL_GET_DSP_PARAM,
			&stApSet,
			sizeof(stApSet),
			&stApSet,
			sizeof(stApSet),
			&dwRtn);

	if(stApSet.dwValue != 0)	//���û����ȡ�������趨��
	{
		return TRUE;
	}

	m_pPack[nDevice].CS.Lock();

	dwPackSize = m_pPack[nDevice].param.size();
	if (dwPackSize != 0)
	{
		memset(m_pParamData[nDevice], 0, MAX_PARAMDATA_SIZE);

		pNum = (PDWORD)m_pParamData[nDevice];
		pPack = (PPARAMPACK)(m_pParamData[nDevice] + sizeof(DWORD));

		if (dwPackSize > 30)
			dwPackSize = 30;

		*pNum = dwPackSize;
		
		for (i = 0; i < dwPackSize; i++)
		{
			pPack[i] = m_pPack[nDevice].param.front();
			m_pPack[nDevice].param.pop_front();
		}

		ControlDriver(nDevice,
				IOCTL_SET_DSP_MULT_PARAM,
				m_pParamData[nDevice],
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

	//ͨ���ź�״̬�ı䣬ˢ������ѹ��֡��	Add By zhangzhen 2007/10/11
	if(bStatusChged)
	{
		RefreshNetFrameRate();
	}
	m_pSignal[nChannel].CS.Unlock();
}

// �õ������ؾͺã� /����
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

// ����������������һ��ģ�庯���㶨������Define���������Ƚ�����Ϊ����������ʵ���㷨�Ż�
BOOL CDSP::FindNetBuf(INT nDevice, INT &nIndex)
{
    // ��Ϊ����һ�����õ�buf����һ�㶼Ϊ��һ���������������㷨�Ż�����ͬ
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

void CDSP::ByteSwapCopy(void *pDst, void *pSrc, unsigned int nLen)
{
	_asm{
		mov edi,pDst;
		mov esi,pSrc;
		mov ecx,nLen;
		add ecx,3;
		and ecx,0xffffffc;
		add ecx,pDst;
SWAP_COPY_LOOP:
		mov eax,[esi];
		bswap eax;
		mov [edi],eax;
		add edi,4;
		add esi,4;
		cmp edi,ecx;
		jne SWAP_COPY_LOOP;
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
�������� : ˢ������֡��
����˵�� : ��
�� �� ֵ : δ���壬����0
�޸ļ�¼ :
		�޸���		�޸�����		�޸�����
----------------------------------------------------------
		zhangzhen	2007/10/11		����
*/
DWORD CDSP::RefreshNetFrameRate()
{
	int	i = 0, j = 0;
	int	nChlNum = 0;	//��Чͨ��������
	int	nchl = 0;	//ͨ����
	int	nFrameRate = 0;	//����֡��
	int	nMaxFrameRate = 0;

	for(i = 0; i < m_nDeviceNum; i++)
	{
		if(m_bNetstart)
		{
			//����֡��
			nChlNum = 0;
			for(j = 0; j < CHANNEL_PER_DEVICE; j++)
			{
				nchl = i*CHANNEL_PER_DEVICE+j;
				if((m_dwNetChannelMask & (1<<(nchl)))	//����ͨ������
					&& m_pSwitch[nchl]			//ͨ�������
					&& !m_pSignal[nchl].bSignal)			//ͨ�����ź�
				{
					nChlNum++;	//��Чͨ��������+1
				}
			}

			//�趨���д�ͨ����֡��
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
				if((m_dwNetChannelMask & (1<<(nchl)))	//����ͨ������
					&& m_pSwitch[nchl]			//ͨ�������
					&& !m_pSignal[nchl].bSignal)			//ͨ�����ź�
				{
					//�趨����֡��
					if(nFrameRate > 25)
					{
						nFrameRate = 25;
					}
					SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, nFrameRate);
					SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 1);
				}
				else	//��Чͨ���ر�
				{
					//�趨����֡��
					SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, 0);
					SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 0);
				}
			}
		}
		else
		{
			//�趨����ͨ��֡��Ϊ0
			SetParam(PT_PCI_SET_NET_ENCODE_FRAMERATE, nchl, 0);
			SetParam(PT_PCI_SET_MOB_ENCODE_FRAMERATE, nchl, 0);
		}
	}

	return 0;
}

/*
�������� : ��������֡��
����˵�� : inc[IN]--����֡������
�� �� ֵ : δ����
�޸ļ�¼ : 
		�޸���		�޸�����		�޸�����
--------------------------------------------------------
		����		2007/10/17		����
*/
DWORD CDSP::NetFrameRateInc(int inc)
{
	m_nNetFrameRateInc += inc;	//֡�������ۼ�

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