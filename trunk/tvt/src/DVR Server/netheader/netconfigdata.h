/*
*��Ȩ :TVT
*
*�ļ�����: netdata.h
*ժҪ����������Զ�����õ����ݽṹ���Լ����ݶ���˵��
*
*��ǰ�汾��1.0
*���ߣ���Դ
*������ڣ�2008.8.6
*
*
*ȡ���汾��1.0
*ԭ���ߣ���Դ
*������ڣ�2008.8.6
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

typedef enum//���������豸����
{
	Buzzer=0,
	AutoMile,
	RemoteAlarm
}ALARMOUTDEVICE;

typedef struct _BasicConfigData //��������
{
//	u_char  nIndex;//ͨ����
	u_char	nCaptionMode;//������ʾ��ʽ,0--None,1--ID,2--Name,3--ID/NAME
	int     iResolution;//ͼ��ߴ�
	int     iliveaudio;//�ֳ���Ƶѡ��

	bool	bAutoRestartPC;//PC�Զ���������
	int		iRestartPCInter;//PC�Զ�����ʱ����
	int		nRestartPCTime;//PC�Զ�����ʱ��

	bool	bAutologin;//�Զ�����ʱ�Ƿ��Զ���¼
	char	sRestartPCUser[16];//PC�Զ���������Ե�½�û���
	char	sRestartPCPwd[16];//PC�Զ���������Ե�½�û�����

	bool	bBuzzerAlarm;//����������
	u_char	nBuzzerTime;//��������������ʱ��2---255

	bool    bPreAlarmRecord;
	u_char	nPreAlarmRecordTime;//��ǰ¼��ʱ��2-60

	bool    bFullScreenAlarm;//ȫ��������ʾ
	u_char	nFullScreenTime;//ȫƴ����ʱ��

	u_char	nMotionHoldTime;//�ƶ�����ʱ��10-255
	u_long	iwDiskAlarmValue;
	u_char	nSensorHoldTime;//Sensor����ʱ��10-255
}BASIC_CONFIG;

typedef struct _camConfig//ͨ������
{
	u_char  nIndex;//ͨ����
	char	sCamName[16];//ͨ������
	bool	bCamProtect;//ͨ����������
	bool	bTimeDisplay;//ʱ����ʾ����
	bool	bCamSwitch;//ͨ������
	u_char	nNetQuality;//���续�� 0-4
	int		nAudioInIndex;//ѡ����Ƶͨ����Ӧ����������ͨ��,-1Ϊû����
}CAMERA_CONFIG;

typedef struct _VideoAmpConfigData	//��Ƶ��������
{
	//��Ƶ���ԣ����ȡ��Աȶȡ��Ҷȡ����Ͷȡ�AGC��
	u_char  nIndex;//ͨ����
	u_long	nBright;			//0-255
	u_long	nContrast;			//0-255
	u_long	nHue;				//0-255
	u_long	nSaturation;		//0-255
	u_long	agc;				//0-10000
}VIDOE_CONFIG;

typedef struct _camRecord//¼����������
{
	u_char  nIndex;//ͨ����
	u_char	nRecQuality;//¼��Ʒ�� 0-4
	u_long	lRecordDisk;//���ڱ��浱ǰ���õ�¼�����
	u_char	nDiskCoverMode;//0������,1����
	bool	bManualRecord;//��ʱ¼�񿪹�
	u_char	nRateManual;//��ʱ¼��֡�� 
	bool	bMotionCheck;//�ƶ���ⱨ������
	u_char	nRateMotion;//�ƶ�¼��֡��
	bool	bScheduleRecord;//��ʱ¼�񿪹�
	u_char	nRateSchedule;//��ʱ¼��֡��
	u_char	nRateSensor;//����¼��֡��
}CAMERA_RECORD;
typedef struct _MotionConfigData //�ƶ��������
{
	u_char  nIndex;//ͨ����
	int		iMotionSensitivity;	//�ƶ����������//0-100	
	int		iMotionSpeed;		//0-100
	int		iMotionBlockNum;	//0-100
	int     iMotionRectNum;		//0-CHNMAXMOTIONRECT
	RECT   MotionRect[CHNMAXMOTIONRECT];
}MOTION_CONFIG;

typedef	struct _ScheduleDay
{
	int  useNo;//�Ѿ�ʹ�õĸ��������ų̵ĸ���
	int  starttime[SCHEDULEDAYMAX];//�Է���Ϊ��λ��24Сʱ�ƿ�ʼʱ��
	int	 endtime[SCHEDULEDAYMAX];//�Է���Ϊ��λ��24Сʱ�ƽ���ʱ��
	//ÿһ���ų������±���ͬ����ʼ����ֹʱ������ʾ
}SCHEDULEDAY;

typedef struct _ScheduleConfigData //�����ų�����
{
	u_char  nIndex;//ͨ����
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULETIME_CONFIG;

typedef struct _ScheduleConfigMotion //�ƶ��ų�����
{
	u_char  nIndex;//ͨ����
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULEMOTION_CONFIG;

typedef struct _ScheduleConfigSensor //�������ų�����
{
	u_char		nIndex;//��������
	SCHEDULEDAY schedule[DAYSOFWEEK];
}SCHEDULESENSOR_CONFIG;

typedef struct _ScheduleConfigOther //���������ų�����
{
	u_char  nIndex;//���������豸�š��ο�ALARMOUTDEVICE����
	SCHEDULEDAY		schedule[DAYSOFWEEK];
}SCHEDULEOTHRE_CONFIG;


typedef struct _AutoMailConfigData//�����ʼ�����
{
	char	sSmtpServer[50];
	char	sUser[50];
	char	sPass[50];
	char	sFrom[50];
	char	sMailTo[50];
	char	sSubject[30];
	bool	bVif_Attachment;//û��ѡ����1 ѡ��Ϊ0
	u_long	nPic_Format;//0:jpg 1:bmp
	bool	bCamera;
	bool	bTime_Date;
	u_long	nResolution;//0: Cif(ȱʡ) 1:QCif 2: D1			
	int		nTimeIntervalLevel;	//m_nTimeIntervalLevel��-1����
	int		nPort;	//�˿ں�,Ĭ��25
	bool	bSSLSupport ;	//SMTP�������Ƿ�֧��SSL��Ĭ�ϲ�֧��
	bool	bAuthRequire;	//SMTP�������Ƿ���Ҫ�����֤��Ĭ����Ҫ
}AUTOMAIL_CONFIG;

//2008.4.21 modify by liy
typedef	struct _SensorInfo//����������
{
	u_char  nIndex;//������
	int		AlarmVolt;
	char	sName[25];
	char    sDesc[50];
	u_long	RecordCarmera[8];//������������ͨ����Ϣ   ��λ��ʾ1-256ͨ��	
	u_long AlarmOutFlag;//�����ı�������豸��Ϣ//0~23λ��ʾ�����屨������ã�24~31λ���α�ʾbuzzer,remote alarm,autoMail
}SENSORINFOR_CONFIG;

typedef struct _otherAlarmOut//���������������
{
	u_char nindex;//0 video loss 1,disk alarm
	u_long AlarmOutFlag;//0~23λ��ʾ�����屨������ã�24~31λ���α�ʾbuzzer,remote alarm,autoMail
}OTHERALARMOUT_CONFIG;

typedef struct _MotionAlarmOut//�ƶ������������
{
	u_char nindex;//cam��
	u_long AlarmOutFlag;//0~23λ��ʾ�����屨������ã�24~31λ���α�ʾbuzzer,remote alarm,autoMail
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
	BOOL  bHavePcAudio;//�Ƿ����Pcaudio
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

//ϵͳ֧�ֵ���Ƶ�ɼ���//tonycms
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


//��Ƶͨ��״̬//tonycms
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

//tonycms ������̨���ýṹ��
#define FLAG_CMD   0x33//
#define CMDX_MOVECTRL		6
#define VERSIONNUM 0x60000//��16λΪ���汾����16λΪ�Ͱ汾��Э�����ʱ���İ汾

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
	TYPE_CONFIG_ALARMOUT,//curType---0��ʾMotion //1��ʾsensor //2��ʾ����
	TYPE_CONFIG_EMAIL,
	TYPE_CONFIG_VIDEOAMP,//�Ѿ��ŵ���������
	TYPE_CONFIG_MOTION,
	TYPE_CONFIG_PTZ
}DVRENUM_CONFIG_TYPE;

#pragma pack(pop,4)
#endif
