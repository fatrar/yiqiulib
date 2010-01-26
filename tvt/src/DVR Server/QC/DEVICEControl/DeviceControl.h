#ifndef _DEVICE_CONTROL_H
#define _DEVICE_CONTROL_H

#ifndef _WAVEFORMATEX_
	#pragma message("_WAVEFORMATEX_ undefined. Please first include vfw.h")
#endif

#ifdef DEVICECONTROL_EXPORTS
#define DEVICECONTROL_API __declspec(dllexport)
#else
#define DEVICECONTROL_API __declspec(dllimport)
#endif

#define DVRERR_BASE_NUM			0x20000000
#define MAKEDVRERRCODE(x)		DVRERR_BASE_NUM+x

#define DVRERR_SUCCESS			MAKEDVRERRCODE(0)
#define DVRERR_FAIL				MAKEDVRERRCODE(1)	
#define DVRERR_DRIVERINITIALIZE	MAKEDVRERRCODE(2)//Driver 初始化错误
#define DVRERR_DRIVERSTART		MAKEDVRERRCODE(3)//Driver 启动捕捉错误
#define DVRERR_DRIVERSTOP		MAKEDVRERRCODE(4)//Driver 退出捕捉错误
#define DVRERR_VIDEOSTANDARD	MAKEDVRERRCODE(5)//不支持此制式错误
#define DVRERR_DRIVERMUSTINIT	MAKEDVRERRCODE(6)//Driver 必须初始化才能进行操作
#define DVRERR_DRIVERGETDEVNUM	MAKEDVRERRCODE(7)//得到设备数量错误
#define DVRERR_NODVRCARD		MAKEDVRERRCODE(8)//没卡
#define DVRERR_INVALIDPARA		MAKEDVRERRCODE(9)//参数错误
#define DVRERR_VERSION			MAKEDVRERRCODE(10)//此版本不能支持此卡的数量

//型号定义:
#define TD4304   10
#define TD4308   11
#define TD4316	 12
#define TD4404	 13
#define TD4408	 14
#define TD4416	 15
#define TD4404S	 16
#define TD4408S	 17
#define TD4416S	 18
#define UNKNOW   0xffff

//销售区域定义
#define _CHINA_TVT	       1
#define _OVERSEA_NEUTRAL   2

//声音Buffer长度
#define INP_BUFFER_SIZE 4000

//视频制式
typedef enum _DVRENUM_VIDEO_STANDARD_TYPE
{
	DVRVIDEO_STANDARD_PAL = 0x0000,
	DVRVIDEO_STANDARD_NTSC = 0x0001
}DVRENUM_VIDEO_STANDARD_TYPE;
/*
//视频尺寸
typedef enum _DVRENUM_PRODUCT_TYPE
{
	DVR_CARD_TDNULL,
	DVR_CARD_TD3004,
 	DVR_CARD_TD3016,
	DVR_CARD_TD3116,
	DVR_CARD_TD3216,
	DVR_CARD_TD3108,
	DVR_CARD_TD3008,
	DVR_CARD_TD3316,
	DVR_CARD_TD3101,
	DVR_CARD_TD3104,
	DVR_CARD_TD4104,
	DVR_CARD_TD3316V1,
	DVR_CARD_TD4116,
	DVR_CARD_TD4108,
//tony4408 2008-12-04
	DVR_CARD_TD4408,
	DVR_CARD_TD3304,
	DVR_CARD_TD4404,
	DVR_CARD_TD4308,
	DVR_CARD_TD4316

}DVRENUM_PRODUCT_TYPE;
*/
//tony4408 2008-12-04
//4408
//add 4308 2009/02/27														
#define IS_DSP_PRODUCT(product_type)	(((product_type)==DVR_CARD_TD4104)||((product_type)==DVR_CARD_TD4104i)||((product_type)==DVR_CARD_TD4116)||((product_type)==DVR_CARD_TD4108)||((product_type)==DVR_CARD_TD4408)||((product_type)==DVR_CARD_TD4308))//4408

//视频尺寸
typedef enum _DVRENUM_VIDEO_SIZE_TYPE
{
	DVRVIDEO_SIZE_NONE		= 0x0000,
	DVRVIDEO_SIZE_320x240	= 0x0001,
	DVRVIDEO_SIZE_640x480	= 0x0002,
	DVRVIDEO_SIZE_640x240	= 0x0004,
	DVRVIDEO_SIZE_640x480SW	= 0x0008,
	DVRVIDEO_SIZE_352x288	= 0x0010,
	DVRVIDEO_SIZE_352x240	= 0x0020,
	DVRVIDEO_SIZE_704x576	= 0x0040,
	DVRVIDEO_SIZE_704x480	= 0x0080
}DVRENUM_VIDEO_SIZE_TYPE;

const CSize DVRENUM_VIDEO_SIZE[]=
{
	 CSize(320,240),
	 CSize(640,480),
	 CSize(640,240),
	 CSize(640,480),
	 CSize(352,288),
	 CSize(352,240),
	 CSize(704,576),
	 CSize(704,480)
};
const CString DVRENUM_VIDEO_SIZE_DESC[]=
{
	"320x240",
	"640x480",
	"640x240",
	"640x480",
	"352x288",
	"352x240",
	"704x576",
	"704x480"
};
//视频流类型
typedef enum _TVT_ENUM_VIDEO_STREAM_TYPE
{
	VIDEO_STREAM_NONE = 0x0000,
	VIDEO_STREAM_PREVIEW = 0x0001,
	VIDEO_STREAM_CAPTURE = 0x0002,
	VIDEO_STREAM_NET = 0x0004,
	VIDEO_STREAM_MOBILE = 0x0008,
	VIDEO_STREAM_NET_REALTIME = 0x0010
}TVT_ENUM_VIDEO_STREAM_TYPE;

//卡支持的功能
typedef enum _ENUM_CARD_FUNCTION
{
	DVRCARD_FUNC_AUTO_RESET,//支持自动Reset功能
	DVRCARD_FUNC_CALL_MONITOR,//支持CallMonitor功能
	DVRCARD_FUNC_SUPPORTMOTION,//是否支持硬件Motion
	DVRCARD_FUNC_SUPPORTQUAD,//是否支持硬件分割
	DVRCARD_FUNC_AUTO_ADJUST_FRATE,//是否支持自动调节帧率
	DVRCADD_FUNC_WATERMARK,//支持水印功能
	DVRCARD_FUNC_PTZPRESET,//支持预置点功能
	DVRCARD_FUNC_OVERLAYDEMO,//支持Overlay显示
	DVRCARD_FUNC_AUTOMAILIMAGE,//AutoMail能不能附加图片
//if 压缩具有相关性如3316的两个通道在一起压缩
	DVRCARD_FUNC_COMPRESS_RELATE,//是否支持压缩相关性

	DVRCARD_FUNC_CIF_INTERLACE,	//CIF分辨率有Interlace现象
	DVRCARD_FUNC_HARD_DEINTERLACE	//硬件De-Interlace
} ENUM_CARD_FUNCTION;

#define HAVEAUTOMAILIMAGE 0
#define NOAUTOMAILIMAGE	  1
//卡参数
typedef enum _ENUM_CARD_OPTIONS
{
	DVRCARDOPT_VIDEODEV_NUM,//得到视频芯片数量
	DVRCARDOPT_AUDIODEV_NUM,//得到声音芯片数量
	DVRCARDOPT_CHANNEL_NUM,//得到视频输入数量
	DVRCARDOPT_ALARMIN_NUM,//得到报警输入数量
	DVRCARDOPT_ALARMOUT_NUM,//得到报警输出数量
	DVRCARDOPT_STREAM_NUM,//得到流数量
	DVRCARDOPT_STREAM_TYPE,//得到支持那些流类型
	DVRCARDOPT_VIDEOSIZE_TYPE,//得到支持哪些视频尺
	DVRCARDOPT_RESET_SECTIME,//Auto Reset的秒数
	DVRCARDOPT_RESET_START,//Auto Reset开始或结束计时
//if 卡支持DVRCARD_FUNC_OVERLAYDEMO功能
	DVRCARDOPT_SCREEN_QUAD,//设置分割模式//设置分割模式	参数定义: 一个表示分割模式的指针，BYTE[0]=分割模式(0~6依次表示1，4，6，8，9，13，16)，BYTE[1]=页面号，后续BYTE为当前所有通道排序的序号
	DVRCARDOPT_OVERLAY_ADDR,//设置Overlay地址
	DVRCARDOPT_OVERLAY_PITCH,//设置Overlay的Pitch
	DVRCARDOPT_OVERLAY_SIZE,//设置Overlay的大小
	DVRCARDOPT_USBDEVICELOST,//得到USB卡是否丢失
	DVRCARDOPT_NETSTREAMSEND,//设置是否发送网络数据

	DVRCARDOPT_CUR_CARDTYPE,	//得到卡类型	Add by djx 2007/08/01
	DVRCARDOPT_FPGA_VERSION,	//得到FPGA版本	Add by djx 2007/08/01

	DVRCARDOPT_NET_CHL_MASK,		//当前网络需要压缩的通道开关	Add by zhangzhen 2007/10/11
	DVRCARDOPT_NET_FRAMERATE_INC,	//提升网络帧率，参数为负，则表示要降低帧率，绝对值表示要改变的帧率	Add By zhangzhen 2007/10/17

	DVRCARDOPT_VBR_CBR_SEL,	//VBR/CBR可选
	DVRCARDOPT_DEINTERLACE	//De-Interlace
} ENUM_CARD_OPTIONS;

//通道状态
typedef enum _ENUM_CHANNEL_GET_STATUS
{
	CHSTATUS_GET_SIGANL,//信号状态
	CHSTATUS_GET_SENSOR,//报警电平
	CHSTATUS_GET_MOTION,//移动状态
	CHSTATUS_GET_ENABLEAGC//能不能调节AGC
}ENUM_CHANNEL_STATUS;

//通道状态的设置
typedef enum _ENUM_CHANNEL_SET_STATUS
{
	CHSTATUS_SET_ALARM,//设置第报警电平
	CHSTATUS_SET_CALLMON,//设置CallMonitor输出
	CHSTATUS_SET_RECQTY,//设置Capture流（如果有的话）的压缩画质
	CHSTATUS_SET_NETQTY,//设置NET流（如果有的话）的压缩画质
	CHSTATUS_SET_RECFRATE,//设置Capture流（如果有的话）的压缩帧率
	CHSTATUS_SET_NETFRATE,//设置NET流（如果有的话）的压缩帧率
	CHSTATUS_SET_MOTIONGRID,//使通道显示或隐藏Motion Grid
	CHSTATUS_SET_SCREENQUAD,//设置第index个芯片的分割模式
	CHSTATUS_SET_NEXTISKEYFRAME,//设置录像压缩流下一帧是关键帧
	CHSTATUS_SET_MOTION_PARAM,//设置Motion灵敏度
	CHSTATUS_SET_TIMESTAMP,//是否加时间戳
	CHSTATUS_SET_AUDIO_ENABLE,//是否打开音频通道(操作单位:一个通道)
	CHSTATUS_SET_NEXTISKEYFRAME_NET,//设置网络压缩流下一帧是关键帧
	CHSTATUS_SET_NEXTISKEYFRAME_MOBILE,	//请求手机流即时关键帧
	CHSTATUS_SET_QTYCBR,		//设定录像VBR/CBR压缩方式
	CHSTATUS_SET_REC_RESOLUTION,//设置录像分辨率
	CHSTATUS_SET_NET_QTYCBR,  //设定网络VBR/CBR压缩方式TD4408
	CHSTATUS_SET_NET_CMP_FRAME//设置子码流压缩帧率
} 
ENUM_CHANNEL_SET_STATUS;

typedef enum _ENUM_VIDEO_AUDIO_PROPERTY
{
	VIDEO_PROPERTY_BRIGHTNESS,
	VIDEO_PROPERTY_CONTRUST,
	VIDEO_PROPERTY_HUE,
	VIDEO_PROPERTY_SATURATION,
	VIDEO_PROPERTY_AGC,

	AUDIO_PROPERTY_AGC
}ENUM_VIDEO_AUDIO_PROPERTY;

//视频属性
typedef struct _VIDEO_PROPER
{
	//视频属性，亮度、对比度、灰度、饱和度、AGC等
	DWORD bright;
	DWORD contrast;
	DWORD hue;
	DWORD saturation;
	DWORD agc;
}VIDEO_PROPER;
//
//音频属性
typedef struct _AUDIO_PROPER
{
	//音频属性，AGC
	DWORD agc;
}AUDIO_PROPER;
//
typedef struct _FrameBufStruct
{
	long		ChannelIndex;//总的通道号
	DWORD		BufferPara;//删除Buffer参数,如果是视频包括卡Index和BufIndex，如果是声音高16表示是否为PC自带声音
	BYTE     	*pBuf;//此帧数据的Buffer地址
	long		BufLen;//此帧数据的Buffer长度
	ULONGLONG	localTime;// 采集时的绝对时间单位为100nm
	ULONGLONG	FrameTime;//采集的相对时间
	DWORD      	nStreamID;//流类型
	long      	nVLostFlag;//信号状态
	long		bIsKeyFrame;//当数据为压缩数据时，是否为关键帧
	int		width;	//<41XD1-17>
	int		height;	//<41XD1-17>
}
FRAMEBUFSTRUCT;

typedef BOOL (CAPTURECALLBACK)(FRAMEBUFSTRUCT *bufStruct);//视频和声音捕捉CallBack函数原形
//设备管理基类
class CDeviceManager
{
public:
	virtual BOOL SupportCardFunc(DWORD FuncType)=0;
	virtual DWORD GetCardOpt(DWORD dwOptType)=0;
	virtual BOOL SetCardOpt(DWORD OptType,DWORD dwOptValue)=0;
	virtual BOOL GetAllChannelStatus(DWORD StatusType,DWORD StatusLen,DWORD	pChStatus[])=0;
	virtual BOOL SetChannelStatus(DWORD StatusType, DWORD index,long Status)=0;
	virtual BOOL DeviceInitial(DWORD VideoFormat, CAPTURECALLBACK *pVideoCallBack, CAPTURECALLBACK *pAudioCallBack,HWND hwnd,DWORD * pProductType)=0;
	virtual void DeviceFree(void)=0;
	virtual BOOL GetVideoStreamInfo(DWORD nStreamtype, DWORD VideoSizeType, BITMAPINFOHEADER *pBmpHdr)=0;
	virtual BOOL GetAudioStreamInfo(WAVEFORMATEX *pWFrmat)=0;
	virtual BOOL VideoCaptureStart(DWORD dwBmpSizeType,BITMAPINFOHEADER RecBmpHeader)=0;
	virtual BOOL VideoCaptureStop(void)=0;
	virtual BOOL AudioCaptureStart(WAVEFORMATEX WFrmat)=0;
	virtual BOOL AudioCaptureStop(void)=0;
	virtual BOOL GetVideoPropertyValue(DWORD ChIndex,VIDEO_PROPER  &VideoProperValue)=0;
	virtual BOOL GetVideoDefaultPropertyValue(DWORD ChIndex,ENUM_VIDEO_AUDIO_PROPERTY prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal)=0;
	virtual BOOL SetVideoPropertyValue(DWORD ChIndex,VIDEO_PROPER  pVideoProperValue)=0;
	virtual BOOL GetAudioPropertyValue(DWORD AudioIndex,AUDIO_PROPER  &AudioProperValue)=0;
	virtual BOOL GetAudioDefaultPropertyValue(DWORD AudioIndex,ENUM_VIDEO_AUDIO_PROPERTY prop,DWORD &minVal,DWORD &maxVal,DWORD &DefaultVal)=0;
	virtual BOOL SetAudioPropertyValue(DWORD AudioIndex,AUDIO_PROPER  pAudioProperValue)=0;
	virtual BOOL SetSwitch(DWORD *pSwitch, DWORD ChannelNum)=0;
	virtual BOOL SetMotionRect(DWORD index,  BYTE pByte[], long cnByte)=0;
	virtual BOOL SetChannelAlarm(DWORD ChannelNum , DWORD pChAlarm[])=0;
	virtual BOOL GetMaxFrameRate(DWORD ChannelNum,BOOL pSwitch[],int pFrameRate [])=0;
	virtual BOOL Password(void)=0;
	virtual BOOL ReleaseBuffer(DWORD isVideo,DWORD DelBufPara)=0;
	virtual BOOL GetQuadRect(DWORD sizetype,DWORD &CamNumPerF,CRect pRect[])=0;
};
#endif//_DEVICE_CONTROL_H