// DeviceSDK.h: interface for the CDeviceSDK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICESDK_H__CD1EF939_D9BB_47BF_9498_80271581E289__INCLUDED_)
#define AFX_DEVICESDK_H__CD1EF939_D9BB_47BF_9498_80271581E289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDSP;

class CDeviceSDK : 
    public CDeviceManager,
    public IIVDeviceBase,
    public IIVStatistic,
    public IIVDeviceSetter
{
    // CDeviceManager
public:
	BOOL m_bNeedExtractDSPFile;
	CDeviceSDK();
	virtual ~CDeviceSDK();
	
	virtual BOOL SupportCardFunc(DWORD FuncType);
	virtual DWORD GetCardOpt(DWORD dwOptType);
	virtual BOOL SetCardOpt(DWORD OptType,DWORD dwOptValue);
	virtual BOOL GetAllChannelStatus(DWORD StatusType,DWORD StatusLen,DWORD	pChStatus[]);
	virtual BOOL SetChannelStatus(DWORD StatusType, DWORD index,long Status);
	virtual BOOL DeviceInitial(DWORD VideoFormat, CAPTURECALLBACK *pVideoCallBack, CAPTURECALLBACK *pAudioCallBack, HWND hwnd,DWORD * pProductType);
	virtual void DeviceFree(void);
	virtual BOOL GetVideoStreamInfo(DWORD nStreamtype, DWORD VideoSizeType, BITMAPINFOHEADER *pBmpHdr);
	virtual BOOL GetAudioStreamInfo(WAVEFORMATEX *pWFrmat);
	virtual BOOL VideoCaptureStart(DWORD dwBmpSizeType,BITMAPINFOHEADER RecBmpHeader);
	virtual BOOL VideoCaptureStop(void);
	virtual BOOL AudioCaptureStart(WAVEFORMATEX WFrmat);
	virtual BOOL AudioCaptureStop(void);
	virtual BOOL GetVideoPropertyValue(DWORD ChIndex,VIDEO_PROPER  &VideoProperValue);
	virtual BOOL GetVideoDefaultPropertyValue(DWORD ChIndex,ENUM_VIDEO_AUDIO_PROPERTY prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal);
	virtual BOOL SetVideoPropertyValue(DWORD ChIndex,VIDEO_PROPER  pVideoProperValue);
	virtual BOOL GetAudioPropertyValue(DWORD AudioIndex,AUDIO_PROPER  &AudioProperValue);
	virtual BOOL GetAudioDefaultPropertyValue(DWORD AudioIndex,ENUM_VIDEO_AUDIO_PROPERTY prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal);
	virtual BOOL SetAudioPropertyValue(DWORD AudioIndex,AUDIO_PROPER  pAudioProperValue);
	virtual BOOL SetSwitch(DWORD *pSwitch, DWORD ChannelNum);
	virtual BOOL SetMotionRect(DWORD index,  BYTE pByte[], long cnByte);
	virtual BOOL SetChannelAlarm(DWORD ChannelNum , DWORD pChAlarm[]);
	virtual BOOL GetMaxFrameRate(DWORD ChannelNum,BOOL pSwitch[],int pFrameRate []);
	virtual BOOL Password(void);
	virtual BOOL ReleaseBuffer(DWORD isVideo,DWORD DelBufPara);
	virtual BOOL GetQuadRect(DWORD sizetype,DWORD &CamNumPerF,CRect pRect[]);
	HINSTANCE m_hDllModule;


    // IIVDeviceBase
public:
    virtual BOOL IsUse(int nChannelID);
    virtual BOOL Use(int nChannelID, bool bState);
    virtual BOOL ShowObjTrace(bool bState);
    virtual BOOL GetObjTraceState(bool& bState);
    virtual BOOL IsHaveFreeDevice(void);

    // IIVStatistic
public:
    virtual BOOL IsHaveStatisticRule(int nChannelID);
    virtual BOOL ResetStatistic(int nChannelID);
    virtual BOOL StartStatistic(int nChannelID, bool bFlag);
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag);

    // IIVDeviceSetter
public:
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm);
    virtual void SetIVDataCallBack(IIVDataSender* pIVDataSender);

protected:	
	
	int m_nCurrentMonitor;	//记录当前Call到的通道号

	int DeleteFiles(DWORD dwVideoFormat);
	int ExtractFiles(DWORD dwVideoFormat);
	BOOL m_bAlarmCard;
	CDSP *m_pDSP;
	VIDEO_PROPER m_VideoProperty[16];
	DWORD m_dwVideoFormat;
	BOOL m_bInitialized;

	static const VIDEO_PROPER	m_default_Video_Proper;	//修改默认值	Modify By zhangzhen 2007/03/05
	void  MapVideoProperByDefault(VIDEO_PROPER& vp);
};

#endif // !defined(AFX_DEVICESDK_H__CD1EF939_D9BB_47BF_9498_80271581E289__INCLUDED_)
