/*H***************************************************************************
 File            : Preview.cpp
 Subsystem       : 
 Function Name(s): CPreview
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 14:07
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "Preview.h"

int CFunctionBase::m_nDeviceNum = 0;
DWORD CFunctionBase::m_pSwitch[MAX_CHANNEL_NUM] = {0};
HANDLE CFunctionBase::m_hDevice[MAX_DEVICE_NUM] = {0};
PACK CFunctionBase::m_pPack[MAX_DEVICE_NUM];
SIGNAL CFunctionBase::m_pSignal[MAX_CHANNEL_NUM];
BOOL CFunctionBase::m_bNetstart;    // [] int value
DWORD CFunctionBase::m_dwNetChannelMask;
int CFunctionBase::m_nNetFrameRateInc;
BOOL CFunctionBase::m_bQuit = FALSE;

#define NET_FRAME_RATE_TOTAL 32

//extern int m_nDeviceNum;


void CFunctionBase::SetSignal(int nChannel, UINT iS)
{
    BOOL bStatusChged = FALSE;
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

BOOL CFunctionBase::SetParamToDSP(int nDevice)
{
    PPARAMPACK pPack;
    PDWORD pNum;
    DWORD dwPackSize;
    DWORD dwRtn;
    int i;

    TVT_AP_SET stApSet; //heliang fix

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
        BYTE szParamData[MAX_PARAMDATA_SIZE] = {0};
        //memset(m_pParamData[nDevice], 0, MAX_PARAMDATA_SIZE);

        pNum = (PDWORD)szParamData[nDevice];
        pPack = (PPARAMPACK)(szParamData[nDevice] + sizeof(DWORD));

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
            szParamData,
            MAX_PARAMDATA_SIZE,
            NULL,
            0,
            &dwRtn);
    }

    m_pPack[nDevice].CS.Unlock();

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
DWORD CFunctionBase::RefreshNetFrameRate()
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


BOOL CFunctionBase::SetParam(int nType, int nChannel, int nValue)
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
        if ( itParam->chanNum == pack.chanNum &&
            itParam->paramType == pack.paramType)
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


CPreview::CPreview(void)
{
    ZeroMemory(m_pPrvBuf, sizeof(m_pPrvBuf));
    ZeroMemory(m_hPrvEvent, sizeof(m_hPrvEvent));
    ZeroMemory(m_hThreadPrv, sizeof(m_hThreadPrv));
    ZeroMemory(m_pDrvHeadOfPrvBuf, sizeof(m_pDrvHeadOfPrvBuf));
}

CPreview::~CPreview(void)
{
}

void CPreview::CreateWorkerThread()
{
    for ( int i=0; i< m_nDeviceNum; ++i )
    {
        m_hThreadPrv[i] = CreateThread(NULL, 0, OnThread, new ThreadParm(this, i), 0, NULL);
    }
}

void CPreview::DestroyWorkerThread()
{
    DWORD dwExitCode;
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
    }
}

DWORD WINAPI CPreview::OnThread(PVOID pParam)
{
    ThreadParm* pThreadParm = (ThreadParm*)pParam;
    pThreadParm->pthis->ProcessPrv(pThreadParm->dwDeviceID);
    delete pThreadParm;
    return 0;
}

void CPreview::CreateBuffer()
{
    for (int i = 0; i < m_nDeviceNum; i++)
    {
        for (int j = 0; j < PRV_BUF_NUM; j++)
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
    }
}

void CPreview::DestroyBuffer()
{
    for (int i = 0; i < m_nDeviceNum; i++)
    {
#ifdef PRECOPY 
        //heliang+
        for (int j = 0; j < PRV_BUF_NUM; j++)
        {
            safeDeleteArray(m_pPrvBuf[i][j].pBuf);
            m_pPrvBuf[i][j].nVLostFlag = 1;
            safeDeleteArray(m_pDrvHeadOfPrvBuf[i][j]); 
        }
#endif
    }
}

void CPreview::GetPrvData( int nDevice )
{
    //StartStopWatch();

    static DWORD dwStreamType = STREAM_TYPE_PRV;
    PBYTE pData;
    PTVT_CAP_STATUS pStatus;
    PTVT_PREV_VBI pVBI;
    DWORD dwReturn;
    while (TRUE)	//ȡ��Driver������BUF������BUF����
    {
        ControlDriver(
            nDevice,
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
        for (int nChannel = 0; nChannel < CHANNEL_PER_DEVICE; nChannel++)
        {
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
                continue;  // [] continue;
            }    

            //���ݲ���ָ��
            BYTE* pBuf = pData + CAP_STATUS_SIZE + (PREV_VBI_SIZE + CIF_BUFF_SIZE + MOTION_STATUS) * nChannel + PREV_VBI_SIZE;
#ifdef PRECOPY            
            memcpy(m_pPrvBuf[nDevice][nIndex].pBuf, pBuf, CIF_BUFF_SIZE); 
            memcpy(m_pDrvHeadOfPrvBuf[nDevice][nIndex], pStatus, sizeof(TVT_CAP_STATUS));	//�û����Ӧ��DRIVER��BUFͷ
#else
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
    }
}

void CPreview::ProcessPrv(int nDevice)
{
    while (TRUE)
    {
        DWORD dwWait = WaitForSingleObject(m_hPrvEvent[nDevice], 5000);
        //ȷ����ֹ�߳�ǰ�������в������õ�DSP
        if(m_bQuit)	//���ÿ�û�еȴ����õĲ����������߳�
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

BOOL CPreview::FindPrvBuf( INT nDevice, INT &nIndex )
{
    return FindTBuf<PRV_BUF_NUM>(nDevice, nIndex,m_pPrvBufCS,m_pPrvBuf);
}


void CPreview::ReleasePrvBuf(INT nDevice, INT nIndex)
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

// End of file

