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
#define DVRERR_DRIVERINITIALIZE	MAKEDVRERRCODE(2)//Driver ��ʼ������
#define DVRERR_DRIVERSTART		MAKEDVRERRCODE(3)//Driver ������׽����
#define DVRERR_DRIVERSTOP		MAKEDVRERRCODE(4)//Driver �˳���׽����
#define DVRERR_VIDEOSTANDARD	MAKEDVRERRCODE(5)//��֧�ִ���ʽ����
#define DVRERR_DRIVERMUSTINIT	MAKEDVRERRCODE(6)//Driver �����ʼ�����ܽ��в���
#define DVRERR_DRIVERGETDEVNUM	MAKEDVRERRCODE(7)//�õ��豸��������
#define DVRERR_NODVRCARD		MAKEDVRERRCODE(8)//û��
#define DVRERR_INVALIDPARA		MAKEDVRERRCODE(9)//��������
#define DVRERR_VERSION			MAKEDVRERRCODE(10)//�˰汾����֧�ִ˿�������

//�ͺŶ���:
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

//����������
#define _CHINA_TVT	       1
#define _OVERSEA_NEUTRAL   2

//����Buffer����
#define INP_BUFFER_SIZE 4000

//��Ƶ��ʽ
typedef enum _DVRENUM_VIDEO_STANDARD_TYPE
{
	DVRVIDEO_STANDARD_PAL = 0x0000,
	DVRVIDEO_STANDARD_NTSC = 0x0001
}DVRENUM_VIDEO_STANDARD_TYPE;
/*
//��Ƶ�ߴ�
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

//��Ƶ�ߴ�
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
//��Ƶ������
typedef enum _TVT_ENUM_VIDEO_STREAM_TYPE
{
	VIDEO_STREAM_NONE = 0x0000,
	VIDEO_STREAM_PREVIEW = 0x0001,
	VIDEO_STREAM_CAPTURE = 0x0002,
	VIDEO_STREAM_NET = 0x0004,
	VIDEO_STREAM_MOBILE = 0x0008,
	VIDEO_STREAM_NET_REALTIME = 0x0010
}TVT_ENUM_VIDEO_STREAM_TYPE;

//��֧�ֵĹ���
typedef enum _ENUM_CARD_FUNCTION
{
	DVRCARD_FUNC_AUTO_RESET,//֧���Զ�Reset����
	DVRCARD_FUNC_CALL_MONITOR,//֧��CallMonitor����
	DVRCARD_FUNC_SUPPORTMOTION,//�Ƿ�֧��Ӳ��Motion
	DVRCARD_FUNC_SUPPORTQUAD,//�Ƿ�֧��Ӳ���ָ�
	DVRCARD_FUNC_AUTO_ADJUST_FRATE,//�Ƿ�֧���Զ�����֡��
	DVRCADD_FUNC_WATERMARK,//֧��ˮӡ����
	DVRCARD_FUNC_PTZPRESET,//֧��Ԥ�õ㹦��
	DVRCARD_FUNC_OVERLAYDEMO,//֧��Overlay��ʾ
	DVRCARD_FUNC_AUTOMAILIMAGE,//AutoMail�ܲ��ܸ���ͼƬ
//if ѹ�������������3316������ͨ����һ��ѹ��
	DVRCARD_FUNC_COMPRESS_RELATE,//�Ƿ�֧��ѹ�������

	DVRCARD_FUNC_CIF_INTERLACE,	//CIF�ֱ�����Interlace����
	DVRCARD_FUNC_HARD_DEINTERLACE	//Ӳ��De-Interlace
} ENUM_CARD_FUNCTION;

#define HAVEAUTOMAILIMAGE 0
#define NOAUTOMAILIMAGE	  1
//������
typedef enum _ENUM_CARD_OPTIONS
{
	DVRCARDOPT_VIDEODEV_NUM,//�õ���ƵоƬ����
	DVRCARDOPT_AUDIODEV_NUM,//�õ�����оƬ����
	DVRCARDOPT_CHANNEL_NUM,//�õ���Ƶ��������
	DVRCARDOPT_ALARMIN_NUM,//�õ�������������
	DVRCARDOPT_ALARMOUT_NUM,//�õ������������
	DVRCARDOPT_STREAM_NUM,//�õ�������
	DVRCARDOPT_STREAM_TYPE,//�õ�֧����Щ������
	DVRCARDOPT_VIDEOSIZE_TYPE,//�õ�֧����Щ��Ƶ��
	DVRCARDOPT_RESET_SECTIME,//Auto Reset������
	DVRCARDOPT_RESET_START,//Auto Reset��ʼ�������ʱ
//if ��֧��DVRCARD_FUNC_OVERLAYDEMO����
	DVRCARDOPT_SCREEN_QUAD,//���÷ָ�ģʽ//���÷ָ�ģʽ	��������: һ����ʾ�ָ�ģʽ��ָ�룬BYTE[0]=�ָ�ģʽ(0~6���α�ʾ1��4��6��8��9��13��16)��BYTE[1]=ҳ��ţ�����BYTEΪ��ǰ����ͨ����������
	DVRCARDOPT_OVERLAY_ADDR,//����Overlay��ַ
	DVRCARDOPT_OVERLAY_PITCH,//����Overlay��Pitch
	DVRCARDOPT_OVERLAY_SIZE,//����Overlay�Ĵ�С
	DVRCARDOPT_USBDEVICELOST,//�õ�USB���Ƿ�ʧ
	DVRCARDOPT_NETSTREAMSEND,//�����Ƿ�����������

	DVRCARDOPT_CUR_CARDTYPE,	//�õ�������	Add by djx 2007/08/01
	DVRCARDOPT_FPGA_VERSION,	//�õ�FPGA�汾	Add by djx 2007/08/01

	DVRCARDOPT_NET_CHL_MASK,		//��ǰ������Ҫѹ����ͨ������	Add by zhangzhen 2007/10/11
	DVRCARDOPT_NET_FRAMERATE_INC,	//��������֡�ʣ�����Ϊ�������ʾҪ����֡�ʣ�����ֵ��ʾҪ�ı��֡��	Add By zhangzhen 2007/10/17

	DVRCARDOPT_VBR_CBR_SEL,	//VBR/CBR��ѡ
	DVRCARDOPT_DEINTERLACE	//De-Interlace
} ENUM_CARD_OPTIONS;

//ͨ��״̬
typedef enum _ENUM_CHANNEL_GET_STATUS
{
	CHSTATUS_GET_SIGANL,//�ź�״̬
	CHSTATUS_GET_SENSOR,//������ƽ
	CHSTATUS_GET_MOTION,//�ƶ�״̬
	CHSTATUS_GET_ENABLEAGC//�ܲ��ܵ���AGC
}ENUM_CHANNEL_STATUS;

//ͨ��״̬������
typedef enum _ENUM_CHANNEL_SET_STATUS
{
	CHSTATUS_SET_ALARM,//���õڱ�����ƽ
	CHSTATUS_SET_CALLMON,//����CallMonitor���
	CHSTATUS_SET_RECQTY,//����Capture��������еĻ�����ѹ������
	CHSTATUS_SET_NETQTY,//����NET��������еĻ�����ѹ������
	CHSTATUS_SET_RECFRATE,//����Capture��������еĻ�����ѹ��֡��
	CHSTATUS_SET_NETFRATE,//����NET��������еĻ�����ѹ��֡��
	CHSTATUS_SET_MOTIONGRID,//ʹͨ����ʾ������Motion Grid
	CHSTATUS_SET_SCREENQUAD,//���õ�index��оƬ�ķָ�ģʽ
	CHSTATUS_SET_NEXTISKEYFRAME,//����¼��ѹ������һ֡�ǹؼ�֡
	CHSTATUS_SET_MOTION_PARAM,//����Motion������
	CHSTATUS_SET_TIMESTAMP,//�Ƿ��ʱ���
	CHSTATUS_SET_AUDIO_ENABLE,//�Ƿ����Ƶͨ��(������λ:һ��ͨ��)
	CHSTATUS_SET_NEXTISKEYFRAME_NET,//��������ѹ������һ֡�ǹؼ�֡
	CHSTATUS_SET_NEXTISKEYFRAME_MOBILE,	//�����ֻ�����ʱ�ؼ�֡
	CHSTATUS_SET_QTYCBR,		//�趨¼��VBR/CBRѹ����ʽ
	CHSTATUS_SET_REC_RESOLUTION,//����¼��ֱ���
	CHSTATUS_SET_NET_QTYCBR,  //�趨����VBR/CBRѹ����ʽTD4408
	CHSTATUS_SET_NET_CMP_FRAME//����������ѹ��֡��
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

//��Ƶ����
typedef struct _VIDEO_PROPER
{
	//��Ƶ���ԣ����ȡ��Աȶȡ��Ҷȡ����Ͷȡ�AGC��
	DWORD bright;
	DWORD contrast;
	DWORD hue;
	DWORD saturation;
	DWORD agc;
}VIDEO_PROPER;
//
//��Ƶ����
typedef struct _AUDIO_PROPER
{
	//��Ƶ���ԣ�AGC
	DWORD agc;
}AUDIO_PROPER;
//
typedef struct _FrameBufStruct
{
	long		ChannelIndex;//�ܵ�ͨ����
	DWORD		BufferPara;//ɾ��Buffer����,�������Ƶ������Index��BufIndex�������������16��ʾ�Ƿ�ΪPC�Դ�����
	BYTE     	*pBuf;//��֡���ݵ�Buffer��ַ
	long		BufLen;//��֡���ݵ�Buffer����
	ULONGLONG	localTime;// �ɼ�ʱ�ľ���ʱ�䵥λΪ100nm
	ULONGLONG	FrameTime;//�ɼ������ʱ��
	DWORD      	nStreamID;//������
	long      	nVLostFlag;//�ź�״̬
	long		bIsKeyFrame;//������Ϊѹ������ʱ���Ƿ�Ϊ�ؼ�֡
	int		width;	//<41XD1-17>
	int		height;	//<41XD1-17>
}
FRAMEBUFSTRUCT;

typedef BOOL (CAPTURECALLBACK)(FRAMEBUFSTRUCT *bufStruct);//��Ƶ��������׽CallBack����ԭ��
//�豸�������
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