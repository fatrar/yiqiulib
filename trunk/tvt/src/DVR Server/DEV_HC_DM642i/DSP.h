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

#define MAX_IV_Parm_Buf_Size (sizeof(DWORD) + sizeof(PARAMPACK) + sizeof(WPG_Rule))

struct IVParmData
{
    PARAMPACK Commadparm;
    WPG_Rule Rule;
};

struct IVPACK
{
    CCriticalSection CS;
    deque<IVParmData> param;
};


class CDSP :
    public IIVDeviceBase2,
    public IIVStatistic,
    public IIVDeviceSetter
{
public:
    CDSP();
    virtual ~CDSP();

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

	BOOL m_bNetstart;
	INT m_nDeviceNum;
	DWORD m_dwNetChannelMask;	//网络通道开关	Add By zhangzhen 2007/10/11
protected:
	BYTE Encrypt(INT nDevice, BYTE byRandom);
	
	
	void ReleaseNetBuf(INT nDevice, INT nIndex);
	void ReleaseMobileBuf(INT nDevice, INT nIndex);
	BOOL FindNetBuf(INT nDevice, INT &nIndex);
	BOOL FindMobileBuf(INT nDevice, INT &nIndex);
	ULONGLONG ChangeTime(const SYSTIME &time);

protected:
    FILETIME m_ftNetLastFrameTime[MAX_CHANNEL_NUM];
    CCounterTime m_CounterTime;
	BOOL m_bStart;
	BOOL m_bQuit;

	DWORD m_dwWidth;
	DWORD m_dwHeight;
	DWORD m_dwMotionSize;
	DWORD m_dwPrvBufSize;
	
    SIGNAL m_pSignal[MAX_CHANNEL_NUM];
	PACK m_pPack[MAX_DEVICE_NUM];
    IVPACK m_pIVPack[MAX_DEVICE_NUM];          // IV new Add
    BYTE* m_pIVParmBuf[MAX_DEVICE_NUM];        // IV new Add

    DWORD m_pSwitch[MAX_CHANNEL_NUM];
    DWORD m_pAudioSwitch[MAX_CHANNEL_NUM];

    void SetSignal(int nChannel, UINT iS);
    BOOL SetParamToDSP(int nDevice);       

    BOOL SetIVParamToDSP(int nDevice);       // IV new Add
    BOOL SetIVSpecialParam(
        int nType, int nChannel, int nValue, // nValue 预留，默认填0就好
        const WPG_Rule& Rule );              // IV new Add

	void ReleaseCapBuf(INT nDevice, INT nIndex);
	BOOL FindCapBuf(INT nDevice, INT &nIndex);
	BOOL FindNetBuf_RT(INT nDevice, INT &nIndex);	//<REC-NET>
	void ReleaseNetBuf_RT(INT nDevice, INT nIndex);	//<REC-NET>
	void ReleaseAudBuf(INT nDevice, INT nIndex);
	BOOL FindAudBuf(INT nDevice, INT &nIndex);
	void ReleasePrvBuf(INT nDevice, INT nIndex);
	BOOL FindPrvBuf(INT nDevice, INT &nIndex);
	
    CCriticalSection m_pPrvBufCS[MAX_DEVICE_NUM];	
    CCriticalSection m_pNetBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pMobileBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pNetBufCS_RT[MAX_DEVICE_NUM];	//<REC-NET>
    CCriticalSection m_pAudBufCS[MAX_DEVICE_NUM];
    CCriticalSection m_pCapBufCS[MAX_DEVICE_NUM];

    FRAMEBUFSTRUCT m_pPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];
	FRAMEBUFSTRUCT m_pNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];
	FRAMEBUFSTRUCT m_pMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];
	FRAMEBUFSTRUCT m_pNetBuf_RT[MAX_DEVICE_NUM][CAP_BUF_NUM];	//<REC-NET>  
    FRAMEBUFSTRUCT m_pAudBuf[MAX_DEVICE_NUM][AUD_BUF_NUM];
    FRAMEBUFSTRUCT m_pCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];

protected:
    void DestroyWorkerThread();
	BOOL CreateWorkerThread();
	void DestroyBuffer();
	HANDLE m_hThreadAud[MAX_DEVICE_NUM];
	HANDLE m_hThreadPrv[MAX_DEVICE_NUM];
	HANDLE m_hThreadCompressStrm[MAX_DEVICE_NUM];

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
	HANDLE m_hDevice[MAX_DEVICE_NUM];
	BOOL ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned);

private:
	PTVT_CAP_STATUS m_pDrvHeadOfNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];	//记录每个网络缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];	//记录每个手机缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];	//记录每个录像缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfPrvBuf[MAX_DEVICE_NUM][PRV_BUF_NUM];	//记录每个现场缓冲区所属的DRIVER层缓冲头

	BOOL m_bNextFrameIsKeyRcd[MAX_CHANNEL_NUM];	//录像流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	BOOL m_bNextFrameIsKeyNet[MAX_CHANNEL_NUM];	//网络流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	BOOL m_bNextFrameIsKeyMobile[MAX_CHANNEL_NUM];	//网络流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	BOOL m_bNextFrameIsKeyNet_RT[MAX_CHANNEL_NUM];	//<REC-NET>网络实时流流关键帧需求状态	TRUE--下帧必须是关键帧，FALSE--无要求(可以为关键帧，也可以是非关键帧)
	
    int m_nFrameCntRcd[MAX_CHANNEL_NUM];	//统计录像流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)
    int m_nFrameCntNet[MAX_CHANNEL_NUM];	//统计网络流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)
	int m_nFrameCntMobile[MAX_CHANNEL_NUM];	//统计手机流两个关键帧之间的帧数(正常情况下上限为100，超过100则需要强制压缩一个关键帧)

private:
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

    // IV New Add
private:
    void GetPrvData(int nDevice);

    void GetOneChannelPreData(
        int nChannel,
        int nDevice,
        PBYTE pData,
        PTVT_CAP_STATUS pStatus );

    void GetCompressData(int nDevice);

    //
    // **********************    IV   *******************
    //
    // IIVDeviceBase
public:
    virtual BOOL IsUse(int nChannelID);
    virtual BOOL Use(int nChannelID, bool bState);
    virtual BOOL IsHaveFreeDevice(void);

    // IIVSimulation
public:
    virtual void Start(
        int nChannelID, 
        IIVSimulationAlarmCallBack* p,
        const WPG_Rule& Rule);
    virtual void Stop(int nChannelID);

    // IIVDeviceBase2
public:
    /**
    *@note Add a Rule, when Program is Run, Set All Rule To Device which channel is Auto Run Channel
    *@param	nChannelID  Channel ID
    *@param RuleID      Rule ID 
    *@param bEnable     Enable or Disable
    *@return Is Succeeded
    */
    virtual BOOL Add(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings);

    virtual BOOL Remove(
        int nChannelID,
        const IV_RuleID& RuleID);

    /**
    *@note Enable a Rule
    *@param	nChannelID  Channel ID
    *@param RuleID      Rule ID 
    *@param bEnable     Enable or Disable
    *@return Is Succeeded
    */
    virtual BOOL EnableRule(
        int nChannelID,
        const IV_RuleID& RuleID,
        BOOL bEnable =TRUE);

    virtual BOOL ModifyRule(
        int nChannelID,
        const WPG_Rule& Rule);

    virtual BOOL ModifySchedule(
        int nChannelID,
        const IV_RuleID& RuleID,
        const ScheduleSettings& Sch );

    virtual BOOL ModifyAlarmOut(
        int nChannelID,
        const IV_RuleID& RuleID,
        const AlarmOutSettings& Alarm );

    virtual void RegisterLiveDataCallBack(
        int nChannelID,
        IVideoSend* pVideoSend);

    virtual void UnRegisterLiveDataCallBack(
        int nChannelID, 
        IVideoSend* pVideoSend);

    virtual void ReleaseLiveBuf(FRAMEBUFSTRUCT* p);

    // IIVStatistic
public:
    virtual BOOL IsHaveStatisticRule(int nChannelID);
    virtual BOOL ResetStatistic(int nChannelID);
    virtual BOOL StartStatistic(int nChannelID, bool bFlag);
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag);

    // IIVDeviceSetter
public:
    virtual void SetIVAlarmOutCallBack(
        AlarmCallBackFn pAlarmCallBackFn, void* pParm);
    virtual void SetIVDataCallBack(IIVDataSender* pIVDataSender);

    // Do IV Data
private:
    void DoIVData(int nDevice, PBYTE pData);

    void DoSnapShot(
        int nChannelID,
        const WPG_EventOccurrence* pEvent, 
        BYTE*& pFirstPic);

    void DoIVAlarm(
        int nChannelID,
        const WPG_EventOccurrence* pEvent, 
        FILETIME* pTime,
        const AlarmOutTable* pTable);

private:
    bool IsNeedAlarmOut(
        int nDevice,
        const IV_RuleID& RuleID,
        const FILETIME& test,
        const AlarmOutTable*& pTable );

private:
    AlarmCallBackFn m_AlarmCallBackFn;
    void* m_pAlarmCallBackParm;
    IIVDataSender* m_pIVDataSender;
    ISnapShotSender* m_pSnapShotSender;
    int m_szCurrentIVChannel[MAX_DEVICE_NUM];
    BOOL m_szHaveStatistic[MAX_DEVICE_NUM];

    // 视频额外的预览
    IVideoSend* m_szVideoSend[MAX_CHANNEL_NUM];
    CCriticalSection m_VideoSendCS;

    enum 
    {
        Invaild_ChannelID= -1,
        Device_Free_Flag = -1,
    };

    // 智能模拟
    IIVSimulationAlarmCallBack* m_pIVAlarmCallBack;
    int m_SimulationChanID;
    IV_RuleID m_RuleID;
#define m_SimulationCS m_VideoSendCS

    struct CurrentRuleSetting
    {
        CurrentRuleSetting(
            const WPG_Rule& _Rule,
            const ScheduleSettings& _Sch,
            const AlarmOutSettings& _Alarm)
            : Rule(_Rule)
            , Sch(_Sch)
            , Alarm(_Alarm)
            , nLastHoldTime(0) {}
            
        WPG_Rule Rule;
        ScheduleSettings Sch;
        AlarmOutSettings Alarm;
        __int64 nLastHoldTime;
    };

    typedef map<IV_RuleID, CurrentRuleSetting*> RuleSettingMap;
    RuleSettingMap m_RuleCfgMap[MAX_DEVICE_NUM];
    CCriticalSection m_CfgMapCS[MAX_DEVICE_NUM];	

    BOOL m_ShowSnapShot;


    // just test
    SYSTIME  m_prevVideoTime;
};




#endif // !defined(AFX_DSP_H__C706B57D_C9A0_4A6C_A74E_FE16F5B26E14__INCLUDED_)
