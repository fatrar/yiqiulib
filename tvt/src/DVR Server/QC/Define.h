#ifndef _DEFINE_H
#define _DEFINE_H
#include "vfw.h"
#include ".\devicecontrol\devicecontrol.h"
const static BOOL b_dev_test=FALSE;
/*
typedef enum _SUPPORT_CARD_TYPE 
{
		my_DVR_CARD_TD3004,
		my_DVR_CARD_TD3008,
		my_DVR_CARD_TD3016,
		my_DVR_CARD_TD3116V1,
		my_DVR_CARD_TD3116V2,
		my_DVR_CARD_TD3216V2,
		my_DVR_CARD_TD3216V3,
		my_DVR_CARD_ALARM_CARD,
		my_DVR_CARD_TDNULL
}DVRCARD_PRODUCT_TYPE;//该测试程序支持的卡类型
*/
#define SUPPORT_CARD_NUMBER 25 //ADD3316 //ADD4104 //ADD4116//ADD USB_2 //add3004v2 djx 2008/1/5 3004_6805 //djx 2008/7/22 4108//add3304S,3308S chenlong 09/08/29
//视频尺寸
typedef enum _DVRENUM_PRODUCT_TYPE
{	
    DVR_CARD_TD3004,
    DVR_CARD_TD3008,
    DVR_CARD_TD3016,
    DVR_CARD_TD3116V1,
    DVR_CARD_TD3116V2,
    DVR_CARD_TD3216V2,
    DVR_CARD_TD3216V3,
    DVR_CARD_ALARM_CARD,
    //ADD 3316
    DVR_CARD_TD3316,
    DVR_CARD_TDNULL,
    DVR_CARD_TDUSB_1,
    DVR_CARD_TDUSB_2,
    DVR_CARD_TD3108,
    DVR_CARD_TD4104,
    DVR_CARD_TD4116,
    DVR_CARD_TD3004V2, //djx 2008/1/5 3004_6805
    DVR_CARD_TD4108, //djx 2008/7/22 4108
    DVR_CARD_TD3308, //zld 2009/4/22 3308
    DVR_CARD_TD3304, //zld 2009/4/22 3304
    DVR_CARD_TD4408, //zld 2009/4/24 4408
    DVR_CARD_TD3016V2, //zld 2009/6/22 3016_6805
    DVR_CARD_TD3008V2, //zld 2009/6/22 3008_6805
    DVR_CARD_TD3304S,  //chenlong 2009/08/29
    DVR_CARD_TD3308S,  //chenlong 2009/08/29
	DVR_CARD_TD4104i,  //hl
    DVR_CARD_TD3216,//暂时无用
    DVR_CARD_TD3116 //暂时无用		
}DVRENUM_PRODUCT_TYPE;

typedef enum _TEST_FUNCTION 
{
		ID_TEST_ID_VIEW,		//0
		ID_TEST_ID_TYPE_VIEW,	//1
		ID_TEST_SUB_ID_BUMBER,	//2
		ID_TEST_DRIVER_INSTALL,	//3
		ID_TEST_PASS_WORD,		//4
		ID_TEST_ALARM_IN,		//5
		ID_TEST_ALARM_OUT,		//6
		ID_TEST_ALARM_RELAY,	//7
		ID_TEST_CALL_MONITOR,	//8
		ID_TEST_SET_SWITCH,		//9
		ID_TEST_RESET,			//10
		ID_TEST_ALARM_PTZ,		//11 djx
		ID_TEST_AUDIO			//12 djx
}DVR_CARD_TEST_FUNCTION;//该测试程序支持的卡类型

#define TEST_ITEM_NUMBER 13 //djx 11

extern BOOL _test_table[SUPPORT_CARD_NUMBER][TEST_ITEM_NUMBER];

extern CString _LaodLibrary_name[SUPPORT_CARD_NUMBER];//by chenlong

extern int _video_channel_number[];

extern int  CARD_ALARM_INFOR[SUPPORT_CARD_NUMBER][3];
extern CRect channel_rect[17];
//extern CString m_ExePath;
typedef struct _CARD_INFOR
{
		//临时信息
		int video_channel_number;
		DVRENUM_PRODUCT_TYPE card_type_id;
		//纪录文档信息按顺序排列
		CString card_serial_number;//序列号

		CString disname;//卡的类别
		CString card_id;//卡的系统ID
		BOOL resSubId;
		BOOL resDriver;
		BOOL resPassWord;
		BOOL resAlarmIn;
		BOOL resAlarmOut;
		BOOL resAlarmRelay;
		BOOL resAlarmPTZ;//报警板支持云台 djx 2007/12/07
		BOOL resCallMonitor;
		BOOL resSetSwitch;
		BOOL resReset;
		
		BOOL resResult;		
		CString RecordTime;
		CString RecordUser;
		
		DWORD CardFpgaVer;//FPGA版本 add djx 2007/08/02
		BOOL resAudio;//声音测试 add djx 08/3/3
}CARD_INFOR;

#define ID3304S      "PCI\\VEN_0316&DEV_9200&SUBSYS_3304F20" //chenlong 09.09.08 3304S
#define ID3308S      "PCI\\VEN_0316&DEV_9200&SUBSYS_3308F20" //chenlong 09.09.08 3308S
//#define ID4308      "PCI\\VEN_19E5&DEV_3511&SUBSYS_351119E5" //zld 2009/4/24 4408
#define ID4408      "PCI\\VEN_19E5&DEV_3511&SUBSYS_351119E5" //zld 2009/4/24 4408
#define ID3304      "PCI\\VEN_0316&DEV_9200&SUBSYS_3304F10" //zld 2009/4/22 3304
#define ID3308      "PCI\\VEN_0316&DEV_9200&SUBSYS_3308F10" //zld 2009/4/22 3308
#define ID3004V2    "PCI\\VEN_1797&DEV_6804&SUBSYS_3004F70" //djx 2008/1/5 3004_6805
#define ID3004		"PCI\\VEN_1797&DEV_6800&SUBSYS_3004F60"//3004 video
#define ID3008		"PCI\\VEN_1797&DEV_6800&SUBSYS_3008F10"//3008 video
#define ID3016		"PCI\\VEN_1797&DEV_6800&SUBSYS_3016F60"//3016 video 
#define ID3016V2	"PCI\\VEN_1797&DEV_6805&SUBSYS_3016F60"//zld 2009/6/22 3016_6805
#define ID3008V2	"PCI\\VEN_1797&DEV_6805&SUBSYS_3008F10"//zld 2009/6/23 3008_6805
#define ID3116V1	"PCI\\VEN_109E&DEV_036E&SUBSYS_3116F10"//3116 1.0 video
#define ID3116V2	"PCI\\VEN_109E&DEV_036E&SUBSYS_3116F20"//3116 2.0 video
#define ID3216V2	"PCI\\VEN_109E&DEV_036E&SUBSYS_3216F20"//3216 2.0 video
#define ID3216V3	"PCI\\VEN_109E&DEV_036E&SUBSYS_3216F30"//3216 3.0 video
#define TDALARMNULL	"                       ALARM CARD"//3216 3.0 video
#define TDNULL	""//3216 3.0 video
//ADD 3316
#define ID3316		"PCI\\VEN_1999&DEV_A900&SUBSYS_00000000&REV_01"
extern CString _card_id[];

#define IDUSB_1		"USB\\Vid_04b4&Pid_8613"
#define IDUSB_2		"USB\\Vid_3100&Pid_f200"
#define ID3108		"PCI\\VEN_109E&DEV_036E&SUBSYS_3108F10"//3108
#define ID4104		"PCI\\VEN_104C&DEV_A002&SUBSYS_4104F230"
#define ID4104i		"PCI\\VEN_104C&DEV_A100&SUBSYS_410AF100"   // heliang +
#define ID4116		"PCI\\VEN_104C&DEV_A016&SUBSYS_4116F100"
#define ID4108      "PCI\\VEN_104C&DEV_A008&SUBSYS_4108F100" //djx 2008/7/22 4108

#define TW9200_VIDEO	"PCI\\VEN_0316&DEV_9200" //zld 2009/4/22 3308
#define TW6805_3016     "PCI\\VEN_1797&DEV_6805" //zld 2009/6/22 3016						 
#define TW6805_VIDEO	"PCI\\VEN_1797&DEV_6804" //djx 2008/1/5 3004_6805
#define TW6800_VIDEO	"PCI\\VEN_1797&DEV_6800"
#define BT878_VIDEO		"PCI\\VEN_109E&DEV_036E"
#define BT878_AUDIO		"PCI\\VEN_109E&DEV_0878"
#define ALOG7416		"PCI\\VEN_A123&DEV_A000"
#define ALOG480			"PCI\\VEN_1999&DEV_A900"
extern CString _card_chip_set[];
extern CString _display_name[];


#define TEST_ALARM_TYPE_NUMBER 3







#endif//_DEFINE_H