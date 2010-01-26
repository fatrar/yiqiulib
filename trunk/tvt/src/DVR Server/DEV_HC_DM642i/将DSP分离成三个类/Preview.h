/*H***************************************************************************
 File            : Preview.h
 Subsystem       : 
 Function Name(s): CPreview
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 13:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _PREVIEW_H_2010_
#define _PREVIEW_H_2010_


template<typename T>
struct ThreadParmT
{
    ThreadParmT(T* p, DWORD ID):pthis(p),dwDeviceID(ID){}
    T* pthis;
    DWORD dwDeviceID;
};


class CFunctionBase
{
public:
    static int m_nDeviceNum;
    static DWORD m_pSwitch[MAX_CHANNEL_NUM];
    static HANDLE m_hDevice[MAX_DEVICE_NUM];
    static PACK m_pPack[MAX_DEVICE_NUM];
    static SIGNAL m_pSignal[MAX_CHANNEL_NUM];
    static BOOL m_bNetstart;
    static DWORD m_dwNetChannelMask;
    static int m_nNetFrameRateInc;
    static BOOL m_bQuit;
public:
    BOOL ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned)
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

    void SetSignal(int nChannel, UINT iS);
    BOOL SetParamToDSP(INT nDevice);
    DWORD RefreshNetFrameRate();
    BOOL SetParam(int nType, int nChannel, int nValue);
};


inline void ReleaseDriverBuffer(PTVT_CAP_STATUS pST)
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

template<int BufSize>
inline BOOL FindTBuf(
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

class CPreview :
    public CFunctionBase
{
public:
    CPreview(void);
    ~CPreview(void);
public:
    void CreateWorkerThread();

    void DestroyWorkerThread();

    void CreateBuffer();

    void DestroyBuffer();

protected:
    static DWORD WINAPI OnThread(PVOID pParam);

    void ProcessPrv(int nDevice);
    void GetPrvData(int nDevice);

protected:
    typedef ThreadParmT<CPreview> ThreadParm;

    BOOL FindPrvBuf(INT nDevice, INT &nIndex);

    void ReleasePrvBuf(INT nDevice, INT nIndex);

private:
    CCriticalSection m_pPrvBufCS[MAX_DEVICE_NUM];	
    FRAMEBUFSTRUCT m_pPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];
    HANDLE m_hThreadPrv[MAX_DEVICE_NUM];
    HANDLE m_hPrvEvent[MAX_DEVICE_NUM];
    PTVT_CAP_STATUS m_pDrvHeadOfPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];	//��¼ÿ���ֳ�������������DRIVER�㻺��ͷ

    DWORD m_dwPrvBufSize;
    CAPTURECALLBACK * m_pVideoCallBack;
};




class CAudio :
    public CFunctionBase
{
public:
    CAudio();
    ~CAudio();

public:
    void CreateWorkerThread();

    void DestroyWorkerThread();

    void CreateBuffer();

    void DestroyBuffer();

protected:
    static DWORD WINAPI OnThread(PVOID pParam);

protected:

    typedef ThreadParmT<CPreview> ThreadParm;
public:

}




#endif  // End of file