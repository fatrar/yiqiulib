///////////////////////////////////
////
//fv2.5A 增加音频VBI（时间截），增大音频缓冲
//2007/5/3 by lonnie
///////////////////////////////////
///////////////////////////////////
////
//fv2.4A 改变interrup type 字段位置,
//2007/4/14 lonnie
///////////////////////////////////
////
//fv2.3A 支持SDK特定区域,
//2007/4/14 lonnie
///////////////////////////////////
////
//fv2.2A 支持双码流,测试数据接口,
//2007/2/9 lonnie
//////////////////////////////////////////////////////////////////////////////////////////////////
////
//fv2.1A 支持双码流
//2007/1/15 lonnie
///////////////////////////////////////////////////////////////
////
//fv2.0A 支持双码流
//2006/9/23 lonnie
////////////////////////////


#ifndef _DSP_CARD_COMMON_HH_
#define _DSP_CARD_COMMON_HH_
#ifdef CHIP_DM642
	#define ULONG unsigned int
	#ifdef HANDLE 
	#define Handle void*
	#else
	#define HANDLE void*
	#endif
#endif//_WINBASE_
#if PAL==1
#define INITFRAMERATE  25
#define TIMEGAP		   40
#if CIFMODE == 1
#define WIDTH    352
#define HEIGHT   288
#else 
#define WIDTH    704
#define HEIGHT 	 576
#endif
#else
#define INITFRAMERATE  30
#define TIMEGAP		   33
#if CIFMODE == 1
#define WIDTH 	 320
#define HEIGHT   240
#else 
#define WIDTH    640
#define HEIGHT   480
#endif
#endif

//#define DSPCARD_CHANNEL_COUNT		4
typedef struct _SYSTIME{  // st 
     unsigned int dwYear; 
     unsigned char byMonth; 
     unsigned char byDay; 
     unsigned char byHour; 
     unsigned char byMinute; 
     unsigned char bySecond; 
	unsigned char byReserve;
     unsigned short wMilliSecond;
}SYSTIME; 
//AP set struct///////////////////////////
typedef struct _TVT_PCI_VIDEO_PREVIEW_PARAM
{
	int  iEnable;
	int  iScale;
}TVT_PCI_VIDEO_PREVIEW_PARAM;

typedef struct _TVT_PCI_VIDEO_ENCODE_PARAM
{
	int  iChan;
	int  iEnable;
	int  iScale;
	int  iBitRate;
	int  iFrameInterval;
//	int  iQuality;
	int  iImmediateIFrame;//dsp clear
	int  iReInit;//dsp clear
}TVT_PCI_VIDEO_ENCODE_PARAM;

typedef struct _TVT_PCI_AUDIO_PREVIEW_PARAM
{
	int  iChannel;
}TVT_PCI_AUDIO_PREVIEW_PARAM;

typedef struct _TVT_PCI_AUDIO_COMPRESS_PARAM
{
	int  iEnable;
}TVT_PCI_AUDIO_COMPRESS_PARAM;

///common struct/////////////////////////////////////////////////////////
#ifndef _WINBASE_
typedef struct _FILETIME
{
	unsigned int dwLowPart;	
	unsigned int dwHighPart;	
}FILETIME,*PFILETIME;
#endif//_WINBASE_

typedef enum _STREAMTYPE
{
	PREV = 0,
	REC,
	NET,
	AUD ,
	IEVENT,
	////////////////////////
	STREAMTYPECNT /////////
	////////////////////////
}STREAMTYPE;
typedef struct _APP_DRIVER_BUFFER_INFO
{
	ULONG m_ulChipTotalNumber;

	HANDLE m_hEvent[STREAMTYPECNT];

	//wirte reg value
	ULONG m_ulRegOffset;
	ULONG m_ulRegValue;
	ULONG m_ulRegCount;
	
}APP_DRIVER_BUFFER_INFO, *PAPP_DRIVER_BUFFER_INFO;/**/

typedef struct _TVT_SET_PARAM_PAC
{
	int paramType;
	int chanNum;
	int value;
}PARAMPACK,*PPARAMPACK;

typedef struct _TVT_AP_SET{
	ULONG dwAddress;
	ULONG dwValue;
}TVT_AP_SET,*PTVT_AP_SET;

typedef struct _TVT_PREV_VBI
{
	unsigned char byInvalid;       // status of this buff 
	unsigned char byVideoFormat;   // 1:422,2:411,3:420
	unsigned short  wStride;//
	unsigned int dwWidth;//width
	unsigned int dwHeight;//height
	SYSTIME  prevVideoTime; //capture time
	unsigned int  videoLoss;//loss state
	unsigned char  byChannelID;//for 8 bytes algian
	unsigned char  byReserve1;//for 8 bytes algian
	unsigned char  byAIProcess;//for 8 bytes algian
	unsigned char  byReserve2;//for 8 bytes algian
    unsigned int   dwRetCode;
    unsigned int   dwReserve3;
}TVT_PREV_VBI,*PTVT_PREV_VBI;

typedef struct _TVT_REC_VBI
{  
	unsigned char byChannel;//channel id
	unsigned char byDataType;//  0:rec ,1:NET 2:mobile      0xff: end of buff                
	unsigned char byInvalid;//status of this buff;
	unsigned char byKeyFrame;//keyframe flag     ; 
	unsigned short wWidth;    //image with;
	unsigned short wHeight;	  //image height;
	unsigned int dwFCC;		//'TVTX'
	unsigned int dwLen;   	//length of compressed data
	SYSTIME  recVideoTime;
	unsigned short wReserve1;
	unsigned short wReServe2;
	unsigned int dwReserve4SDK[10];
}TVT_REC_VBI,*PTVT_REC_VBI;
typedef struct _TVT_AUD_VBI
{
	unsigned int dwAudioFormat;
	SYSTIME  audioTime;
	unsigned int dwLen;
	unsigned int dwReserve;
}TVT_AUD_VBI,*PTVT_AUD_VBI;
typedef struct _TVT_CAP_STATUS
{
	unsigned char		byLock;
	unsigned char		byReserve1;
	unsigned char		byReserve2;
	unsigned char		byReserve3;
	unsigned int 		dwReserve4;
}TVT_CAP_STATUS,*PTVT_CAP_STATUS;

typedef struct _TVT_AI_VBI
{
	unsigned int 		dwNumberOfTargets;
	unsigned int 		dwNumberOfEvents;
	SYSTIME				frameRealTime;
	unsigned char 		byViewStates;
	unsigned char		byReserve1;
	unsigned char		byReserve2;
	unsigned char		byReserve3;
	
}TVT_AI_VBI,*PTVT_AI_VBI;

//const value define ////////////////////////

#define CAP_STATUS_SIZE			8

#define PREV_VBI_SIZE			128
#define REC_VBI_SIZE            	128
#define NET_VBI_SIZE			REC_VBI_SIZE
#define AUD_VBI_SIZE			0

#define D1_BUFF_SIZE			(720*576*2)     //for one channel
#define CIF_BUFF_SIZE			(352*288*2)     //for one channel
#define MOTION_STATUS			(352*288/8/8)   //for one channel
#define REC_BUFF_SIZE			(200*1024)      //for one channel
#define NET_BUFF_SIZE			REC_BUFF_SIZE      //for one channel

#define AUD_BUFF_SIZE			(1024*8*2*2)        //for 4channels

#define EACH_PREV_BUFF_SIZE		(1024*1024+3*1024*1024)//(CAP_STATUS_SIZE + (PREV_VBI_SIZE+CIF_BUFF_SIZE+MOTION_STATUS)*3+720*576*2+MOTION_STATUS+PREV_VBI_SIZE)
#define EACH_REC_BUFF_SIZE		(CAP_STATUS_SIZE + (REC_VBI_SIZE+REC_BUFF_SIZE)*4)
#define EACH_NET_BUFF_SIZE		(CAP_STATUS_SIZE + (REC_VBI_SIZE+REC_BUFF_SIZE)*4)
#define EACH_AUD_BUFF_SIZE	(CAP_STATUS_SIZE + AUD_BUFF_SIZE) //
#define EACH_IEVENT_BUFF_SIZE	(32)//*1024*1024)
#define EACH_TRAN_BUFF_SIZE		(1024*1024)
#define TARGET_MAX_NUM			50

#define AI_STREAM_BUF_SIZE	(1024*1024)              ///////new length
#define AI_MAX_RULE_COUNT   5

/*
+------------------+
|CAP_STATUS_SIZE   |
+------------------+
|VBI for channel1  | 
+------------------+
|                  |
|image data for ch1|
|                  |
+------------------+
|motion_Status     | //just for preview
+------------------+
|VBI for channel2  |
+------------------+
|                  |
|image data for ch2|
|                  |
+------------------+
|motion_Status     |
+------------------+
|VBI for channel3  |
+------------------+
|                  |
|image data for ch4|
|                  |
+------------------+
|motion_Status     |
+------------------+
|VBI for channel4  |
+------------------+------------+
|                  |            |
|image data for ch4|            |
|                  |            |
+------------------+            |
|motion_Status     |            |
+------------------+            |
|                               |
|             D1 ear            |
|                               |
|                               |
|                               |
+-------------------------------+
*/


//dsp memory layout///////////////////////////////
#define DSP_MEM_LAYOUT_BASE			0x200
#define DATA_VALID_FLAG_PREV			(DSP_MEM_LAYOUT_BASE)
#define DATA_VALID_FLAG_REC			(DATA_VALID_FLAG_PREV+4)
#define DATA_VALID_FLAG_AUD			(DATA_VALID_FLAG_REC+4)
#define DATA_VALID_FLAG_IEVENT	(DATA_VALID_FLAG_AUD+4)

#define DATA_VALID_FLAG_NET		DATA_VALID_FLAG_AUD
#define DATA_VALID_FLAG_MOB		DATA_VALID_FLAG_AUD
#define DATA_VALID_FLAG_DECV		DATA_VALID_FLAG_AUD

#define DMA_INT_TYPE	(DATA_VALID_FLAG_IEVENT+4)
#define DMA_INT_VIDEO	1
#define DMA_INT_AUDIO	2
#define DMA_INT_VREC	4
#define DMA_INT_INTL	8

#define DMA_ADDR_BASE		(DMA_INT_TYPE+4)//	(DATA_VALID_FLAG_PREV+4)

#define DMA_ADDR_PREV		(DMA_ADDR_BASE)
#define DMA_ADDR_REC		(DMA_ADDR_PREV+4)
#define DMA_ADDR_NET		(DMA_ADDR_REC+4)
#define DMA_ADDR_MOB		(DMA_ADDR_NET+4)
#define DMA_ADDR_AUD		(DMA_ADDR_MOB+4)
#define DMA_ADDR_IEVENT		(DMA_ADDR_AUD+4)
#define DMA_ADDR_DECV		(DMA_ADDR_IEVENT+4)


#define DSPTIME_FROM_PCI_SYNC (DMA_ADDR_DECV+4)
#define DSPTIME_FROM_PCI_ADDR (DSPTIME_FROM_PCI_SYNC+4)
//next addr:(DSPTIME_FROM_PCI_ADDR+12) //sizeof(_SYSTEMTIME)

#define AI_STREAM_PC_PHYADD  (DSPTIME_FROM_PCI_ADDR+12)///new
#define AI_STREAM_FLAG 		 (AI_STREAM_PC_PHYADD+4)///new
#define AI_STREAM_LEN		 (AI_STREAM_FLAG+4)///new

#define DECODE_CMD_PC_PHYADD  (AI_STREAM_LEN+12)
#define DECODE_CMD_FLAG	(DECODE_CMD_PC_PHYADD+4)
#define DECODE_CMD_LEN		(DECODE_CMD_FLAG+4)

#define PCI_VIDEO_MEMADDR_SIZE		0x290///old
#define PCI_VIDEO_MEMADDR_DATA		0x294///old

#define PT_DAT_DEFAULT		0x100
#define PT_CMD_PCI_DEFAULT	0x200
#define PT_CMD_NET_DEFAULT	0x300
#define PT_CMD_MOB_DEFAULT	0x400

//#define PT_VIDEO						PT_DAT_DEFAULT+0
//#define PT_AUDIO						PT_DAT_DEFAULT+1

#define PT_VIDEO_MD						PT_DAT_DEFAULT+2


#define PT_PCI_SET_VIDEO_BRIGHTNESS	 	(PT_CMD_PCI_DEFAULT+2) //Brightness
#define PT_PCI_SET_VIDEO_SATURATION	 	(PT_CMD_PCI_DEFAULT+3) //Color saturation
#define PT_PCI_SET_VIDEO_HUE	 		(PT_CMD_PCI_DEFAULT+4) //Hue
#define PT_PCI_SET_VIDEO_CONTRAST 		(PT_CMD_PCI_DEFAULT+5) //Contrast

#define PT_PCI_SET_ENCODE_BITRATE		(PT_CMD_PCI_DEFAULT+6)
#define PT_PCI_SET_ENCODE_FRAMERATE  	(PT_CMD_PCI_DEFAULT+7)
#define PT_PCI_SET_ENCODE_ISENCODE      (PT_CMD_PCI_DEFAULT+8)
#define PT_PCI_SET_ENCODE_IINTERVAL		(PT_CMD_PCI_DEFAULT+9)

#define PT_PCI_SET_MOTION_SENTIVE		(PT_CMD_PCI_DEFAULT+10)
#define PT_PCI_SET_MOTION_INTERVAL		(PT_CMD_PCI_DEFAULT+11)

#define PT_PCI_SET_IMMED_IFRAME			(PT_CMD_PCI_DEFAULT+12)

#define PT_PCI_SET_WATER_MARK			(PT_CMD_PCI_DEFAULT+13)

#define PT_PCI_SET_TIME_STAMP           (PT_CMD_PCI_DEFAULT+14)
#define PT_PCI_SET_CALL_MONITOR         (PT_CMD_PCI_DEFAULT+15)   //call moNITOR
#define PT_PCI_SET_D1_CHANNEL			(PT_CMD_PCI_DEFAULT+16)

#define PT_PCI_SET_AI_ENABLE			(PT_CMD_PCI_DEFAULT+17)
#define PT_PCI_SET_ADD_RULE				(PT_CMD_PCI_DEFAULT+18)
#define PT_PCI_SET_DEL_RULE				(PT_CMD_PCI_DEFAULT+19)
#define PT_PCI_SET_UPDATE_RULE			(PT_CMD_PCI_DEFAULT+20)



#define PT_PCI_SET_NET_ENCODE_BITRATE		PT_CMD_NET_DEFAULT+1
#define PT_PCI_SET_NET_ENCODE_FRAMERATE  	PT_CMD_NET_DEFAULT+2
#define PT_PCI_SET_NET_ENCODE_ISENCODE      PT_CMD_NET_DEFAULT+3
#define PT_PCI_SET_NET_ENCODE_IINTERVAL		PT_CMD_NET_DEFAULT+4
#define PT_PCI_SET_NET_IMMED_IFRAME			PT_CMD_NET_DEFAULT+5
#define PT_PCI_SET_NET_WATER_MARK			PT_CMD_NET_DEFAULT+6

#define PT_PCI_SET_MOB_ENCODE_BITRATE		PT_CMD_MOB_DEFAULT+1
#define PT_PCI_SET_MOB_ENCODE_FRAMERATE  	PT_CMD_MOB_DEFAULT+2
#define PT_PCI_SET_MOB_ENCODE_ISENCODE      PT_CMD_MOB_DEFAULT+3
#define PT_PCI_SET_MOB_ENCODE_IINTERVAL		PT_CMD_MOB_DEFAULT+4
#define PT_PCI_SET_MOB_IMMED_IFRAME			PT_CMD_MOB_DEFAULT+5
#define PT_PCI_SET_MOB_WATER_MARK			PT_CMD_MOB_DEFAULT+6

/*

*/

#define IOCTL_WIRITE_PORT_REGISTER	           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_PORT_REGISTER	           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_TEST_REGISTER	                   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_INITIALIZE_DRV	               CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FINISH_DRV  	                   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_WIRITE_FIRST_REGISTER	           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_FIRST_REGISTER	           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_WIRITE_SECOND_REGISTER	       CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_SECOND_REGISTER	           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CONTROL_CHIP_SETTING             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VIDEO_GET_DATA_INFO              CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80a, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SHOW_STREAM_START                CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80b, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SHOW_STREAM_END                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80c, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GET_DEVICE_NUMBER                CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80d, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SET_DSP_PARAM	                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80e, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_DSP_PARAM	                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80f, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_DSP_MULT_PARAM	          CTL_CODE(FILE_DEVICE_UNKNOWN, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_COFF	              	  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_COFF	              		  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x812, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GET_DEVICE_PASSWORD             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x813, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VIDEO_GET_TEST_DATA             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x814, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_DECODE_STREAM             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x815, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_DECODE_CMD                CTL_CODE(FILE_DEVICE_UNKNOWN, 0x816, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_WRITE_AI_STREAM				 IOCTL_WRITE_DECODE_STREAM

#endif //_DSP_CARD_COMMON_HH_
