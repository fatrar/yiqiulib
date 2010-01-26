#ifndef _DEFINE_BASE_H_
#define _DEFINE_BASE_H_

enum DisplayFormatValues
{
	DISPLAY_FORMAT_UYVY = 0,
	DISPLAY_FORMAT_YUY2,
	MAX_DISPLAY_FORMAT
};

#define DEFAULT_DISPLAY_FORMAT DISPLAY_FORMAT_YUY2

enum DataFormatValues
{
	DATA_FORMAT_YUY422_PK = 0,	
	DATA_FORMAT_YUY411_PL,
	DATA_FORMAT_YUY420_PL,
	MAX_DATA_FORMAT
};
#define DEFAULT_DATA_FORMAT DATA_FORMAT_YUY411_PL

enum VideoForamtValues
{	
	VIDEO_FORMAT_NTSC_M = 0,
	VIDEO_FORMAT_PAL_B,
	MAX_VIDEO_FORMAT,
	VIDEO_FORMAT_NTSC_J,								// Not implemented
	VIDEO_FORMAT_PAL_M,								// Not implemented
	VIDEO_FORMAT_PAL_N,								// Not implemented
	VIDEO_FORMAT_SECAM,								// Not implemented
	VIDEO_FORMAT_PAL_NC								// Not implemented
};
#define DEFAULT_VIDEO_FORMAT VIDEO_FORMAT_NTSC_M

enum ImageSizeValues
{
	IMAGE_SIZE_352X240 =	0,
	IMAGE_SIZE_704X240,
	IMAGE_SIZE_704X480,
	MAX_IMAGESIZE
};
#define DEFAULT_IMAGE_SIZE IMAGE_SIZE_704X240	//

enum ImageWidthValues
{
	IMAGE_WIDTH_640 = 0,
	IMAGE_WIDTH_704,
	MAX_IMAGEWIDTH
};
#define DEFAULT_IMAGE_WIDTH IMAGE_WIDTH_704

enum ImageWidthRValues
{
	IMAGE_WIDTH_R640 = 640,
	IMAGE_WIDTH_R704 = 704
};
#define DEFAULT_IMAGE_RWIDTH IMAGE_WIDTH_R704

enum ImageHeightRValues
{
	IMAGE_HEIGHT_R480 = 480,
	IMAGE_HEIGHT_R576 = 576
};
#define DEFAULT_IMAGE_RHEIGHT IMAGE_HEIGHT_R480

enum DisplayModeValues
{
	DISPLAY_MODE_1	= 0,	
	DISPLAY_MODE_4,
	DISPLAY_MODE_6,
	DISPLAY_MODE_8,
	DISPLAY_MODE_9,
	DISPLAY_MODE_10,
	DISPLAY_MODE_13,
	DISPLAY_MODE_16,
	MAX_DISPLAY_MODE
};
#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_16	//

enum AudioSamplingFrquency
{
	AUDIO_FRQ_8 =	0,							// Not implemented
	AUDIO_FRQ_11,
	AUDIO_FRQ_16,
	AUDIO_FRQ_22,
	MAX_AUDIO_FRQ,
};
#define DEFAULT_AUDIO_FRQ			AUDIO_FRQ_8
#define DEFAULT_AUDIO_VOLUME		255
#define MAX_AUDIO_VOLUME			255

#define DEFAULT_BRIGHTNESS			0x80
#define DEFAULT_CONTRAST			0x80
#define DEFAULT_COLOR				0x80
#define DEFAULT_HUE					0x80

#define ENCODER_DEFAULT_QP			3
#define ENCODER_DEFAULT_IFN		20
#define ENCODER_DEFAULT_MES		5
#define ENCODER_DEFAULT_KEY		0
#define ENCODER_DEFAULT_CM			0

//Device
#define MAX_VIDEO_CHANNEL			16
#define MAX_AUDIO_CHANNEL			16
#define MAX_CHANNEL_CHANGEABLE	128
#define INVALID_CHANNEL_NUMBER	0xFF
#define ALL_DEVICE					0x80

#define CHIP_MAX_32					32
#define CHIP_MAX_16					16
#define CHIP_MAX_8					8
#define CHIP_MAX_4					4
#define CHIP_MAX_2					2
#define CHIP_MAX_1					1
#define MAX_DEVICE					1
#define MAX_RECORD_PATH				4

#define DMA_VIDEO_LIVE				1
#define DMA_VIDEO_RECORD			2
#define DMA_AUDIO_RECORD			4

#define MAX_WIDTH_COUNT				4
#define MAX_HEIGHT_COUNT			4

//Color Control
#define MAX_COLOR						4
#define COLOR_BRIGHTNESS			0
#define COLOR_CONTRAST				1
#define COLOR_COLOR					2
#define COLOR_HUE						3
#define COLOR_ALL						0xFF

#define MEM_LOCK						1
#define MEM_UNLOCK					0

#define VIDEO_NOT_SYNC				0
#define VIDEO_SYNC					1

#define PAGESIZE						4096

#define MAX_VCAP_DMA_SIZE			128*4

#define MAX_VIDEO_CAP_SIZE			804	//704*292*2*8 //288 + VBILine 4
#define MAX_ENCODER_SIZE			100	//704*576
#define MAX_VDUMMY_SIZE				101	//704*292*2

#define MAX_VBI_LINE					4
#define MAX_VALID_VBI_LINE			2
#define MAX_VIDEO_QUEUE_COUNT		8

#define MAX_AUDIO_QUEUE_COUNT		16
#define MAX_AUDIO_CAP_SIZE			34		//4096*2*(16+1)
#define ONE_AUDIO_QUEUE_SIZE		4096*2	//4096*2

#define FIELD_ODD						0
#define FIELD_EVEN					1
#define FIELD_LEFT_352				0
#define FIELD_RIGHT_352				1

#define IDX_VIDEO						1
#define IDX_AUDIO						2
#define AM16R480_LOCAL				0
#define AM16R480_MOTION				1

#define RUBY3_DEVICE_NAME			"\\\\.\\AM7416R3%d"
#define MAX_DEVICE_NAME				0x80
#define MAX_OSD_TEXT					32
#define MAX_REC_TABLE				0x80

#define MAX_ONE_CHANNEL_FPS_NTSC	30
#define MAX_ONE_CHANNEL_FPS_PAL	25

#define MAX_OSD_FONT					65
#define MAX_OSD_WIDTH				34
#define MAX_OSD_HEIGHT				22

#define AVD201_ID1					0xB8
#define AVD201_ID2					0xBA
#define ENC7121_ID					0x8C

#define AL_QUAD_CHANNEL				4


//APP Only
#define WINDOW95						1
#define WINDOW98						2
#define WINDOWNT						3
#define WINDOW2000					4

#define VIDEO_LIVE					0
#define VIDEO_CAPTURE				1
#define DX_OFFSCREEN					0
#define DX_OVERLAY					1

#define WINDOW_WIDTH					1024
#define WINDOW_HEIGHT				768

#define LOGO_WIDTH					320
#define LOGO_HEIGHT					240

#ifdef AMPLAYER
	#define OVERLAY_START_LEFT			40
	#define OVERLAY_START_TOP			30
#elif MPEGPLAYER
	#define OVERLAY_START_LEFT			72
	#define OVERLAY_START_TOP			80
#else
	#define OVERLAY_START_LEFT			68
	#define OVERLAY_START_TOP			12
#endif

#define AM7416R_WINDOW_NAME		"ALogics AM7416RDlg Window"
#define MPEGPLAYER_WINDOW_NAME	"ALogics One Channel Player Window"
#define AMPLAYER_WINDOW_NAME		"ALogics Multi Channel Player Window"
#define ALCLIENT_WINDOW_NAME		"ALogics Client Window"

#define AM7416RAPP_EXE_NAME		"AM7416RApp.exe"
#define ALPLAYER_EXE_NAME			"ALPlayer.exe"
#define LOGO_FILE_NAME				"Logo_320X240.bmp"
#define ALOGICS_NAME					"ALogics"

#define BK_OVERLAY_COLORKEY_32	(RGB(130, 130, 130))
#define BK_OVERLAY_COLORKEY_16	0x8410
#define BK_CAPTURE_COLORKEY_32	(RGB( 30,  30,  30))
#define OSD_TEXT_COLORKEY			(RGB(0xFF,0xFF,0x00))
#define LINE_RECT_COLOR				(RGB(240, 248, 255))
#define FOCUS_RECT_COLOR			(RGB( 50,  50, 200))
#define MOTION_COLORKEY				(RGB(240,   0,   0))
#define VSYNC_LOSS_COLORKEY		(RGB(220,  20,  60))

#define CALENDAR_BACK_COLOR		(RGB(0xC0,0xC0,0xC0))
#define CALENDAR_RECORD_COLOR		(RGB(0x77,0x88,0x99))
#define CALENDAR_FOCUS_TEXT_COLOR	(RGB(0xD2,0x69,0x1E))

#define HOUR_BACK_COLOR				(RGB(0xC0,0xC0,0xC0))
#define HOUR_RECORD_COLOR			(RGB(0x99,0x99,0x99))
#define HOUR_FOCUS_TEXT_COLOR		(RGB(0xD2,0x69,0x1E))

#define MINUTE_VBACK_COLOR			(RGB(0xC0,0xC0,0xC0))
#define MINUTE_VRECORD_COLOR		(RGB(0x99,0x99,0x99))
#define MINUTE_VFOCUS_TEXT_COLOR	(RGB(0xD2,0x69,0x1E))

#define MINUTE_ABACK_COLOR			(RGB(0xC0,0xC0,0xC0))
#define MINUTE_ARECORD_COLOR		(RGB(0x99,0x99,0x99))
#define MINUTE_AFOCUS_TEXT_COLOR	(RGB(0xD2,0x69,0x1E))

#define DATATIME_TEXT_COLOR		(RGB(0x13,0x13,0x46))

enum SystemLanguageID
{
	ID_KOREAN   = 0x412,
	ID_ENGLISH  = 0x409,
	ID_JAPANESE = 0x411
};

#define VIDEO_DATA_FILE_EXT		"avd"
#define VIDEO_IDX_FILE_EXT			"vdx"

#define AUDIO_DATA_FILE_EXT		"wav"
#define AUDIO_IDX_FILE_EXT			"adx"

#define MPEG_I_FRAME					0xA0
#define MPEG_P_FRAME					0xA1
#define MPEG_X_FRAME					0xA2

#define MAX_PATH_SIZE				512
#define ONE_SECTOR_SIZE				512

#define DEFAULT_NETWORK_PORT_NUM	19876
#define MAX_NETWORK_BUF				PAGESIZE*2

#define NET_CONFIG_CMD					0x01
#define NET_START_CHANNEL_CMD			0x02
#define NET_STOP_CHANNEL_CMD			0x03
#define NET_VIDEO_DATA_CMD				0x04
#define NET_RECEIVE_OK_CMD				0x05
#define NET_EXIT_CMD						0x06
#define NET_DETECT_CMD					0x07

#define	DECODER_SUCCESS			1
#define	DECODER_HEADER_ERR		-1
#define	DECODER_CODE_ERR			-2

#define ONEYEAR_MONTH12				12
#define ONEWEEK_DAY7					7
#define ONEMONTH_WEEK6				6
#define ONEDAY_HOUR24				24
#define ONEHOUR_MINUTE60			60
#define ONEMINUTE_SECOND60			60
#define ONESECOND_MSECOND1000		1000
#define ONEMONTH_DAY31				31
#define INVALID_POS					-1

#define MAX_MOTION_COL				30
#define MAX_MOTION_ROW				24

#endif

