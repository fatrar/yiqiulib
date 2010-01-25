/*H***************************************************************************
 File            : DeviceSDK.cpp
 Subsystem       : 
 Function Name(s): CDeviceSDK
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 11:22
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "DeviceSDK.h"
#include "DSP.h"
#include "..\\AlarmCard\\AlarmCard.h"
//tonytttt
#include "..\netheader\netconfigdata.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//修改默认值	Modify By zhangzhen 2007/03/05
//const VIDEO_PROPER	CDeviceSDK::m_default_Video_Proper = {70,145,128,128,128};
const VIDEO_PROPER	CDeviceSDK::m_default_Video_Proper = {128,128,128,128,128};

CDeviceSDK::CDeviceSDK()
{
	m_hDllModule = NULL;
	m_bInitialized = FALSE;
	m_bAlarmCard = FALSE;
	m_pDSP = NULL;
	for (int i = 0; i < 16; i++)
	{
        m_VideoProperty[i] = m_default_Video_Proper; //修改默认值
	}

	//增加CallMonitor功能，默认全部关闭
	m_nCurrentMonitor = -1;
}

CDeviceSDK::~CDeviceSDK()
{

}

/******************************************************************************
* 函数名称： BOOL SupportCardFunc(DWORD FuncType)
* 功能描述： 设备是否支持某些功能
* 输入参数： FuncType功能类型
* 输出参数： 无
* 返 回 值： 支持返回TRUE，否则返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SupportCardFunc(DWORD FuncType)
{
    ERROR_RETURN_FALSE(!m_bInitialized);

	switch(FuncType)
	{
	case DVRCARD_FUNC_AUTO_RESET:
    case DVRCARD_FUNC_AUTO_ADJUST_FRATE:
    case DVRCARD_FUNC_SUPPORTMOTION:
    case DVRCARD_FUNC_SUPPORTQUAD:
    case DVRCARD_FUNC_OVERLAYDEMO:
		return FALSE;

	case DVRCARD_FUNC_CALL_MONITOR: //支持CALL MONITOR功能
    case DVRCADD_FUNC_WATERMARK:
	case DVRCARD_FUNC_PTZPRESET:
		return TRUE;

	case DVRCARD_FUNC_AUTOMAILIMAGE:
		return HAVEAUTOMAILIMAGE;
	default:
		return FALSE;
	}
}

DWORD CDeviceSDK::GetCardOpt(DWORD dwOptType)
{	
    ERROR_RETURN(!m_bInitialized, 0xFFFFFFFF);

	switch(dwOptType)
	{
	case DVRCARDOPT_VIDEODEV_NUM:
		return m_pDSP->GetDevNum();
	case DVRCARDOPT_AUDIODEV_NUM:
		return 4 * m_pDSP->GetDevNum();
	case DVRCARDOPT_CHANNEL_NUM:
		return 4 * m_pDSP->GetDevNum();
	case DVRCARDOPT_ALARMIN_NUM:
    {
        if (m_bAlarmCard)
		{
			return 16;
		}
        return 0;
    }
	case DVRCARDOPT_ALARMOUT_NUM:
	{
        if (m_bAlarmCard)
        {
	        return 5;
        }
	    return 0;
    }
	case DVRCARDOPT_STREAM_NUM:
		return 5;	//<REC-NET>现场流+录像流+网络流+手机流+网络实时流
	case DVRCARDOPT_STREAM_TYPE:
		return VIDEO_STREAM_PREVIEW | VIDEO_STREAM_CAPTURE | VIDEO_STREAM_NET | VIDEO_STREAM_MOBILE | VIDEO_STREAM_NET_REALTIME/*<REC-NET>*/;
	case DVRCARDOPT_VIDEOSIZE_TYPE:
		if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL)
			return DVRVIDEO_SIZE_352x288;
		else if (m_dwVideoFormat == DVRVIDEO_STANDARD_NTSC)
			return DVRVIDEO_SIZE_320x240;
	default:
		return 0xFFFFFFFF;
	}
}

BOOL CDeviceSDK::SetCardOpt(DWORD OptType,DWORD dwOptValue)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	switch (OptType)
	{
	case DVRCARDOPT_OVERLAY_ADDR:
	case DVRCARDOPT_OVERLAY_PITCH:
	case DVRCARDOPT_OVERLAY_SIZE:
	case DVRCARDOPT_SCREEN_QUAD:
		break;
	case DVRCARDOPT_NETSTREAMSEND:
		{
			m_pDSP->m_bNetstart=!!dwOptValue;
			m_pDSP->RefreshNetFrameRate();
		}
		break;
	case DVRCARDOPT_NET_CHL_MASK:
		{
			m_pDSP->m_dwNetChannelMask = dwOptValue;
			m_pDSP->RefreshNetFrameRate();
		}
		break;
	case DVRCARDOPT_NET_FRAMERATE_INC:	//提升网络帧率
		{
			m_pDSP->NetFrameRateInc((int)dwOptValue);
		}
		break;
	default:
		SetLastError(DVRERR_INVALIDPARA);
		return FALSE;
	}

	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetAllChannelStatus(DWORD StatusType, DWORD StatusLen, DWORD pChStatus[])
* 功能描述： 得到Channel状态包括信号状态、报警状态等
* 输入参数： StatusType 状态类型
*			 StatusLen  状态的数量pChStatus的维数
*			 pChStatus  状态数组
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetAllChannelStatus(DWORD StatusType,DWORD StatusLen,DWORD	pChStatus[])
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	switch (StatusType)
	{
	case CHSTATUS_GET_SIGANL:
		m_pDSP->CheckVideoLoss(pChStatus, StatusLen);
		break;
	case CHSTATUS_GET_SENSOR:
		if (m_bAlarmCard)
		{
			DWORD alarmStaus = 0;
			if(ALM_AlarmIn(&alarmStaus))
			{
				for(DWORD i = 0; i < StatusLen; i++)
				{
					pChStatus[i] = alarmStaus & (0x01 << i);
				}
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
        // bug [] ? heliang m_bAlarmCard=false break?
	case CHSTATUS_GET_MOTION:
		break;
	}

	return TRUE;
}

BOOL CDeviceSDK::SetChannelStatus(DWORD StatusType, DWORD index,long Status)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	if (index >= 4 * m_pDSP->GetDevNum())
	{
		return FALSE;
	}

    // heliang +
    static int s_szBitRate[] = {40000, 150000, 500000, 600000, 1000000};

	WORD wFrameRate = LOWORD(Status);
	WORD wInterval = HIWORD(Status);
	WORD wMotionSens = LOWORD(Status);
	WORD wMotionInter = HIWORD(Status);

	switch (StatusType)
	{
	case CHSTATUS_SET_ALARM:
		{
			if (m_bAlarmCard)
			{
				static DWORD alarmStaus = 0;
				if((5 - 1) == index)//继电器报警
					index = 16;
				if(Status)
					alarmStaus |= (0x01 << index);
				else
					alarmStaus &= ~(0x01 << index);
				ALM_AlarmOut(alarmStaus);
			}
		}
		break;
	case CHSTATUS_SET_RECQTY:
		if ( Status <0 || Status > 4 )
		{
            return FALSE;
		}

        m_pDSP->SetParam(PT_PCI_SET_ENCODE_BITRATE, index, s_szBitRate[Status]);
        break;
	case CHSTATUS_SET_NETQTY:
        if ( Status <0 || Status > 4 )
        {
            return FALSE;
        }

        m_pDSP->SetParam(PT_PCI_SET_NET_ENCODE_BITRATE, index, s_szBitRate[Status]);
        break;
	case CHSTATUS_SET_RECFRATE:
		if (wFrameRate != 0xFFFF)
		{
			if (m_dwVideoFormat == DVRVIDEO_STANDARD_PAL && wFrameRate > 25)
				wFrameRate = 25;
			else if (wFrameRate > 30)
				wFrameRate = 30;

			//当录像流关闭时，保证网络正常传输	<REC-NET>
			m_pDSP->m_bRecordStop[index] = FALSE;
			if(wFrameRate <= 0)
			{
				wFrameRate = m_dwVideoFormat == DVRVIDEO_STANDARD_PAL?25:30;
				m_pDSP->m_bRecordStop[index] = TRUE;
			}

			if (wFrameRate > 0)
			{
				m_pDSP->SetParam(PT_PCI_SET_ENCODE_ISENCODE, index, 1);
				m_pDSP->SetParam(PT_PCI_SET_ENCODE_FRAMERATE, index, wFrameRate);
			}
			else
			{
				m_pDSP->SetParam(PT_PCI_SET_ENCODE_ISENCODE, index, 0);
			}
		}
		break;
	case CHSTATUS_SET_MOTION_PARAM:
		if (wMotionSens != 0xFFFF)
		{
			if (wMotionSens > 100)
				wMotionSens = 100;
			m_pDSP->SetParam(PT_PCI_SET_MOTION_SENTIVE, index, wMotionSens);
		}

		if (wMotionInter != 0xFFFF)
		{
			if (wMotionInter > 2)
				wMotionInter = 2;
			m_pDSP->SetParam(PT_PCI_SET_MOTION_INTERVAL, index, 2 - wMotionInter);
		}
		break;
	case CHSTATUS_SET_NEXTISKEYFRAME:
		if(!m_pDSP->m_bRecordStop[index])	//录像停止后，将可能有频繁的关键帧请求，忽略	Modify by zhangzhen 2008/06/16
		{
			m_pDSP->SetParam(PT_PCI_SET_IMMED_IFRAME, index, 1);
		}
		break;
	case CHSTATUS_SET_TIMESTAMP:
		m_pDSP->SetParam(PT_PCI_SET_TIME_STAMP, index, Status);
		break;
	case CHSTATUS_SET_AUDIO_ENABLE:
		m_pDSP->SetAudioSwitch(index, Status);
		break;
	case CHSTATUS_SET_NEXTISKEYFRAME_NET:
		m_pDSP->SetParam(PT_PCI_SET_NET_IMMED_IFRAME, index, 1);
		break;
	case CHSTATUS_SET_NEXTISKEYFRAME_MOBILE:
		m_pDSP->SetParam(PT_PCI_SET_MOB_IMMED_IFRAME, index, 1);
		break;
	case CHSTATUS_SET_CALLMON:	//CallMonitor
		{
			if(index != m_nCurrentMonitor)
			{
				if(m_nCurrentMonitor != -1)
				{
					m_pDSP->SetParam(PT_PCI_SET_CALL_MONITOR, m_nCurrentMonitor, 0);
				}

				m_pDSP->SetParam(PT_PCI_SET_CALL_MONITOR, index, 1);
				
				m_nCurrentMonitor = index;
			}
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
	
BOOL CDeviceSDK::DeviceInitial(DWORD VideoFormat, CAPTURECALLBACK *pVideoCallBack, CAPTURECALLBACK *pAudioCallBack, HWND hwnd,DWORD * pProductType)
{
	if (m_bInitialized == TRUE)
		return TRUE;

	m_bNeedExtractDSPFile=TRUE;
    CFile file;
    if (file.Open(s_pOutFileName[VideoFormat], CFile::modeRead))
    {
        m_bNeedExtractDSPFile=FALSE;
        file.Close();
    }

	if (m_bNeedExtractDSPFile)
	{
		if (ExtractFiles(VideoFormat) != DVRERR_SUCCESS)
		{
			return FALSE;
		}
	}
	
	m_pDSP = new CDSP();
	if (!m_pDSP->Initialize(VideoFormat, pVideoCallBack, pAudioCallBack))
	{
		delete m_pDSP;
		m_pDSP = NULL;
		return FALSE;
	}

	m_dwVideoFormat = VideoFormat;
	m_bInitialized = TRUE;
	*pProductType = DVR_CARD_TD4104;

	//WATERMARK	CALLMONITOR
	for (int i = 0; i < 4 * m_pDSP->GetDevNum(); i++)
	{
		m_pDSP->SetParam(PT_PCI_SET_WATER_MARK, i, 0);
		m_pDSP->SetParam(PT_PCI_SET_CALL_MONITOR, i, 0); //初始化Call Monitor状态为全闭
	}

	m_bAlarmCard = ALM_Initial();
	return TRUE;
}
	
void CDeviceSDK::DeviceFree(void)
{
	if (m_bInitialized == FALSE)
		return;

	m_pDSP->Terminate();
	delete m_pDSP;

	if (m_bAlarmCard == TRUE)
	{
		ALM_Quit();
		m_bAlarmCard = FALSE;
	}

	DeleteFiles(m_dwVideoFormat);

	m_bInitialized = FALSE;
}

/******************************************************************************
* 函数名称： BOOL GetVideoStreamInfo(DWORD nStreamtype, DWORD VideoSizeType, BITMAPINFOHEADER *BmpHdr)
* 功能描述： 得到视频流BITMAPINFOHEADER
* 输入参数： nStreamtype	流类型
*			 VideoSizeType  视频尺寸类型
* 输出参数： BmpHdr此视频流的BITMAPINFOHEADER信息
* 返 回 值： 成功(支持此种视频尺寸)返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetVideoStreamInfo(DWORD nStreamtype, DWORD VideoSizeType, BITMAPINFOHEADER *pBmpHdr)
{
	ERROR_RETURN_FALSE(!m_bInitialized);
	
	pBmpHdr->biSize = sizeof(BITMAPINFOHEADER);
	pBmpHdr->biPlanes = 1;
	pBmpHdr->biXPelsPerMeter = 0;
	pBmpHdr->biYPelsPerMeter = 0;
	pBmpHdr->biClrUsed = 0;
	pBmpHdr->biClrImportant = 0;	
	pBmpHdr->biBitCount = 16;
	pBmpHdr->biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');

	if (nStreamtype == VIDEO_STREAM_CAPTURE)
	{
		pBmpHdr->biSize = sizeof(BITMAPINFOHEADER);
		pBmpHdr->biPlanes = 1;
		pBmpHdr->biXPelsPerMeter = 0;
		pBmpHdr->biYPelsPerMeter = 0;
		pBmpHdr->biClrUsed = 0;
		pBmpHdr->biClrImportant = 0;	
		pBmpHdr->biBitCount = 16;
		pBmpHdr->biCompression = MAKEFOURCC('T', 'V', 'T', 'X');
	}

	if (nStreamtype == VIDEO_STREAM_NET)
	{
		pBmpHdr->biSize = sizeof(BITMAPINFOHEADER);
		pBmpHdr->biPlanes = 1;
		pBmpHdr->biXPelsPerMeter = 0;
		pBmpHdr->biYPelsPerMeter = 0;
		pBmpHdr->biClrUsed = 0;
		pBmpHdr->biClrImportant = 0;	
		pBmpHdr->biBitCount = 16;
		pBmpHdr->biCompression = MAKEFOURCC('T', 'V', 'T', 'X');
	}
	
	if (nStreamtype == VIDEO_STREAM_MOBILE )
	{
		pBmpHdr->biSize = sizeof(BITMAPINFOHEADER);
		pBmpHdr->biPlanes = 1;
		pBmpHdr->biXPelsPerMeter = 0;
		pBmpHdr->biYPelsPerMeter = 0;
		pBmpHdr->biClrUsed = 0;
		pBmpHdr->biClrImportant = 0;	
		pBmpHdr->biBitCount = 16;
		pBmpHdr->biCompression = MAKEFOURCC('X', 'V', 'I', 'D');
	}

	//<REC-NET>网络实时流
	if (nStreamtype == VIDEO_STREAM_NET_REALTIME)
	{
		pBmpHdr->biSize = sizeof(BITMAPINFOHEADER);
		pBmpHdr->biPlanes = 1;
		pBmpHdr->biXPelsPerMeter = 0;
		pBmpHdr->biYPelsPerMeter = 0;
		pBmpHdr->biClrUsed = 0;
		pBmpHdr->biClrImportant = 0;	
		pBmpHdr->biBitCount = 16;
		pBmpHdr->biCompression = MAKEFOURCC('T', 'V', 'T', 'X');
	}
	
	switch (VideoSizeType)
	{
	case DVRVIDEO_SIZE_320x240:
		if(nStreamtype == VIDEO_STREAM_MOBILE)
		{
			pBmpHdr->biWidth = 160;
			pBmpHdr->biHeight = 112;
		}
		else
		{
			pBmpHdr->biWidth = 320;
			pBmpHdr->biHeight = 240;
		}
		break;
	case DVRVIDEO_SIZE_640x480:
		if(nStreamtype == VIDEO_STREAM_MOBILE)
		{
			pBmpHdr->biWidth = 160;
			pBmpHdr->biHeight = 112;
		}
		else
		{
			pBmpHdr->biWidth = 640;
			pBmpHdr->biHeight = 480;
		}
		break;
	case DVRVIDEO_SIZE_352x288:
		if(nStreamtype == VIDEO_STREAM_MOBILE)
		{
			pBmpHdr->biWidth = 176;
			pBmpHdr->biHeight = 144;
		}
		else
		{
			pBmpHdr->biWidth = 352;
			pBmpHdr->biHeight = 288;
		}
		break;
	case DVRVIDEO_SIZE_704x576:
		if(nStreamtype == VIDEO_STREAM_MOBILE)
		{
			pBmpHdr->biWidth = 176;
			pBmpHdr->biHeight = 144;
		}
		else
		{
			pBmpHdr->biWidth = 704;
			pBmpHdr->biHeight = 576;
		}
		break;
	default :
		if(nStreamtype == VIDEO_STREAM_MOBILE)
		{
			pBmpHdr->biWidth = 160;
			pBmpHdr->biHeight = 112;
		}
		else
		{
			pBmpHdr->biWidth = 320;
			pBmpHdr->biHeight = 240;
		}
	}
	pBmpHdr->biSizeImage = pBmpHdr->biWidth * pBmpHdr->biHeight * pBmpHdr->biBitCount/8;
	
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetAudioFormatInfo(WAVEFORMATEX *WFrmat)
* 功能描述： 得到声音流WAVEFORMATEX
* 输入参数： 无
* 输出参数： WFrmat此声音流的WAVEFORMATEX信息
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetAudioStreamInfo(WAVEFORMATEX *pWFrmat)
{
	ERROR_RETURN_FALSE(!m_bInitialized);
		
	pWFrmat->wFormatTag      = WAVE_FORMAT_PCM;
	pWFrmat->wBitsPerSample  = 8;
	pWFrmat->nChannels       = 1;
	pWFrmat->cbSize          = 0;
	pWFrmat->nBlockAlign     = pWFrmat->nChannels * pWFrmat->wBitsPerSample / 8;
	pWFrmat->nSamplesPerSec  = 8000;
	pWFrmat->nAvgBytesPerSec = pWFrmat->nBlockAlign * pWFrmat->nSamplesPerSec;
	return TRUE;
}

BOOL CDeviceSDK::VideoCaptureStart(DWORD dwBmpSizeType,BITMAPINFOHEADER RecBmpHeader)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return m_pDSP->CaptureStart();
}

BOOL CDeviceSDK::VideoCaptureStop(void)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	//停止视频，Call Monitor状态为关闭
	if(this->m_nCurrentMonitor != -1)
	{
		m_pDSP->SetParam(PT_PCI_SET_CALL_MONITOR, m_nCurrentMonitor, 0);
		m_nCurrentMonitor = -1;
	}

	return m_pDSP->CaptureStop();
}

BOOL CDeviceSDK::AudioCaptureStart(WAVEFORMATEX WFrmat)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return TRUE;
}

BOOL CDeviceSDK::AudioCaptureStop(void)
{
	ERROR_RETURN_FALSE(!m_bInitialized);
	return TRUE;
}
	
BOOL CDeviceSDK::GetVideoPropertyValue(DWORD ChIndex, VIDEO_PROPER &VideoProperValue)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	VideoProperValue = m_VideoProperty[ChIndex];
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetVideoDefaultPropertyValue(DWORD ChIndex,ENUM_VIDEO_AUDIO_PROPERTY 
prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal)
* 功能描述： 得到视频属性的最大,最小,默认值
* 输入参数： ChIndex通道号
*			 prop视频属性
* 输出参数： minVal最小值
*			 maxVal最大值
*	         DefaultVal默认值
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetVideoDefaultPropertyValue(DWORD ChIndex, ENUM_VIDEO_AUDIO_PROPERTY prop, DWORD &minVal, DWORD &maxVal, DWORD &DefaultVal)
{
	switch(prop)
	{
	case VIDEO_PROPERTY_BRIGHTNESS:
	case VIDEO_PROPERTY_CONTRUST:
	case VIDEO_PROPERTY_HUE:
	case VIDEO_PROPERTY_SATURATION:
	case VIDEO_PROPERTY_AGC:
		minVal = 0;
		maxVal = 255;
		DefaultVal = 128;
		break;
	default:
		SetLastError(ERROR_INVALID_FUNCTION);
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL SetVideoProperValue(DWORD ChIndex, VIDEO_PROPER &VideoProperValue)
* 功能描述： 设置视频属性值VIDEO_PROPER
* 输入参数： ChIndex通道号
*			 VideoProperValue此通道的视频属性值
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SetVideoPropertyValue(DWORD ChIndex,VIDEO_PROPER pVideoProperValue)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	if (ChIndex >= 4 * m_pDSP->GetDevNum())
	{
		return FALSE;
	}

	if (pVideoProperValue.bright > 255)
		pVideoProperValue.bright = 255;
	if (pVideoProperValue.saturation > 255)
		pVideoProperValue.saturation = 255;
	if (pVideoProperValue.hue > 255)
		pVideoProperValue.hue = 255;
	if (pVideoProperValue.contrast > 255)
		pVideoProperValue.contrast = 255;
	if (pVideoProperValue.agc > 255)
		pVideoProperValue.agc = 255;

	m_VideoProperty[ChIndex] = pVideoProperValue;

	//Add By zhangzhen 2007/03/05	修改DSP默认图像属性
//	MapVideoProperByDefault(pVideoProperValue);

	m_pDSP->SetParam(PT_PCI_SET_VIDEO_BRIGHTNESS, ChIndex, pVideoProperValue.bright);
	m_pDSP->SetParam(PT_PCI_SET_VIDEO_SATURATION, ChIndex, pVideoProperValue.saturation);
	m_pDSP->SetParam(PT_PCI_SET_VIDEO_HUE, ChIndex, pVideoProperValue.hue);
	m_pDSP->SetParam(PT_PCI_SET_VIDEO_CONTRAST, ChIndex, pVideoProperValue.contrast);

	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetAudioPropertyValue(DWORD AudioIndex, AUDIO_PROPER &AudioProperValue)
* 功能描述： 得到音频属性值AUDIO_PROPER
* 输入参数： AudioIndex通道号
* 输出参数： AudioProperValue此通道的音频属性值
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetAudioPropertyValue(DWORD AudioIndex,AUDIO_PROPER  &AudioProperValue)
{
	ERROR_RETURN_FALSE(!m_bInitialized);
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetAudioDefaultPropertyValue(DWORD AudioIndex,ENUM_VIDEO_AUDIO_PROPERTY 
prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal)
* 功能描述： 得到音频属性的最大,最小,默认值
* 输入参数： AudioIndex音频通道号
*			 prop音频属性
* 输出参数： minVal最小值
*			 maxVal最大值
*	         DefaultVal默认值
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetAudioDefaultPropertyValue(DWORD AudioIndex, ENUM_VIDEO_AUDIO_PROPERTY prop, DWORD &minVal, DWORD &maxVal, DWORD &DefaultVal)
{
	switch(prop)
	{
		case AUDIO_PROPERTY_AGC:
			minVal = 0;
			maxVal = 255;
			DefaultVal = 128;
			break;
		default:
			SetLastError(ERROR_INVALID_FUNCTION);
			return FALSE;
	}
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL SetAudioPropertyValue(DWORD AudioIndex, AUDIO_PROPER AudioProperValue)
* 功能描述： 设置音频属性值AUDIO_PROPER
* 输入参数： AudioIndex音频通道号
*			 AudioProperValue此通道的视频属性值
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SetAudioPropertyValue(DWORD AudioIndex, AUDIO_PROPER pAudioProperValue)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL SetSwitch(DWORD *Switch, DWORD ChannelNum)
* 功能描述： 设置通道开关
* 输入参数： Switch开关数组，每个通道的开关
*			 ChannelNum数组维数
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SetSwitch(DWORD *pSwitch, DWORD ChannelNum)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return m_pDSP->SetSwitch(pSwitch, ChannelNum);
}

/******************************************************************************
* 函数名称： BOOL SetMotionRect(int index, LPDWORD pByte[], long cnByte)
* 功能描述： 设置移动区域，硬件实现移动时才有意义
* 输入参数： index通道号
*			 pByte每个BYTE的每一位对应图象中的一块区域
*			 cnByte pByte的维数
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SetMotionRect(DWORD index,  BYTE pByte[], long cnByte)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL SetChannelAlarm(DWORD ChannelNum, DWORD pChAlarm[])
* 功能描述： 设置通道报警然后自动倍帧(如果设置为自动倍帧的话)
* 输入参数： ChannelNum pChAlarm数组维数
*			 pChAlarm[]每个通道的报警状态
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::SetChannelAlarm(DWORD ChannelNum, DWORD pChAlarm[])
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	return TRUE;
}

BOOL CDeviceSDK::GetMaxFrameRate(DWORD ChannelNum,BOOL pSwitch[],int pFrameRate [])
{
	int rate;

	switch(m_dwVideoFormat)
	{
	case DVRVIDEO_STANDARD_PAL:
		rate = 25;
		break;
	case DVRVIDEO_STANDARD_NTSC:
		rate = 30;
		break;
	default:
		rate = 25;
	}

	for (int i = 0; i < (int)ChannelNum; i++)
	{
		pFrameRate[i] = rate;
	}
	
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL Password()
* 功能描述： 调用卡的加密接口
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::Password(void)
{
	return m_pDSP->Password();
}

/******************************************************************************
* 函数名称： BOOL ReleaseBuffer()
* 功能描述： 释放使用的缓冲，使它继续加入捕捉的行列
* 输入参数： isVideo如果为非零释放视频缓冲，否则是释放声音缓冲
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::ReleaseBuffer(DWORD isVideo,DWORD DelBufPara)
{
	ERROR_RETURN_FALSE(!m_bInitialized);

	m_pDSP->ReleaseBuffer(isVideo, DelBufPara);
	return TRUE;
}

/******************************************************************************
* 函数名称： BOOL GetQuadRect(DWORD sizetype, DWORD &CamNumPerF, CRect pRect[])
* 功能描述： 当Preview流为硬件分割时，得到每个Camera的分割区域,这儿假定分割区域内的
* 输入参数： sizetype尺寸类型
* 输出参数： CamNumPerF每个画面的Camera数量
*	         pRect每个画面的区域
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 创建日期   版本号    创建人 
* 修改日期   版本号    修改人	 修改内容
* -----------------------------------------------
******************************************************************************/
BOOL CDeviceSDK::GetQuadRect(DWORD sizetype,DWORD &CamNumPerF,CRect pRect[])
{
	return TRUE;
}

// FreeResource, UnlockResource is not necessary use in win32, only win16  
int CDeviceSDK::ExtractFiles(DWORD dwVideoFormat)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	PBYTE pData;
	DWORD dwSize;
	CFile file;
	//CString strName;
	CFileStatus status;

	DeleteFiles(dwVideoFormat);
//.OUT

    hResInfo = ::FindResource(m_hDllModule, s_pStandardName[dwVideoFormat], TEXT("OUTFILE"));
	if (hResInfo == NULL)
	{
		TRACE("No .Out Resource\n");
		return DVRERR_FAIL;
	}

	hResData = ::LoadResource(m_hDllModule, hResInfo);
	if (hResData == NULL)
	{
		return DVRERR_FAIL;
	}

	pData = (PBYTE)::LockResource(hResData);
	if (pData == NULL)
	{
		return DVRERR_FAIL;
	}

	dwSize = ::SizeofResource(m_hDllModule, hResInfo);
	if (file.Open(s_pOutFileName[dwVideoFormat], CFile::modeCreate | CFile::modeWrite) == 0)
	{
		TRACE("Create .Out Fail\n");
		return DVRERR_FAIL;
	}

	file.Write(pData, dwSize);
	file.Close();

	CFile::GetStatus(s_pOutFileName[dwVideoFormat], status);
	status.m_attribute |= 0x02;
	CFile::SetStatus(s_pOutFileName[dwVideoFormat], status);
//.OUT AlarmCard.dll

	return DVRERR_SUCCESS;
}

int CDeviceSDK::DeleteFiles(DWORD dwVideoFormat)
{
	if (!m_bNeedExtractDSPFile)
	{
		return DVRERR_SUCCESS;
	}
	if (dwVideoFormat == DVRVIDEO_STANDARD_PAL)
	{
		SetFileAttributes(OUT_FILE_NAME_PAL, FILE_ATTRIBUTE_ARCHIVE);
		DeleteFile(OUT_FILE_NAME_PAL);
	}
	else
	{
		SetFileAttributes(OUT_FILE_NAME_NTSC, FILE_ATTRIBUTE_ARCHIVE);
		DeleteFile(OUT_FILE_NAME_NTSC);
	}

	return DVRERR_SUCCESS;
}

/*
函数功能: 将AP层的图像属性域映射到DSP层的域(都为[0,255]，但两层中取不同的默认值)
参数说明: vp[IN/OUT]---需要转换的图像属性结构,并由它返回转换后的图像属性
返 回 值: 空

 修改日期   版本号    修改人	 修改内容
------------------------------------------------
*/
void  CDeviceSDK::MapVideoProperByDefault(VIDEO_PROPER& vp)
{
//	return;
/*
宏MAP_DEFAULT_VIDEO_PROPER
参数:
x		: 需要转换的目标值
ls(ld)	: 源(目标)域下限
hs(hd)	: 源(目标)域上限
ds(dd)	: 源(目标)域默认值

算法(假设转换后的值为t):
	x<ds时	ls	x	ds	hs
			ld	t	dd	hd
			(x-ls)/(t-ld) = (ds-ls)/(dd-ld)
			=>t = (x-ls)*(dd-ld)/(ds-ls)+ld

	x>dd时	ls	ds	x	hs
			ld	dd	t	hd
			(x-ds)/(t-dd) = (hs-ds)/(hd-dd)
			=>t = (x-ds)*(hd-dd)/(hs-ds)+dd
*/
#define	MAP_DEFAULT_VIDEO_PROPER(x, ls, hs, ds, ld, hd, dd)	\
	{\
		if((x) <= ds)	{(x) = ((x)-(ls))*((dd)-(ld))/((ds)-(ls))+(ld);}\
		else			{(x) = ((hd)-(dd))*((x)-(ds))/((hs)-(ds))+(dd);}\
	}
	
	MAP_DEFAULT_VIDEO_PROPER(vp.bright, 0, 255, 128, 0, 255, m_default_Video_Proper.bright);
	MAP_DEFAULT_VIDEO_PROPER(vp.saturation, 0, 255, 128, 0, 255, m_default_Video_Proper.saturation);
	MAP_DEFAULT_VIDEO_PROPER(vp.hue, 0, 255, 128, 0, 255, m_default_Video_Proper.hue);
	MAP_DEFAULT_VIDEO_PROPER(vp.contrast, 0, 255, 128, 0, 255, m_default_Video_Proper.contrast);
	MAP_DEFAULT_VIDEO_PROPER(vp.agc, 0, 255, 128, 0, 255, m_default_Video_Proper.agc);
}



// End of File
