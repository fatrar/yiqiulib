﻿/**H**************************************************************************
 File            : DSP.h
 Subsystem       : 
 Function Name(s): CDSP
 Author          : YiQiu
 Date            : 2010-4-8  
 Time            : 16:24
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DSP_H_2010_4
#define _DSP_H_2010_4
#pragma once

//#define _UseLiveTime

class CTiCoffFile;
#include "CounterTime.h"

//Add By zhangzhen 2007/02/09
extern void ReleaseDriverBuffer(PTVT_CAP_STATUS pST);

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
    //public IIVStatistic,
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
	void ReleasePrvBuf(int nDevice, int nChannelID, INT nIndex);
	BOOL FindPrvBuf(int nChannelID, INT &nIndex);

    CCriticalSection m_pPrvBufCS[MAX_CHANNEL_NUM];

    CCriticalSection m_pNetBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pMobileBufCS[MAX_DEVICE_NUM];
	CCriticalSection m_pNetBufCS_RT[MAX_DEVICE_NUM];	//<REC-NET>
    CCriticalSection m_pAudBufCS[MAX_DEVICE_NUM];
    CCriticalSection m_pCapBufCS[MAX_DEVICE_NUM];


    FRAMEBUFSTRUCT m_pPrvBuf[MAX_CHANNEL_NUM][PRV_BUF_NUM];

	FRAMEBUFSTRUCT m_pNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];
	FRAMEBUFSTRUCT m_pMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];
	FRAMEBUFSTRUCT m_pNetBuf_RT[MAX_DEVICE_NUM][CAP_BUF_NUM];	//<REC-NET>  
    FRAMEBUFSTRUCT m_pAudBuf[MAX_DEVICE_NUM][AUD_BUF_NUM];
    FRAMEBUFSTRUCT m_pCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];

protected:
    void DestroyWorkerThread();
	BOOL CreateWorkerThread();

    BOOL CreateBuffer();
	void DestroyBuffer();
	HANDLE m_hThreadAud[MAX_DEVICE_NUM];
	HANDLE m_hThreadPrv[MAX_DEVICE_NUM];
	HANDLE m_hThreadCompressStrm[MAX_DEVICE_NUM];

	static DWORD WINAPI OnThreadAud(PVOID pParam);
	static DWORD WINAPI OnThreadPrv(PVOID pParam);
	static DWORD WINAPI OnThreadCompressStrm(PVOID pParam);

	CAPTURECALLBACK * m_pAudioCallBack;
	CAPTURECALLBACK * m_pVideoCallBack;
	DWORD m_dwVideoFormat;
	
	HANDLE m_hCompressEvent[MAX_DEVICE_NUM];
	HANDLE m_hAudEvent[MAX_DEVICE_NUM];
	HANDLE m_hPrvEvent[MAX_DEVICE_NUM];

	BOOL DeviceInit();
    void DeviceExit();
	HANDLE m_hDevice[MAX_DEVICE_NUM];
	BOOL ControlDriver(INT nIndex, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned);

private:
	PTVT_CAP_STATUS m_pDrvHeadOfNetBuf[MAX_DEVICE_NUM][NET_BUF_NUM];	//记录每个网络缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfMobileBuf[MAX_DEVICE_NUM][MOBILE_BUF_NUM];	//记录每个手机缓冲区所属的DRIVER层缓冲头
	PTVT_CAP_STATUS m_pDrvHeadOfCapBuf[MAX_DEVICE_NUM][CAP_BUF_NUM];	//记录每个录像缓冲区所属的DRIVER层缓冲头

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
        ThreadParm(CDSP* p, DWORD ID)
            : pthis(p), dwDeviceID(ID){}
        CDSP* pthis;
        DWORD dwDeviceID;
    };

    struct SmoonthThreadParm : public ThreadParm
    {
        SmoonthThreadParm(CDSP* p, DWORD ID, HANDLE h)
            : ThreadParm(p,ID), handle(h){}
        HANDLE handle;
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
    virtual BOOL IsHaveFreeDevice(void);
    virtual BOOL IsHaveStatisticRule(int nChannelID);
    virtual void GetDeviceInfo(
        size_t* pnDeviceNum,
        size_t* pnChannelNumByDevice );

    virtual void GetIVDeviceInfo(
        size_t* pnIVChannelNumByDevice,
        size_t* pnMaxRuleNumByIVChannel );

    // IIVSimulation
public:
    virtual BOOL StartSimulation(
        int nChannelID, 
        IIVSimulationAlarmCallBack* p,
        const WPG_Rule& Rule);
    virtual BOOL StopSimulation(int nChannelID);

    // IIVDeviceBase2
public:
    virtual BOOL Use(int nChannelID, bool bState);

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

    // IIVDeviceSetter
public:
    virtual void SetIVAlarmOutCallBack(
        AlarmCallBackFn pAlarmCallBackFn, void* pParm);
    virtual void SetIVDataCallBack(
        IIVDataSender* pIVDataSender,
        IIVStatisticFresher* pIVStatisticFresher);
    virtual void SetSnapShotCallBack(ISnapShotSender* pSnapShotSender);

    // Do IV Data
private:
    void DoIVData(int nDevice, PBYTE pData);

    void DoSnapShot(
        int nChannelID,
        const WPG_EventOccurrence* pEvent, 
        BYTE*& pFirstPic);

    void PassSnapShot(
        const WPG_EventOccurrence* pEvent, 
        BYTE*& pFirstPic);

    void DoIVAlarm(
        int nChannelID,
        const WPG_EventOccurrence* pEvent, 
        FILETIME* pTime,
        const AlarmOutTable* pTable);

    bool IsNeedAlarmOut(
        int nDevice,
        const IV_RuleID& RuleID,
        const FILETIME& test,
        const AlarmOutTable*& pTable );

private:
    static DWORD WINAPI OnThreadAlarmCallBack(PVOID pParam);

    DWORD LoopAlarmCallBack();

    struct IVAlarmCallBackParm
    {
        IVAlarmCallBackParm(){}
        IVAlarmCallBackParm(
            IVRuleType _t, int _nChannel,
            AlarmOutTable _Table, FILETIME _Time)
            : t(_t)
            , nChannel(_nChannel)
            , Table(_Table)
            , Time(_Time){}
        IVRuleType t;
        int nChannel;
        AlarmOutTable Table;
        FILETIME Time;
    };
    /**
    *@note 智能报警
    */
    AlarmCallBackFn m_AlarmCallBackFn;
    void* m_pAlarmCallBackParm;
    IIVDataSender* m_pIVDataSender;
    IIVStatisticFresher* m_pIVStatisticFresher;
    ISnapShotSender* m_pSnapShotSender;
    int m_szCurrentIVChannel[MAX_DEVICE_NUM];
    deque<IVAlarmCallBackParm> m_IVAlarmCallBackParmQueue;
    HANDLE m_hIVAlarmCallBackThread;
    DWORD m_dwIVAlarmCallBackTheadID;
    CCriticalSection m_AlarmCallBackCS;
    enum AlarmCallBackThreadMsg
    {
        Alarm_Coming,
    };
    //BOOL m_ShowSnapShot;

private:
    /**
    *@note 视频额外的预览，比如当用户点击到规则配置主界面，
    */
    IVideoSend* m_szVideoSend[MAX_CHANNEL_NUM];
    CCriticalSection m_VideoSendCS[MAX_CHANNEL_NUM];

private:
    /**
    *@note 智能模拟
    */
    IIVSimulationAlarmCallBack* m_pIVAlarmCallBack;
    int m_SimulationChanID;
    int m_LastRunChanID;
    IV_RuleID m_SimulationRuleID;
    WPG_LINE_CROSS_DIRECTION m_StatisticDir;
    CCriticalSection m_SimulationCS;

    void SimulationAddRule(
        int nChannelID,
        const WPG_Rule& Rule);
    void SimulationRemoveRule(
        int nChannelID);

private:
    /**
    *@note Normal Rule Buf
    *@details 这个数据结构只会保存正在运行的IV的通道的数据
    * 主程序起来后，应该调数据配置模块读取上一次用户设置的数据，
    * 然后将默认运行的智能的通道的所有规则设置给本模块,
    * 当有智能切换时，应该首先清空正在运行智能的通道保存的数据，
    * 然后将新通道的所有规则设置到本模块
    */
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

    template<typename T, T CurrentRuleSetting::*t>
    inline BOOL ModifyXX(
        int nChannelID,
        const IV_RuleID& RuleID,
        const T& V );

private:
    /**
    *@note Statistic Function
    */
    struct StatisticSetting
    {
        StatisticSetting():bIsEnable(FALSE){}
        WPG_Rule Rule;
        BOOL bIsEnable;
    };
    StatisticSetting* m_pStatisticRule[MAX_DEVICE_NUM];

    BOOL _AddStatistic(
        int nChannelID,
        const WPG_Rule& Rule);

    BOOL _RemoveStatistic(
        int nChannelID,
        const WPG_Rule& Rule);

    BOOL AddStatistic(
        int nChannelID,
        int nDeviceID,
        const WPG_Rule& Rule);

    BOOL RemoveStatistic(
        int nChannelID,
        int nDeviceID,
        const IV_RuleID& RuleID);

    BOOL EnableStatistic(
        int nChannelID,
        int nDeviceID,
        const IV_RuleID& RuleID,
        BOOL bEnable);

    BOOL ModifyStatistic(
        int nChannelID,
        int nDeviceID,
        const WPG_Rule& Rule);

    // just test
#ifdef _UseLiveTime
    SYSTIME  m_prevVideoTime;
#endif
    
private:
    /**
    *@note Smooth IV Live
    */
    enum SmoothDef
    {
        Switch_Channel = WM_USER + 1,
        Push_Live_Data,
        IV_Data_Coming,
        Play_Again,
        Alarm_Occur,
        End_Thead,
    };

    HANDLE m_hSmooth[MAX_DEVICE_NUM];
    DWORD m_dwSmoothTheadID[MAX_DEVICE_NUM];
    //BOOL m_bFisrtIVDataFlag[MAX_DEVICE_NUM];

    static DWORD WINAPI OnThreadSmooth(PVOID pParam);

    template<int nDevice>
    static void CALLBACK SmoothTimer(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2);

    LPTIMECALLBACK GetSmoothTimer(int nDevice);

    void LoopLiveSmooth(int nDevice, HANDLE h);
    inline void FreeLiveList(list<FRAMEBUFSTRUCT*>& LiveList);
    inline void VideoSend(int nChannel, FRAMEBUFSTRUCT* p);

    // just Test
    BOOL m_bIsOperatorDevice[MAX_DEVICE_NUM];
};









#endif  // _DSP_H_2010_


// End of file

