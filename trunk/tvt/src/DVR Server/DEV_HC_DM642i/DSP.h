// DSP.h: interface for the CDSP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSP_H__C706B57D_C9A0_4A6C_A74E_FE16F5B26E14__INCLUDED_)
#define AFX_DSP_H__C706B57D_C9A0_4A6C_A74E_FE16F5B26E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTiCoffFile;
#include "CounterTime.h"

//Add By zhangzhen 2007/02/09
extern inline	void ReleaseDriverBuffer(PTVT_CAP_STATUS pST);
#define NET_FRAME_RATE_TOTAL 32

class CDSP  
{
public:
	BOOL m_bRecordStop[MAX_CHANNEL_NUM];	//<REC-NET>	记录录像状态开关
	int m_nNetFrameRateInc;	//网络帧率增量	Add By zhangzhen 2007/10/17
	DWORD NetFrameRateInc(int inc);
	DWORD RefreshNetFrameRate();
	BOOL SetAudioSwitch(DWORD dwChannel, LONG lStatus);
	BOOL Password();
	void CheckVideoLoss(DWORD *pStatus, DWORD dwLen);
	BOOL SetParam(int nType, int nChannel, int nValue);
	DWORD GetDevNum();
	BOOL SetSwitch(DWORD *pSwitch, DWORD ChannelNum);
	BOOL ReleaseBuffer(DWORD isVideo,DWORD DelBufPara);
	void Terminate();
	BOOL CaptureStop();
	BOOL CaptureStart();
	BOOL Initialize(DWORD dwVideoFormat, CAPTURECALLBACK *pVideoCallBack, CAPTURECALLBACK *pAudioCallBack);
	CDSP();
	virtual ~CDSP();
	BOOL m_bNetstart;
	INT m_nDeviceNum;
	DWORD m_dwNetChannelMask;	//网络通道开关	Add By zhangzhen 2007/10/11
protected:
	BYTE Encrypt(INT nDevice, BYTE byRandom);
	
	FILETIME m_ftNetLastFrameTime[MAX_CHANNEL_NUM];
	CCounterTime m_CounterTime;
	void ReleaseNetBuf(INT nDevice, INT nIndex);
	void ReleaseMobileBuf(INT nDevice, INT nIndex);
	BOOL FindNetBuf(INT nDevice, INT &nIndex);
	BOOL FindMobileBuf(INT nDevice, INT &nIndex);
	ULONGLONG ChangeTime(const SYSTIME &time);
	//CTiCoffFile *m_pTiCoffFile[MAX_DEVICE_NUM];
	BOOL m_bStart;
	BOOL m_bQuit;
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	DWORD m_dwMotionSize;
	DWORD m_dwPrvBufSize;
	void SetSignal(int nChannel, UINT iS);
	SIGNAL m_pSignal[MAX_CHANNEL_NUM];
	PBYTE m_pParamData[MAX_DEVICE_NUM];
	BOOL SetParamToDSP(INT nDevice);
	PACK m_pPack[MAX_DEVICE_NUM];
	void ReleaseCapBuf(INT nDevice, INT nIndex);
	BOOL FindCapBuf(INT nDevice, INT &nIndex);
	BOOL FindNetBuf_RT(INT nDevice, INT &nIndex);	//<REC-NET>
	void ReleaseNetBuf_RT(INT nDevice, INT nIndex);	//<REC-NET>
	DWORD m_pSwitch[MAX_CHANNEL_NUM];
	DWORD m_pAudioSwitch[MAX_CHANNEL_NUM];
	void ReleaseAudBuf(INT nDevice, INT nIndex);
	BOOL FindAudBuf(INT nDevice, INT &nIndex);
	void ReleasePrvBuf(INT nDevice, INT nIndex);
	BOOL FindPrvBuf(INT nDevice, INT &nIndex);
	
    CCriticalSection m_pPrvBufCS[MAX_DEVICE_NUM];
	FRAMEBUFSTRUCT m_pPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];
	
    CCriticalSection m_pNetBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pMobileBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pNetBufCS_RT[MAX_DEVICE_NUM];	//<REC-NET>
	FRAMEBUFSTRUCT m_pNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];
	FRAMEBUFSTRUCT m_pMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];
	FRAMEBUFSTRUCT m_pNetBuf_RT[MAX_DEVICE_NUM][CAP_BUF_NUM];	//<REC-NET>
//	NETBUF_DEQUE m_pNetBufDeque[MAX_DEVICE_NUM];
	
    void DestroyWorkerThread();
	BOOL CreateWorkerThread();
	void DestroyBuffer();
	HANDLE m_hThreadAud[MAX_DEVICE_NUM];
	HANDLE m_hThreadPrv[MAX_DEVICE_NUM];
	HANDLE m_hThreadCompressStrm[MAX_DEVICE_NUM];

	CCriticalSection m_pAudBufCS[MAX_DEVICE_NUM];
	FRAMEBUFSTRUCT m_pAudBuf[MAX_DEVICE_NUM][AUD_BUF_NUM];
	CCriticalSection m_pCapBufCS[MAX_DEVICE_NUM];
	FRAMEBUFSTRUCT m_pCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];

	static DWORD WINAPI OnThreadAud(PVOID pParam);
	static DWORD WINAPI OnThreadPrv(PVOID pParam);
	static DWORD WINAPI OnThreadCompressStrm(PVOID pParam);

	BOOL CreateBuffer();
	CAPTURECALLBACK * m_pAudioCallBack;
	CAPTURECALLBACK * m_pVideoCallBack;
	DWORD m_dwVideoFormat;
	void DeviceExit();
	HANDLE m_hCompressEvent[MAX_DEVICE_NUM];
	HANDLE m_hAudEvent[MAX_DEVICE_NUM];
	HANDLE m_hPrvEvent[MAX_DEVICE_NUM];
	BOOL DeviceInit();
	void VariableInit();
	HANDLE m_hDevice[MAX_DEVICE_NUM];
	BOOL ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned);
private:
	PTVT_CAP_STATUS m_pDrvHeadOfNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];	//记录每个网络缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];	//记录每个手机缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];	//记录每个录像缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];	//记录每个现场缓冲区所属的DRIVER层缓冲头

	BOOL m_bNextFrameIsKeyRcd[MAX_CHANNEL_NUM];	//录像流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	int m_nFrameCntRcd[MAX_CHANNEL_NUM];	//统计录像流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)
	BOOL m_bNextFrameIsKeyNet[MAX_CHANNEL_NUM];	//网络流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	BOOL m_bNextFrameIsKeyMobile[MAX_CHANNEL_NUM];	//网络流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	BOOL m_bNextFrameIsKeyNet_RT[MAX_CHANNEL_NUM];	//<REC-NET>网络实时流流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	int m_nFrameCntNet[MAX_CHANNEL_NUM];	//统计网络流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)
	int m_nFrameCntMobile[MAX_CHANNEL_NUM];	//统计手机流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)
	int CmprssStreamProcessNet(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST);
	int CmprssStreamProcessRcd(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST);
	int CmprssStreamProcessMobile(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST);
	int CmprssStreamProcessNet_RT(int nDeviceNo, const PTVT_REC_VBI& pVBI, const PBYTE pVideoData, const PTVT_CAP_STATUS& pST);	//<REC-NET>
	void ProcessCompressStreams(INT nDevice);
	void ProcessAud(INT nDevice);
	void ProcessPrv(INT nDevice);

protected:
    struct ThreadParm
    {
        ThreadParm(CDSP* p, DWORD ID):pthis(p),dwDeviceID(ID){}
        CDSP* pthis;
        DWORD dwDeviceID;
    };
};

#endif // !defined(AFX_DSP_H__C706B57D_C9A0_4A6C_A74E_FE16F5B26E14__INCLUDED_)
