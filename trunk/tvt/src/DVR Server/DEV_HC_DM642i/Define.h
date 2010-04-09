/**H**************************************************************************
 File            : Define.h
 Subsystem       : 
 Function Name(s): CDefine
 Author          : YiQiu
 Date            : 2010-4-8  
 Time            : 16:19
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DEFINE_H_2010_4
#define _DEFINE_H_2010_4



enum 
{
    Invaild_ChannelID= -1,
    Device_Free_Flag = -1,

    MAX_DEVICE_NUM		=	4,
    CHANNEL_PER_DEVICE	=	4,
    MAX_CHANNEL_NUM		=	16,
    MOTION_LENGTH_PAL	=	(352 * 288/(8 * 8)),
    MOTION_LENGTH_NTSC	=	(320 * 240/(8 * 8)),
    PRV_BUF_SIZE_PALCIF	=	(352 * 288 * 2 + MOTION_LENGTH_PAL),
    PRV_BUF_SIZE_NTSCCIF =	(320 * 240 * 2 + MOTION_LENGTH_NTSC),

    PRV_BUF_NUM			=	40,//16
    AUD_BUF_SIZE		=	1024,
    AUD_BUF_NUM			=	32,
    CAP_BUF_SIZE		=	REC_BUFF_SIZE,
    CAP_BUF_NUM			=	32*4,/*32*/	//zhangzhen	2007/02/09
    NET_BUF_SIZE		=	(352 * 288 * 2),
    NET_BUF_NUM			=	4,
    MOBILE_BUF_NUM		=	4,
    STREAM_TYPE_PRV		=	0,
    STREAM_TYPE_CAP		=	1,
    STREAM_TYPE_NET		=	2,
    STREAM_TYPE_AUD		=	3,

    MAX_PARAM_ONETIME	=	30,
    MAX_PARAMDATA_SIZE	=	364,

    PRV_TEMP_BUF_SIZE	=	(352 * 288 * 2),

    NET_FRAME_RATE_TOTAL = 32,
   
    MAX_IV_Parm_Buf_Size = sizeof(DWORD) + sizeof(PARAMPACK) + sizeof(WPG_Rule),

//#define NET_FRAME_RATE_TOTAL 32
//
//#define MAX_IV_Parm_Buf_Size (sizeof(DWORD) + sizeof(PARAMPACK) + sizeof(WPG_Rule))

};





//sdk
//#define MAX_DEVICE_NUM			4
//#define CHANNEL_PER_DEVICE		4
//#define MAX_CHANNEL_NUM			16
//#define MOTION_LENGTH_PAL		(352 * 288/(8 * 8))
//#define MOTION_LENGTH_NTSC		(320 * 240/(8 * 8))
//#define PRV_BUF_SIZE_PALCIF		(352 * 288 * 2 + MOTION_LENGTH_PAL)
//#define PRV_BUF_SIZE_NTSCCIF	(320 * 240 * 2 + MOTION_LENGTH_NTSC)
//
//#define PRV_BUF_NUM				40//16
//#define AUD_BUF_SIZE			1024
//#define AUD_BUF_NUM				32
//#define CAP_BUF_SIZE			REC_BUFF_SIZE
//#define CAP_BUF_NUM				32*4/*32*/	//zhangzhen	2007/02/09
//#define NET_BUF_SIZE			(352 * 288 * 2)
//#define NET_BUF_NUM				4
//#define MOBILE_BUF_NUM			4
//#define STREAM_TYPE_PRV			0
//#define STREAM_TYPE_CAP			1
//#define STREAM_TYPE_NET			2
//#define STREAM_TYPE_AUD			3
//
//#define MAX_PARAM_ONETIME		30
//#define MAX_PARAMDATA_SIZE		364
//
//#define PRV_TEMP_BUF_SIZE		(352 * 288 * 2)

typedef struct _NETBUF_DEQUE
{
	CCriticalSection CS;
	deque<FRAMEBUFSTRUCT *> Deque;
}NETBUF_DEQUE, *PNETBUF_DEQUE;

typedef struct _PACK
{
	CCriticalSection CS;
	deque<PARAMPACK> param;
}PACK, *PPACK;

typedef struct _SIGNAL
{
	CCriticalSection CS;
	BOOL bSignal;	//VideoLoss标志，0--正常信号，1--VideoLoss
}SIGNAL, *pSIGNAL;



#endif  // _DEFINE_H_2010_


// End of file

