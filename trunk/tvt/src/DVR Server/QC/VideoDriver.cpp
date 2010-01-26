/************************************************************************
* 版权所有 (C)2004, 深圳市同为视频技术有限公司。
* 
* 文件名称： VideoDriver.cpp
* 内容摘要： 控制卡设备(Driver)视频部分的基类，主要做一些不区别具体卡类型的视频操作
* 当前版本： 1.0
* 作    者： 陈士光
* 完成日期： 
* 
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#include "stdafx.h"
#include "VideoDriver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int g_td3016_convert_ch[32]={4-1,8-1,12-1,16-1,3-1,7-1,11-1,15-1,2-1,6-1,10-1,14-1,1-1,5-1,9-1,13-1,   4+16-1,8+16-1,12+16-1,16+16-1,3+16-1,7+16-1,11+16-1,15+16-1,2+16-1,6+16-1,10+16-1,14+16-1,1+16-1,5+16-1,9+16-1,13+16-1 };
int g_td3008_convert_ch[32]={1,3,5,7,0,2,4,6}; //by chenlong

CVideoDriver::CVideoDriver()
{
	//CDeviceDriver::CDeviceDriver(); heliang -
	m_dwFrameIndex = 0;
	m_dwFrameIndexRel = 0;
	m_dwVideoFormat = DVRVIDEO_STANDARD_PAL;
	m_pBufFlagAddr = NULL;
	m_hVideoCaptureThread = NULL;
	m_bVideoCapturing = FALSE;
	m_pVideoCallBack = NULL;
	m_nDeviceIndex = -1;
	m_nDeviceNum = 0;
		
	m_pVideoCallBack = NULL;
	for(int i = 0; i < 16; i++)
	{
		m_VideoProperty[i].contrast = 3295;
		m_VideoProperty[i].saturation = 5000;
		m_VideoProperty[i].bright = 5000;
		m_VideoProperty[i].agc = -1;
		m_VideoProperty[i].hue = 5000;
	}
}

CVideoDriver::~CVideoDriver()
{
	m_pBufFlagAddr = NULL;
	m_hVideoCaptureThread = NULL;
	m_bVideoCapturing = FALSE;
	CDeviceDriver::~CDeviceDriver();
}


/******************************************************************************
* 函数名称： BOOL InitializeDriver()
* 功能描述： 初始化Driver
* 输入参数： 无
* 输出参数： 无
* 返 回 值：成功返回TRUE，否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::InitializeDriver(long DeviceIndex, DWORD dwVideoFormat, CAPTURECALLBACK *pVideoCallBack)
{
	TRACE("InitializeDriver()\n");
	if(NULL == pVideoCallBack) 
	{
		SetLastError(DVRERR_INVALIDPARA);
		return FALSE;
	}
	m_dwVideoFormat = dwVideoFormat;
	m_pVideoCallBack = pVideoCallBack;
	m_nDeviceIndex = DeviceIndex;
	BOOL ret = CDeviceDriver::InitializeDriver();
	if(!ret) 
	{
		AfxMessageBox("InitializeDriver fail!");
		return FALSE;
	}

	m_nDeviceNum = GetDeviceNumber();
	if(0 == m_nDeviceNum) 
	{
		AfxMessageBox("GetDeviceNumber fail!");
		CDeviceDriver::ExitDriver();
		SetLastError(DVRERR_DRIVERGETDEVNUM);
		return FALSE;
	}

	APP_DRIVER_BUFFER_INFO dbufIn;
	APP_DRIVER_BUFFER_INFO dbufOut;
	memset(&dbufIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&dbufOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	dbufIn.m_enumCurrentOPType = ENUM_CHIP_INITIALIZE;
	dbufIn.m_hEvent = m_hCaptureEvent;

	ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
						&dbufIn,  
						sizeof(APP_DRIVER_BUFFER_INFO),
						&dbufOut, 
						sizeof(APP_DRIVER_BUFFER_INFO));
	if(ret)
	{
		m_pBufFlagAddr = (LPBYTE)dbufOut.m_ulRetuBufFlagAddress;
	}
	else
	{
		CDeviceDriver::ExitDriver();
		m_pBufFlagAddr = NULL;	
		SetLastError(DVRERR_DRIVERINITIALIZE);

		AfxMessageBox("IOCTL_CONTROL_CHIP_SETTING fail!");
	}
	GetDefaultOrder(m_dwChannelNum, m_dwChannelSwitch);
	return ret;
}

/******************************************************************************
* 函数名称： void ExitDriver()
* 功能描述： 退出Driver
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

void CVideoDriver::ExitDriver()
{
	TRACE("ExitDriver()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_CHIP_FINISH;

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
							 &stIn, 
							 sizeof(APP_DRIVER_BUFFER_INFO),
							 NULL, 
							 0);
	m_nDeviceIndex = -1;
	CDeviceDriver::ExitDriver();
}

/******************************************************************************
* 函数名称： BOOL StartCapture(DVRENUM_VIDEO_SIZE_TYPE sizeType)
* 功能描述： 开始捕捉视频,生成视频捕捉线程
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::StartCapture(DWORD sizeType)
{
	TRACE("StartCapture()\n");
	DWORD threadid;

	if(m_nDeviceIndex < 0) 
	{
		SetLastError(DVRERR_DRIVERMUSTINIT);
		return FALSE;
	}

	m_bVideoCapturing = TRUE;
	m_hVideoCaptureThread = CreateThread(0, 0, VideoCaptureThread, this, 0, &threadid);
	if(NULL == m_hVideoCaptureThread)
	{
		m_bVideoCapturing = FALSE;
		return FALSE;
	}

	APP_DRIVER_BUFFER_INFO DrvBuf;
	memset(&DrvBuf, 0, sizeof(APP_DRIVER_BUFFER_INFO) );

	DrvBuf.m_enumCurrentOPType = ENUM_VIDEO_CAPTURE_START;

	switch(m_dwVideoFormat)
	{
	case DVRVIDEO_STANDARD_NTSC:
		DrvBuf.m_ulVideoStandard = 1;
		break;
	case DVRVIDEO_STANDARD_PAL:
		DrvBuf.m_ulVideoStandard = 3;
		break;
	default:
		SetLastError( DVRERR_VIDEOSTANDARD );
		return FALSE;
		break;
	}
	
	DrvBuf.m_ulDefaultMUXValue = 0;
	DrvBuf.m_ulVideoDepth = 2;

	switch(sizeType)
	{
	case DVRVIDEO_SIZE_320x240:
		DrvBuf.m_ulVideoSizeType = TVT_VIDEO_IMAGE_SIZE_320240;
		break;
	case DVRVIDEO_SIZE_640x480:
		DrvBuf.m_ulVideoSizeType = TVT_VIDEO_IMAGE_SIZE_640480;
		break;
	case DVRVIDEO_SIZE_640x240:
		DrvBuf.m_ulVideoSizeType = TVT_VIDEO_IMAGE_SIZE_640240;
		break;
	case DVRVIDEO_SIZE_640x480SW:
		DrvBuf.m_ulVideoSizeType = TVT_VIDEO_IMAGE_SIZE_640480SW;
		break;
	case DVRVIDEO_SIZE_352x288:
		DrvBuf.m_ulVideoSizeType = TVT_VIDEO_IMAGE_SIZE_352288;
		break;
	default:
		SetLastError(DVRERR_INVALIDPARA);
		return FALSE;
	}

	DrvBuf.m_bVideoRealTimeFlag = FALSE;
	DrvBuf.m_ulCallMonitorChannel = 0;

	DWORD dwChannelNum;
	DWORD dwChannelSwitch[30];
	GetDefaultOrder(dwChannelNum, dwChannelSwitch);
	for(DWORD i = 0; i < dwChannelNum; i++)
	{
		DrvBuf.m_ulChannel[i] = dwChannelSwitch[i];
	}
	DrvBuf.m_ulChannelNumber = dwChannelNum;
	
	switch (m_nDeviceIndex)
	{
	case 0:
		DrvBuf.m_ulC51OpValue[27] = 3;
		DrvBuf.m_ulC51OpValue[25] = 10800;
		DrvBuf.m_ulC51OpValue[26] = 11000;
		break;
	case 1:
		DrvBuf.m_ulC51OpValue[27] = 3;
		DrvBuf.m_ulC51OpValue[25] = 10800;
		DrvBuf.m_ulC51OpValue[26] = 11000;
		break;
	case 2:
		DrvBuf.m_ulC51OpValue[27] = 3;
		DrvBuf.m_ulC51OpValue[25] = 10900;
		DrvBuf.m_ulC51OpValue[26] = 11500;
		break;
	default:
		DrvBuf.m_ulC51OpValue[27] = 4;
		DrvBuf.m_ulC51OpValue[25] = 11200;
		DrvBuf.m_ulC51OpValue[26] = 11700;
		break;
	}
	DrvBuf.m_ulC51OpValue[20] = 0;
	DrvBuf.m_ulC51OpValue[28] = 4600;

	if (1 == DrvBuf.m_ulVideoStandard)
	{
		DrvBuf.m_ulC51OpValue[29] = 3800;
	}
	else
	{
		DrvBuf.m_ulC51OpValue[29] = 3900;
	}
	
	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
							 &DrvBuf, 
							 sizeof(APP_DRIVER_BUFFER_INFO),
							 NULL, 
							 0);
	if(!ret)
	{
		SetLastError( DVRERR_DRIVERSTART );
	}
	return ret;
}

/******************************************************************************
* 函数名称： BOOL StopCapture()
* 功能描述： 结束捕捉视频,退出视频捕捉线程
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::StopCapture()
{
	TRACE("StopCapture()\n");
	if( !m_bVideoCapturing) return FALSE;

	m_bVideoCapturing = FALSE;

	SetEvent(m_hCaptureEvent);
	if(NULL != m_hVideoCaptureThread)
	{
		WaitForSingleObject(m_hVideoCaptureThread, INFINITE);
		CloseHandle(m_hVideoCaptureThread);
		m_hVideoCaptureThread = NULL;
	}

	APP_DRIVER_BUFFER_INFO DrvBuf;
	memset(&DrvBuf, 0, sizeof(APP_DRIVER_BUFFER_INFO) );

	DrvBuf.m_enumCurrentOPType = ENUM_VIDEO_CAPTURE_STOP;

	BOOL ret = ControlDriver( IOCTL_CONTROL_CHIP_SETTING,
							  &DrvBuf, sizeof(APP_DRIVER_BUFFER_INFO),
							  NULL, 0);
	if(!ret)
	{
		SetLastError( DVRERR_DRIVERSTOP );
	}

	return ret;
}

DWORD WINAPI CVideoDriver::VideoCaptureThread(LPVOID lpParameter)
{
	CVideoDriver *pVideoDriver = (CVideoDriver *)lpParameter;		
	return pVideoDriver->VideoCaptureFunc();
}

/******************************************************************************
* 函数名称： BOOL VideoCaptureFunc()
* 功能描述： 视频捕捉线程,获得事件就可以从Driver取到数据
*			 有可能正在获取的时候,来了另一帧所以while(GetOneFrameData());
*			 取完数据后将事件置为无信号状态,等待下一次数据
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 直到停止捕捉线程才退出,返回0(正常退出线程)
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

DWORD CVideoDriver::VideoCaptureFunc()
{
	TRACE("VideoCaptureFunc()\n");
	while(m_bVideoCapturing)
	{
		DWORD dwResult = WaitForSingleObject(m_hCaptureEvent, INFINITE);
		ResetEvent(m_hCaptureEvent);
		if (WAIT_OBJECT_0 == dwResult)
		{
			while(GetOneFrameData())
			{
				if(!m_bVideoCapturing)
				{
					break;
				}
			};//不停的获取数据直到缓冲无数据
		}
	}
	return 0;
}

/******************************************************************************
* 函数名称： BOOL GetOneFrameData()
* 功能描述： 获取一帧数据
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 如果成功获取一帧数据返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::GetOneFrameData()
{
	TVT_VIDEO_EACH_BUF_INFO stOut;
	FRAMEBUFSTRUCT videoFrame;

	memset(&stOut, 0, sizeof(TVT_VIDEO_EACH_BUF_INFO));
	BOOL ret = ControlDriver(IOCTL_VIDEO_GET_DATA_INFO, 
							NULL, 
							0,
							&stOut, 
							sizeof(TVT_VIDEO_EACH_BUF_INFO));
	if (ret)
	{
//		DWORD DelBuf=MAKELPARAM(stOut.m_uChipNumber, stOut.m_uBufNumber);
		DWORD DelBuf = MAKELPARAM(m_nDeviceIndex, stOut.m_ulCurrentRiscNumber);
		videoFrame.localTime	= stOut.m_largeLocalTime.QuadPart;
		videoFrame.pBuf			= (LPBYTE)stOut.m_ulBufUserAddress;
		videoFrame.BufLen		= stOut.m_ulBufLen;
		videoFrame.nStreamID	= VIDEO_STREAM_PREVIEW;
		videoFrame.nVLostFlag	= stOut.m_ucLostFlag;
		//非压缩数据肯定是关键帧，必须这样
		videoFrame.bIsKeyFrame	= 1;
		videoFrame.BufferPara	= DelBuf;
		videoFrame.FrameTime	= stOut.m_ullTime;
		videoFrame.ChannelIndex	= GetChannelIndex(stOut.m_uChipNumber, stOut.m_uBufNumber);
		m_dwFrameIndex++;
		if(m_pVideoCallBack)
			m_pVideoCallBack(&videoFrame);
	}
	return ret;
}

/******************************************************************************
* 函数名称： BOOL ReleaseBuffer(DWORD bufParam)
* 功能描述： 释放Buf指针,准备继续抓数据
* 输入参数： bufParam删除Buf用到的参数
* 输出参数： 无
* 返 回 值： 无
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::ReleaseBuffer(DWORD bufParam)
{
	DWORD BufIndex = bufParam;

	LockDelBuf();

	PTVT_VIDEO_FRAME_INFO pVideoBufFlag = (PTVT_VIDEO_FRAME_INFO)m_pBufFlagAddr;
	if (pVideoBufFlag && BufIndex >= 0 && BufIndex < TVT_VIDEO_CHIP_BUFFER_LEN)
	{
		pVideoBufFlag->m_stBufFlag.m_stBufferUsedFlag[BufIndex].m_ulBufferIsUsedFlag = 0;
		
		m_dwFrameIndexRel++;
		UnlockDelBuf();
		return TRUE;
	}
	else
	{
		UnlockDelBuf();
		return FALSE;
	}
}

/******************************************************************************
* 函数名称： long GetDeviceNumber()
* 功能描述： 得到视频芯片数量
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 视频芯片数量
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

long CVideoDriver::GetDeviceNumber()
{
	TRACE("GetDeviceNumber()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&stOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_GET_CHIP_NUMBER;

	BOOL ret = ControlDriver(
					IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, 
					sizeof(APP_DRIVER_BUFFER_INFO),
					&stOut, 
					sizeof(APP_DRIVER_BUFFER_INFO));

	return stOut.m_ulChipTotalNumber;
}

/******************************************************************************
* 函数名称： BOOL GPIORead(long lBeg, long lEnd, DWORD &lVal)
* 功能描述： 读GPIO,有些卡的Driver不支持GPIO操作，但也不会执行此函数
* 输入参数： lBeg要读的GPIO的开始位
*			 lEnd要读的GPIO的结束位
* 输出参数： lVal得到的GPIO从位lBeg到lEnd的值
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 其他说明:	 GPIO(General Purpose Input Output)通用输入/输出
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::GPIORead(long lBeg, long lEnd, DWORD &lVal)
{
	TRACE("GPIORead()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	m_GPIOSecIn.Lock();
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&stOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_READ_GPIO_VALUE;

	stIn.m_ulBeginBit = lBeg;
	stIn.m_ulBitLen = lEnd - lBeg + 1;

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					&stOut, sizeof(APP_DRIVER_BUFFER_INFO));
	lVal = stOut.m_ulGPIOValue;
	m_GPIOSecIn.Unlock();
	return ret;
}

/******************************************************************************
* 函数名称： BOOL GPIOWrite(long lBeg, long lEnd, long lVal)
* 功能描述： 写GPIO,有些卡的Driver不支持GPIO操作，但也不会执行此函数
* 输入参数： lBeg要写的GPIO的开始位
*			 lEnd要写的GPIO的结束位
*			 lVal要写的值
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 其他说明:	 GPIO(General Purpose Input Output)通用输入/输出
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::GPIOWrite(long lBeg, long lEnd, long lVal)
{
	TRACE("GPIOWrite()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	m_GPIOSecIn.Lock();
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&stOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_WRITE_GPIO_VALUE;

	stIn.m_ulBeginBit = lBeg;
	stIn.m_ulBitLen = lEnd - lBeg + 1;
	stIn.m_ulGPIOValue = lVal;
	BOOL ret= ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					&stOut, sizeof(APP_DRIVER_BUFFER_INFO));
	m_GPIOSecIn.Unlock();
	return ret;
}

/******************************************************************************
* 函数名称： BOOL CallMonitor(long VideoIndex)
* 功能描述： 设置CallMonitor的视频通道
* 输入参数： VideoIndex视频通道
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 其他说明:	 此处功能默认不支持，如果某种卡支持，自己重载此函数
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::CallMonitor(long VideoIndex)
{
	TRACE("CallMonitor()\n");
	SetLastError(ERROR_INVALID_FUNCTION);
	return FALSE;
}

/******************************************************************************
* 函数名称： BOOL GetSignalStatus(DWORD *pSignalStatus, long statusNum)
* 功能描述： 得到信号状态有没有丢失
* 输入参数： statusNum数组pSignalStatus的维数
* 输出参数： pSignalStatus每个通道的信号状态
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::GetSignalStatus(DWORD *pSignalStatus, long statusNum)
{
	TRACE("GetSignalStatus()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&stOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_GET_VIDEO_LOST_STATUS;

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					&stOut, sizeof(APP_DRIVER_BUFFER_INFO));
	for(int i = 0; i < statusNum; i++)
	{
		pSignalStatus[i] = stOut.m_ulSignalState[i];
	}
	return ret;
}

/******************************************************************************
* 函数名称： BOOL SetVideoVideoProper(long ChIndex, VIDEO_PROPER videoProper)
* 功能描述： 设置视频属性值亮度、对比度、色度等
* 输入参数： ChIndex通道序号
*			 videoProper属性值
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 其他说明： 此函数没有整合总体通道概念，只是设置某个芯片的某个通道
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::SetVideoPropertyValue(long ChIndex, VIDEO_PROPER videoProper)
{
	TRACE("SetVideoPropertyValue()\n");
	m_VideoProperty[ChIndex] = videoProper;
	APP_DRIVER_BUFFER_INFO stIn;

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_SET_VIDEO_IMAGE_PROP;

	stIn.m_ulImageChannelIndex = ChIndex;
	stIn.m_ulImageBrightness = videoProper.bright;
	if(stIn.m_ulImageBrightness == 10000)
		stIn.m_ulImageBrightness -= 1;

	stIn.m_ulImageContrast = videoProper.contrast;
	if(stIn.m_ulImageContrast == 10000)
		stIn.m_ulImageContrast -= 1;

	stIn.m_ulImageHue = videoProper.hue;
	if(stIn.m_ulImageHue == 10000)
		stIn.m_ulImageHue -= 1;
	
	stIn.m_ulImageSaturation = videoProper.saturation;
	if(stIn.m_ulImageSaturation == 10000)
		stIn.m_ulImageSaturation -= 1;

	if(m_dwChannelNum <= 1)
	{
		stIn.m_ulImageAGCGain = 0x22f0;
	}
	else
	{
		if(0xffffffff == videoProper.agc)
			stIn.m_ulImageAGCGain = 0x22f0;
		else
			stIn.m_ulImageAGCGain = 0x2300 | (videoProper.agc * 255 / 10000);
	}

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					NULL, 0);
	return ret;
}

/******************************************************************************
* 函数名称： BOOL GetVideoVideoProper(long ChIndex, VIDEO_PROPER videoProper)
* 功能描述： 获得视频属性值亮度、对比度、色度等
* 输入参数： ChIndex通道序号
* 输出参数： videoProper属性值
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 其他说明： 此函数没有整合总体通道概念，只是设置某个芯片的某个通道
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::GetVideoPropertyValue(long ChIndex, VIDEO_PROPER &videoProper)
{
	TRACE("GetVideoPropertyValue()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	APP_DRIVER_BUFFER_INFO stOut;

	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
	memset(&stOut, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_GET_VIDEO_IMAGE_PROP;
	stIn.m_ulImageChannelIndex = ChIndex;

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					&stOut, sizeof(APP_DRIVER_BUFFER_INFO));

	videoProper.bright = stOut.m_ulImageBrightness ;
	videoProper.contrast = stOut.m_ulImageContrast ;
	videoProper.hue = stOut.m_ulImageHue ;
	videoProper.saturation = stOut.m_ulImageSaturation ;

	if(0x22f0 == stOut.m_ulImageAGCGain)
		videoProper.agc = 0xffffffff;
	else
		videoProper.agc = stOut.m_ulImageAGCGain * 10000 / 255;

	return ret;
}

/******************************************************************************
* 函数名称： BOOL Password()
* 功能描述： 板卡加密功能
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 有加密功能且成功解密返回TRUE,否则返回FALSE
* 其他说明： 此函数没有整合总体通道概念，只是设置某个芯片的某个通道
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::Password()
{
	TRACE("Password()\n");
	APP_DRIVER_BUFFER_INFO stIn;

	memset( &stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO) );
	stIn.m_enumCurrentOPType = ENUM_GET_ENCRYPT;
	
	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING, 
					&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
					NULL, 0);
	return ret;
}

/******************************************************************************
* 函数名称： BOOL ResetComputer(DWORD dwStart)
* 功能描述： Auto Reset功能
* 输入参数： dwStart为0表示结束Auto Reset功能计时,并退出AutoReset功能
*				   为1表示开始Auto Reset功能计时
*				   为2表示重新计时开始
* 输出参数： 无
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::ResetComputer(DWORD dwStart)
{
	TRACE("ResetComputer()\n");
	m_GPIOSec.Lock();
	BOOL ret;
	switch(dwStart)
	{
	case 0:
		ret = SetC51AddressValue(22, 0xf3);
		break;
	case 1:
		ret = SetC51AddressValue(2,  0xf2);
		break;
//	case 2:
//		ret = SetC51AddressValue( 30, 55 );
//		break;
	}
	m_GPIOSec.Unlock();
	return ret;
}

/******************************************************************************
* 函数名称： BOOL SetC51AddressValue(long lAddress, long lValue)
* 功能描述： 设置C51地址值,这里主要用来和单片机通信
* 输入参数： lAddress C51地址
*			 lValue   lAddress地址要设置的值
* 输出参数： 无
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::SetC51AddressValue(long lAddress, long lValue)
{
	TRACE("SetC51AddressValue()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_SET_C51_ADDRESS_VALUE;
	stIn.m_ulC51OpByteOrBlock = 0;
	stIn.m_ulC51OpSubAddress = lAddress;
	stIn.m_ulC51OpValue[0] = lValue;
	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
							&stIn, sizeof(APP_DRIVER_BUFFER_INFO), 
							NULL, 0);
	return ret;
}

/******************************************************************************
* 函数名称： BOOL SetC51BlockValue(long lAddress,long *lValue,long nLen)
* 功能描述： 设置C51地址值,这里主要用来和单片机通信
* 输入参数： lAddress C51地址
*			 lValue   lAddress地址要设置的值
*			 nLen	  lValue的维数		
* 输出参数： 无
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::SetC51BlockValue(long lAddress,ULONG *lValue,long nLen)
{
	TRACE("SetC51BlockValue()\n");
	APP_DRIVER_BUFFER_INFO stIn;
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUM_SET_C51_ADDRESS_VALUE;
	stIn.m_ulC51OpByteOrBlock = 1;
	stIn.m_ulC51OpSubAddress = lAddress;
	stIn.m_ulC51OpValueCount = nLen;

	for(int i = 0; i < nLen; i++)
		stIn.m_ulC51OpValue[i] = lValue[i];

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
						&stIn, sizeof(APP_DRIVER_BUFFER_INFO), 
						NULL, 0);

	return ret;
}

/******************************************************************************
* 函数名称： BOOL SetResetComputerTime(long SecTime)
* 功能描述： 设置Auto Reset功能的时间
* 输入参数： SecTime多少秒数程序没反应就自动Reset
* 输出参数： 无
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::SetResetComputerTime(long SecTime)
{
	TRACE("SetResetComputerTime()\n");
	BOOL ret = SetC51AddressValue(23, SecTime);
	if (ret)
	{
		Sleep(300);
		ret = SetC51AddressValue(22, 0xf4);
	}
	return ret;
}

/******************************************************************************
* 函数名称： BOOL SetChannelOrder(long ChannelCount, long *pChannelIndex)
* 功能描述： 设置卡的通道切换顺序
* 输入参数： ChannelCount切换的Camera数量
*			 pChannelIndex切换的Camera 序号
* 输出参数： 无
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CVideoDriver::SetChannelOrder(DWORD ChannelCount, DWORD *pChannelIndex)
{
	TRACE("SetChannelOrder()\n");
	if(!m_hVideoCaptureThread) 
		return FALSE;
	
	//保存下来，StartCapture时使用
	m_dwChannelNum = ChannelCount;
    DWORD i;
	for(i = 0; i < m_dwChannelNum; i++)
		m_dwChannelSwitch[i] = pChannelIndex[i];

	APP_DRIVER_BUFFER_INFO stIn;
	memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));

	stIn.m_enumCurrentOPType = ENUN_SET_CHANNEL_TURN_ORDER;
	stIn.m_ulChannelNumber = ChannelCount;

	for(i = 0; i < ChannelCount; i++)
	{		
		if(Is6800())		//从切换到实时时要重新设定一次增益值
			SetVideoPropertyValue(pChannelIndex[i], m_VideoProperty[pChannelIndex[i]]);		
		stIn.m_ulChannel[i] = pChannelIndex[i];
	}

	BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
							&stIn, sizeof(APP_DRIVER_BUFFER_INFO),
							NULL, 0);
	return ret;
}

BOOL CVideoDriver::SetChannelOrder3016(int nindex, DWORD ChannelCount, DWORD *pChannelIndex)
{
    TRACE("SetChannelOrder()\n");
    if(!m_hVideoCaptureThread) 
        return FALSE;
    
    //保存下来，StartCapture时使用
    m_dwChannelNum = ChannelCount;
    DWORD i;
    for(i = 0; i < m_dwChannelNum; i++)
        m_dwChannelSwitch[i] = pChannelIndex[i];
    
    APP_DRIVER_BUFFER_INFO stIn;
    memset(&stIn, 0, sizeof(APP_DRIVER_BUFFER_INFO));
    
    stIn.m_enumCurrentOPType = ENUN_SET_CHANNEL_TURN_ORDER;
    stIn.m_ulChannelNumber = nindex * 4;
    
    for(i = 0; i < ChannelCount; i++)
    {		
        if(Is6800())		//从切换到实时时要重新设定一次增益值
            SetVideoPropertyValue(pChannelIndex[i], m_VideoProperty[pChannelIndex[i]]);		
        stIn.m_ulChannel[4 * nindex + i] = pChannelIndex[i];
    }
    
    BOOL ret = ControlDriver(IOCTL_CONTROL_CHIP_SETTING,
        &stIn, sizeof(APP_DRIVER_BUFFER_INFO),
        NULL, 0);
    return ret;
} 

int CVideoDriver::Check_6800_6805_Channel(int index)
{
	if (m_iTD30xxSubType==TD3016_SUB_TYPE_6805)
		return g_td3016_convert_ch[index];
	if (m_iTD30xxSubType==TD3008_SUB_TYPE_6805)
		return g_td3008_convert_ch[index];
	else 
		return index;
}


