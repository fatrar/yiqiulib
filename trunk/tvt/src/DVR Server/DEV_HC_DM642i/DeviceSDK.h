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
    public CDeviceManager
{
    // CDeviceManager
public:
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
	
public:
    // 用类厂导出给外部调用智能方面的功能，不在这个类去实现，可以减少一层的数据传递
    CDSP *m_pDSP;  
    HINSTANCE m_hDllModule;

protected:		
	int m_nCurrentMonitor;	//记录当前Call到的通道号

	int DeleteFiles(DWORD dwVideoFormat);
	int ExtractFiles(DWORD dwVideoFormat);
	BOOL m_bAlarmCard;
	
	VIDEO_PROPER m_VideoProperty[16];
	DWORD m_dwVideoFormat;
	BOOL m_bInitialized;

	static const VIDEO_PROPER	m_default_Video_Proper;	//修改默认值	Modify By zhangzhen 2007/03/05
	void  MapVideoProperByDefault(VIDEO_PROPER& vp);
};

#endif // !defined(AFX_DEVICESDK_H__CD1EF939_D9BB_47BF_9498_80271581E289__INCLUDED_)
