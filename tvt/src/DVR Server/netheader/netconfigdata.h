/*
*版权 :TVT
*
*文件名称: netdata.h
*摘要：定义网络远程配置的数据结构，以及数据定义说明
*
*当前版本：1.0
*作者：李源
*完成日期：2008.8.6
*
*
*取代版本：1.0
*原作者：李源
*完成日期：2008.8.6
*
*/
#ifndef CONFIGDATAHEADER_H
#define CONFIGDATAHEADER_H
#pragma once
#pragma pack(push,4)
typedef unsigned char u_char;
typedef unsigned long u_long;
#define SCHEDULEDAYMAX 24
#define CHNMAXMOTIONRECT 100
#define DAYSOFWEEK    7

typedef enum//其他报警设备类型
{
	Buzzer=0,
	AutoMile,
	RemoteAlarm
}ALARMOUTDEVICE;

typedef struct _BasicConfigData //基本配置
{
//	u_char  nIndex;//通道号
	u_char	nCaptionMode;//标题显示方式,0--None,1--ID,2--Name,3--ID/NAME
	int     iResolution;//图像尺寸
	int     iliveaudio;//现场音频选择

	bool	bAutoRestartPC;//PC自动重启开关
	int		iRestartPCInter;//PC自动重启时间间隔
	int		nRestartPCTime;//PC自动重启时间

	bool	bAutologin;//自动重起时是否自动登录
	char	sRestartPCUser[16];//PC自动重启起电脑登陆用户名
	char	sRestartPCPwd[16];//PC自动重启起电脑登陆用户密码

	bool	bBuzzerAlarm;//蜂鸣器开关
	u_char	nBuzzerTime;//蜂鸣器报警持续时间2---255

	bool    bPreAlarmRecord;
	u_char	nPreAlarmRecordTime;//警前录像时间2-60

	bool    bFullScreenAlarm;//全屏报警提示
	u_char	nFullScreenTime;//全拼持续时间

	u_char	nMotionHoldTime;//移动保持时间10-255
	u_long	iwDiskAlarmValue;
	u_char	nSensorHoldTime;//Sensor保持时间10-255
}BASIC_CONFIG;

typedef struct _camConfig//通道配置
{
	u_char  nIndex;//通道号
	char	sCamName[16];//通道名称
	bool	bCamProtect;//通道保护开关
	bool	bTimeDisplay;//时间显示开关
	bool	bCamSwitch;//通道开关
	u_char	nNetQuality;//网络画质 0-4
	int		nAudioInIndex;//选择视频通道对应的声音输入通道,-1为没声音
}CAMERA_CONFIG;

typedef struct _VideoAmpConfigData	//视频属性配置
{
	//视频属性，亮度、对比度、灰度、饱和度、AGC等
	u_char  nIndex;//通道号
	u_long	nBright;			//0-255
	u_long	nContrast;			//0-255
	u_long	nHue;				//0-255
	u_long	nSaturation;		//0-255
	u_long	agc;				//0-10000
}VIDOE_CONFIG;

typedef struct _camRecord//录像启动配置
{
	u_char  nIndex;//通道号
	u_char	nRecQuality;//录像品质 0-4
	u_long	lRecordDisk;//用于保存当前可用的录像磁盘
	u_char	nDiskCoverMode;//0不覆盖,1覆盖
	bool	bManualRecord;//即时录像开关
	u_char	nRateManual;//即时录像帧率 
	bool	bMotionCheck;//移动侦测报警开关
	u_char	nRateMotion;//移动录像帧率
	bool	bScheduleRecord;//定时录像开关
	u_char	nRateSchedule;//定时录像帧率
	u_char	nRateSensor;//报警录像帧率
}CAMERA_RECORD;
typedef struct _MotionConfigData //移动侦测配置
{
	u_char  nIndex;//通道号
	int		iMotionSensitivity;	//移动侦测灵敏度//0-100	
	int		iMotionSpeed;		//0-100
	int		iMotionBlockNum;	//0-100
	int     iMotionRectNum;		//0-CHNMAXMOTIONRECT
	RECT   MotionRect[CHNMAXMOTIONRECT];
}MOTION_CONFIG;

typedef	struct _ScheduleDay
{
	int  useNo;//已经使用的个数，即排程的个数
	int  starttime[SCHEDULEDAYMAX];//以分钟为单位的24小时制开始时间
	int	 endtime[SCHEDULEDAYMAX];//以分钟为单位的24小时制结束时间
	//每一个排程项用下标相同的起始和终止时间来表示
}SCHEDULEDAY;

typedef struct _ScheduleConfigData //日期排程配置
{
	u_char  nIndex;//通道号
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULETIME_CONFIG;

typedef struct _ScheduleConfigMotion //移动排程配置
{
	u_char  nIndex;//通道号
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULEMOTION_CONFIG;

typedef struct _ScheduleConfigSensor //传感器排程配置
{
	u_char		nIndex;//传感器号
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULESENSOR_CONFIG;

typedef struct _ScheduleConfigOther //其他报警排程配置
{
	u_char  nIndex;//其他报警设备号。参看ALARMOUTDEVICE定义
	SCHEDULEDAY		schedule[DAYSOFWEEK];
}SCHEDULEOTHRE_CONFIG;


typedef struct _AutoMailConfigData//电子邮件配置
{
	char	sSmtpServer[50];
	char	sUser[50];
	char	sPass[50];
	char	sFrom[50];
	char	sMailTo[50];
	char	sSubject[30];
	bool	bVif_Attachment;//没有选中是1 选中为0
	u_long	nPic_Format;//0:jpg 1:bmp
	bool	bCamera;
	bool	bTime_Date;
	u_long	nResolution;//0: Cif(缺省) 1:QCif 2: D1			
	int		nTimeIntervalLevel;	//m_nTimeIntervalLevel从-1计数
	int		nPort;	//端口号,默认25
	bool	bSSLSupport ;	//SMTP服务器是否支持SSL，默认不支持
	bool	bAuthRequire;	//SMTP服务器是否需要身份验证，默认需要
}AUTOMAIL_CONFIG;

//2008.4.21 modify by liy
typedef	struct _SensorInfo//传感器配置
{
	u_char  nIndex;//传感器
	int		AlarmVolt;
	char	sName[25];
	char    sDesc[50];
	u_long	RecordCarmera[8];//传感器关联的通道信息   按位表示1-256通道	
	u_long AlarmOutFlag;//关联的报警输出设备信息//0~23位表示报警板报警输出好，24~31位依次表示buzzer,remote alarm,autoMail
}SENSORINFOR_CONFIG;

typedef struct _otherAlarmOut//其他报警输出配置
{
	u_char nindex;//0 video loss 1,disk alarm
	u_long AlarmOutFlag;//0~23位表示报警板报警输出好，24~31位依次表示buzzer,remote alarm,autoMail
}OTHERALARMOUT_CONFIG;

typedef struct _MotionAlarmOut//移动报警输出配置
{
	u_char nindex;//cam号
	u_long AlarmOutFlag;//0~23位表示报警板报警输出好，24~31位依次表示buzzer,remote alarm,autoMail
}MOTIONALARMOUT_CONFIG;

typedef struct _SystemInfo
{
	long  VideoInputNum; //???
	long  AudioInputNum;//???
	long  SensorNum;//Sensor?
	long  BasicAlarmOutNum;//???????
	long  OtherAlarmOutNum;//???????
	DWORD LocalDrives; //A-Z????
	DWORD VideosizeType;//???VIDEO_SIZE
	long  COMcount;//COM??
	BOOL  bHavePcAudio;//是否包含Pcaudio
}SYSTEMINFO;

const CString VIDEO_SIZE[]=
{
	"320x240",
	"640x480",
	"640x240",
	"640x480",
	"352x288",
	"352x240",
	"704x576",
	"704x480",
	"704x288",
	"704x240"
};

/*
const CString CAPTION_MODE[]=
{
	"none",
	"ID",
	"Name",
	"ID/NAME",
};
*/

const CString AUTOMAIL_PIC_FORMAT[]=
{
	"jpg",
	"bmp",
};

const CString AUTOMAIL_Resolution[]=
{
	"Cif",
	"QCif",
};
/*
const CString NET_VIDEO_QUALITY[]=
{
	"lowest",
	"lower",
	"medium",
	"higher",
	"highest",
};
*/

const CString PROCOL_LIST[]=
{
	"NULL",
	"PelcoP",
	"PelcoD",
	"Lilin",
	"Minking",
	"Neon",
	"Star",
	"Vido",
	"DSCP",
	"VISCA",
	"DH-SD",
};

const CString AutoMailTimeIntervalLevel[]=
{
	"5",
	"10",
	"20",
	"30",
	"60",
	"120",
	"300",
	"600",
};

const CString FullScreenAlarm[]=
{
	"5",
	"10",
	"20",
	"30",
	"60",
};

const CString ParityBit[]=
{
	"None",	//N ??
	"Odd",	//O ??
	"Even",	//E ??
	"Mark",	//M ??
	"Space", //S ??
};

//系统支持的视频采集卡//tonycms
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
	DVR_CARD_TD4316,

	DVR_CARD_TD4304,
	DVR_CARD_TD4404L,
	DVR_CARD_TD4408L,
	DVR_CARD_TD4416L


}DVRENUM_PRODUCT_TYPE;


//视频通道状态//tonycms
typedef enum _DVRENUM_CHANNEL_STATUS
{
 	CHANNEL_STATUS_NORMAL=0,
	CHANNEL_STATUS_LOSS,
	CHANNEL_STATUS_PROTECT,
	CHANNEL_STATUS_SCHEDULE,
	CHANNEL_STATUS_MANUAL,
	CHANNEL_STATUS_MOTION,
	CHANNEL_STATUS_SENSOR,
	CHANNEL_STATUS_CLOSE
}DVRENUM_CHANNEL_STATUS;

//tonycms 增加云台配置结构体
#define FLAG_CMD   0x33//
#define CMDX_MOVECTRL		6
#define VERSIONNUM 0x60000//高16位为主版本，低16位为低版本，协议更改时更改版本

typedef struct _Pack_cmd
{
	short	flag;
	BYTE	cmd;
	long    ver;
	DWORD	data_size_extra;
}PACK_CMD;


typedef struct _Pack_Cfg
{
	DWORD flagHead;
	DWORD packSize;
	DWORD idxFrame;
	DWORD idxPack;//Total Pack Index
	BYTE channel;
	BYTE frameType;//0=KeyFrame
	BYTE nPack;//Count of Packs in a frame
	BYTE idxPackFrame;//Pack Index inter Frame 
}PACK_CFG;

typedef enum _DVRENUM_CONFIG_TYPE
{
	TYPE_CONFIG_BASE=1,
	TYPE_CONFIG_RECORD,
	TYPE_CONFIG_SENSOR,
	TYPE_CONFIG_SCHEDULE,//curType---0:cam,1:sensor,2:alarm out,3:Motion
	TYPE_CONFIG_ALARMOUT,//curType---0表示Motion //1表示sensor //2表示其他
	TYPE_CONFIG_EMAIL,
	TYPE_CONFIG_VIDEOAMP,//已经放到了主界面
	TYPE_CONFIG_MOTION,
	TYPE_CONFIG_PTZ
}DVRENUM_CONFIG_TYPE;

#pragma pack(pop,4)
#endif
