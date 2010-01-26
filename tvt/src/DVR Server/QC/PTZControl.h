/*******************************************************************************************************************************************
修改记录:
修改内容: 
		  <1> 增加一个新的PTZ_Init接口,由主调程序传入实际通道数量,使用户只能配置实际存在的通道;
			  如：当只有4个通道时，配置界面上只出现前4个通道，用户只能配置前4个通道
			  同时原来的PTZ_Init接口仍然有效，以使该DLL兼容以前版本
			  接口如下：
				PTZ_PROTOCOL_API PTZ_RESULT PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lptProtConfPos,int iChanelCnt)
修 改 人: 张振
修改日期: 2006/06/08

修改内容: 
  		  <2> 重载PTZ_Command接口，扩展主调程序对云台的直接控制功能:
			  原来只支持基本的简单命令，而对预置点/Group/速度的控制只能在DLL内部进行，现在增加了对它们的直接控制命令接口
			  新重载的接口如下:
				PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(const PTZ_CMD* pCmd,void* pResult=NULL)
修 改 人: 张振
修改日期: 2006/07/12
*******************************************************************************************************************************************/
#ifndef _PTZ_PROTOCOL_H_
#define _PTZ_PROTOCOL_H_

#include <afxsock.h>

/////////////////////////////////////////
#ifdef PTZ_PROTOCOL_EXPORTS
#define PTZ_PROTOCOL_API __declspec(dllexport)
#else
#define PTZ_PROTOCOL_API __declspec(dllimport)
#endif//#ifdef PTZ_PROTOCOL_EXPORTS
///////////////
/////////////////
#define MAXPRESET 16
#define MAXGROUP  4		//Group总数
#define MAX_PRESETNAME_LEN	(20+1)//预置点名最大长度20 + '\0'
#define MAX_GROUPNAME_LEN	(20+1)//Group名最大长度20 + '\0'

enum PROTOCOL_TYPE	//协议类型	张振	2006/07/27
{
		PROTOCOL_NULL=0,
		PROTOCOL_PELCOP,
		PROTOCOL_PELCOD,
		PROTOCOL_LILIN,
		PROTOCOL_MINKING,
		PROTOCOL_NEON,
		PROTOCOL_STAR,
		PROTOCOL_VIDO,
		PROTOCOL_DSCP,
		PROTOCOL_VISCA,		//<VISCA1>增加协议
		//{{do not specify value of them
		PROTOCOL_RANGE
		//}}
};

//没有特别需求，请不要改动下面各枚举常量的定义顺序
enum{//命令代码定义
	PTZCOMMAND_STOP=0		//停止转动
		,PTZCOMMAND_LEFT	//左转
		,PTZCOMMAND_RIGHT	//右转
		,PTZCOMMAND_UP		//上转
		,PTZCOMMAND_DOWN	//下转
		,PTZCOMMAND_NEAR	//近聚焦
		,PTZCOMMAND_FAR		//远聚焦
		,PTZCOMMAND_ZOOMOUT	//缩小
		,PTZCOMMAND_ZOOMIN	//放大
		,PTZCOMMAND_IRISOPEN	//光圈开
		,PTZCOMMAND_IRISCLOSE	//光圈关

		//以下分别是客户端调用预置点1~16的命令ID
		,PTZCOMMAND_GOPRESET1
		,PTZCOMMAND_GOPRESET2
		,PTZCOMMAND_GOPRESET3
		,PTZCOMMAND_GOPRESET4
		,PTZCOMMAND_GOPRESET5
		,PTZCOMMAND_GOPRESET6
		,PTZCOMMAND_GOPRESET7
		,PTZCOMMAND_GOPRESET8
		,PTZCOMMAND_GOPRESET9
		,PTZCOMMAND_GOPRESET10
		,PTZCOMMAND_GOPRESET11
		,PTZCOMMAND_GOPRESET12
		,PTZCOMMAND_GOPRESET13
		,PTZCOMMAND_GOPRESET14
		,PTZCOMMAND_GOPRESET15
		,PTZCOMMAND_GOPRESET16

		//以下分别是客户端巡球1~4的命令ID
		,PTZCOMMAND_GOGROUP1
		,PTZCOMMAND_GOGROUP2
		,PTZCOMMAND_GOGROUP3
		,PTZCOMMAND_GOGROUP4

		//以下分别是客户端设置预置点1~16的命令ID(不设置名称)
		,PTZCOMMAND_SAVEPRESET1
		,PTZCOMMAND_SAVEPRESET2
		,PTZCOMMAND_SAVEPRESET3
		,PTZCOMMAND_SAVEPRESET4
		,PTZCOMMAND_SAVEPRESET5
		,PTZCOMMAND_SAVEPRESET6
		,PTZCOMMAND_SAVEPRESET7
		,PTZCOMMAND_SAVEPRESET8
		,PTZCOMMAND_SAVEPRESET9
		,PTZCOMMAND_SAVEPRESET10
		,PTZCOMMAND_SAVEPRESET11
		,PTZCOMMAND_SAVEPRESET12
		,PTZCOMMAND_SAVEPRESET13
		,PTZCOMMAND_SAVEPRESET14
		,PTZCOMMAND_SAVEPRESET15
		,PTZCOMMAND_SAVEPRESET16

		,PTZCOMMAND_GROUPSET	//客户端到服务器端的组设定功能ID.
		,PTZCOMMAND_PRESETSET	//客户端到服务器端的预置点信息(命名)设定功能ID.
		,PTZCOMMAND_SPEEDSET	//客户端到服务器的速度设定ID.

		,PTZCOMMAND_GROUPGET	//服务器到客户端的组设定功能ID.
		,PTZCOMMAND_PRESETGET	//服务器端到客户端的预置点信息(命名)设定功能ID.
		,PTZCOMMAND_SPEEDGET	//服务器到客户端的速度设定ID.

		,PTZCOMMAND_QUERYCFGRIGHT	//客户端询问服务器端是否允许客户端做配置操作
		,PTZCOMMAND_PERMIT		//服务器端允许客户端做配置操作
		,PTZCOMMAND_REFUSE		//服务器端拒绝客户端做配置操作

		,PTZCOMMAND_GETPROTOCOLTYPE	//获取协议类型代码

		,PTZCOMMAND_GETALL	//获取某通道PTZ所有信息
		//...在这里追加命令码
		,PTZCOMMAND_RANGE
};

//<2>START
typedef struct _PTZ_CMD{	//PTZ命令结构
		WORD	channel_id;	//通道ID
		WORD	cmd;		//命令代码
}PTZ_CMD;

typedef struct _PRESET_INFO {//预置点信息结构
		WORD	prstno;					//预置点号
		char	name[MAX_PRESETNAME_LEN];//预置点名称
}PRESET_INFO;

typedef struct _GROUP_INFO{	//Group信息结构
		WORD	grpno;		//Group号
		WORD	prstnoList;	//预置点列表
		WORD	dwell;		//巡球时间间隙(S)
		char	name[MAX_GROUPNAME_LEN];	//Group名称
} GROUP_INFO;


typedef union _PTZ_INFO{//各种PTZ命令参数
		struct //预置点信息(预置点改名，取预置点信息时使用)
		{
			BYTE	cnt;//下面列表中有效元素数量
			BYTE	activePreset;	//当前云台镜头指向的预置点号(如果当前没有指向任何预置点，则为-1)
			BYTE	optType;	//操作模式:区分Lilin和其他协议设置预置点的操作模式
								//(注解: optType = 0: 表示一般操作模式；即，先移动云台方向，然后选择一个预置点保存，最后(可选)更改预置点名
								//		 optType = 1: Lilin协议模式；即，先选择一个预置点(这是云台会转到这个预置点)，然后移动云台到目标位置，
								//						最后为预置点取名并保存(此时预置点才真正保存到了云台))
								//		 optType = 2...255:其他为定义操作模式
			PRESET_INFO		prstInfoList[MAXPRESET]; //预置点信息列表
		}presetsInfo;

		struct//Group信息（Group配置及取Group信息时使用）
		{
			BYTE	cnt; //下面列表中有效元素数量
			BYTE	activeGroup;	//当前云台正在巡视的GROUP号(如果当前没有执行巡视操作，则为-1)
			GROUP_INFO	grpInfoList[MAXGROUP]; //Group信息列表
		}groupsInfo;

		struct//云台转动速度信息
		{
				WORD	maskSpd;		//从第0位到第11位分别表示以下12个成员的有效性（1代表有效，0代表无效）
										//具体各位和成员的对应关系见下面SPDMASK_***的宏定义

				BYTE	curPanSpd;		//当前水平速度
				BYTE	curTiltSpd;		//当前竖直速度
				BYTE	curFocusSpd;	//当前聚焦速度
				BYTE	curZoomSpd;		//当前缩放速度

				BYTE	minPanSpd;		//最小水平速度
				BYTE	minTiltSpd;		//最小竖直速度
				BYTE	minFocusSpd;	//最小聚焦速度
				BYTE	minZoomSpd;		//最小缩放速度

				BYTE	maxPanSpd;		//最大水平速度				
				BYTE	maxTiltSpd;		//最大竖直速度				
				BYTE	maxFocusSpd;	//最大聚焦速度				
				BYTE	maxZoomSpd;		//最大缩放速度
		}speedsInfo;

		struct{
			WORD	right;	//客户端权限
		}rightInfo;//权限信息

		struct{
			int	prtclType;	//协议类型
		}protocolInfo;

		//某通道PTZ所有信息
		struct{
			struct //预置点信息(预置点改名，取预置点信息时使用)
			{
				BYTE	cnt;//下面列表中有效元素数量
				BYTE	activePreset;	//当前云台镜头指向的预置点号(如果当前没有指向任何预置点，则为-1)
				BYTE	optType;	//操作模式:区分Lilin和其他协议设置预置点的操作模式
									//(注解: optType = 0: 表示一般操作模式；即，先移动云台方向，然后选择一个预置点保存，最后(可选)更改预置点名
									//		 optType = 1: Lilin协议模式；即，先选择一个预置点(这是云台会转到这个预置点)，然后移动云台到目标位置，
									//						最后为预置点取名并保存(此时预置点才真正保存到了云台))
									//		 optType = 2...255:其他为定义操作模式
				PRESET_INFO		prstInfoList[MAXPRESET]; //预置点信息列表
			}presetsInfo;

			struct//Group信息（Group配置及取Group信息时使用）
			{
				BYTE	cnt; //下面列表中有效元素数量
				BYTE	activeGroup;	//当前云台正在巡视的GROUP号(如果当前没有执行巡视操作，则为-1)
				GROUP_INFO	grpInfoList[MAXGROUP]; //Group信息列表
			}groupsInfo;

			struct//云台转动速度信息
			{
					WORD	maskSpd;		//从第0位到第11位分别表示以下12个成员的有效性（1代表有效，0代表无效）
											//具体各位和成员的对应关系见下面SPDMASK_***的宏定义

					BYTE	curPanSpd;		//当前水平速度
					BYTE	curTiltSpd;		//当前竖直速度
					BYTE	curFocusSpd;	//当前聚焦速度
					BYTE	curZoomSpd;		//当前缩放速度

					BYTE	minPanSpd;		//最小水平速度
					BYTE	minTiltSpd;		//最小竖直速度
					BYTE	minFocusSpd;	//最小聚焦速度
					BYTE	minZoomSpd;		//最小缩放速度

					BYTE	maxPanSpd;		//最大水平速度				
					BYTE	maxTiltSpd;		//最大竖直速度				
					BYTE	maxFocusSpd;	//最大聚焦速度				
					BYTE	maxZoomSpd;		//最大缩放速度
			}speedsInfo;

			struct{//权限信息
				WORD	right;	//客户端权限
			}rightInfo;

			struct{
				int	prtclType;	//协议类型
			}protocolInfo;
		}AllInfo;
		//…再有扩展命令参数，添加在此处
} PTZ_INFO;

//以下是maskSpd的各种取值宏定义，支持各种速度的单独/联合访问，建议总是使用这一系列宏名代替常数
#define SPDMASK_MINPAN	0x0001	//minPanSpd有效
#define SPDMASK_MAXPAN	0x0002	//maxPanSpd有效
#define SPDMASK_CURPAN	0x0004	//curPanSpd有效
#define SPDMASK_PAN		(SPDMASK_MINPAN | SPDMASK_MAXPAN | SPDMASK_CURPAN)//水平速度(minPanSpd/maxPanSpd/curPanSpd)有效

#define SPDMASK_MINTILT	0x0008	//minTiltSpd有效
#define SPDMASK_MAXTILT	0x0010	//maxTiltSpd有效
#define SPDMASK_CURTILT	0x0020	//curTiltSpd有效
#define SPDMASK_TILT	(SPDMASK_MINTILT | SPDMASK_MAXTILT | SPDMASK_CURTILT)//竖直(minTiltSpd/maxTiltSpd/curTiltSpd)有效

#define SPDMASK_MINFOCUS	0x0040	//minFocusSpd有效
#define SPDMASK_MAXFOCUS	0x0080	//maxFocusSpd有效
#define SPDMASK_CURFOCUS	0x0100	//curFocusSpd有效
#define SPDMASK_FOCUS		(SPDMASK_MINFOCUS | SPDMASK_MAXFOCUS | SPDMASK_CURFOCUS)//聚焦(minFocusSpd/maxFocusSpd/curFocusSpd)有效

#define SPDMASK_MINZOOM		0x0200	//minZoomSpd有效
#define SPDMASK_MAXZOOM		0x0400	//maxZoomSpd有效
#define SPDMASK_CURZOOM		0x0800	//curZoomSpd有效
#define SPDMASK_ZOOM		(SPDMASK_MINZOOM | SPDMASK_MAXZOOM | SPDMASK_CURZOOM)//缩放(minZoomSpd/maxZoomSpd/curZoomSpd)有效

#define SPDMASK_ALL			(SPDMASK_PAN | SPDMASK_TILT | SPDMASK_FOCUS | SPDMASK_ZOOM)//全部有效
#define SPDMASK_MIN			(SPDMASK_MINPAN | SPDMASK_MINTILT | SPDMASK_MINFOCUS | SPDMASK_MINZOOM)//最小值有效
#define SPDMASK_MAX			(SPDMASK_MAXPAN | SPDMASK_MAXTILT | SPDMASK_MAXFOCUS | SPDMASK_MAXZOOM)//最大值有效
#define SPDMASK_CUR			(SPDMASK_CURPAN | SPDMASK_CURTILT | SPDMASK_CURFOCUS | SPDMASK_CURZOOM)//当前值有效
//<2>END

enum PTZ_STATE
{
	PTZ_ENBLE
		,PTZ_DISABLE	
};
enum PTZ_RESULT
{
	PTZ_OK
		,PTZ_FALSE
		,PTZ_BADDB
};


//BOOL ProtocolSupport(PROTOCOL_TYPE eProtocolID);


/**********************************************************************
* 函数名称： // PTZ_Free
* 功能描述： //释放DLL
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Free();



/**********************************************************************
* 函数名称： // PTZ_Init
* 功能描述： // 进行初始化
* 输入参数： // isServer:判断是否是Server端;hwndParent:主窗口的句柄;
			 // SendFuc:发送网络传送数据的回调函数;
			 // lptProtConfPos:协议选择配置的窗口位置
			 // iChanelCnt:视频通道数量
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lpProtConfPos);
PTZ_PROTOCOL_API PTZ_RESULT PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lptProtConfPos,int iChanelCnt);//<1>



/**********************************************************************
* 函数名称： // PTZ_OnReceiveData
* 功能描述： // 接受数据
* 输入参数： // SockID:识别码;pvData:存储数据的缓冲区;dwDataLen:数据长度
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_OnReceiveData(DWORD SockID,void* pvData,DWORD dwDataLen);



/**********************************************************************
* 函数名称： // PTZ_GotoPreset
* 功能描述： // 显示预置点
* 输入参数： // point:指定弹出对话框的左上角位置
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_GotoPreset(POINT point);



/**********************************************************************
* 函数名称： // PTZ_Command
* 功能描述： // 发送命令
* 输入参数： // dwCommand:命令码
			 // pCmd[IN]:命令信息
			 // pResult[OUT]:命令返回缓冲区
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(DWORD dwCommand,int nChannelID=-1);
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(const PTZ_CMD* pCmd,CSocket* pSock=NULL);//<2>


/**********************************************************************
* 函数名称： // PTZ_SpeedDlg
* 功能描述： // 设置预置点的速度
* 输入参数： // point:指定弹出对话框的左上角位置
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_SpeedDlg(POINT point,int nPosType=0);



/**********************************************************************
* 函数名称： // PTZ_PresetSetDlg
* 功能描述： // 设置预置点
* 输入参数： // point:指定弹出对话框的左上角位置
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_PresetSetDlg(POINT point,int nPosType=0);



/**********************************************************************
* 函数名称： // PTZ_SetUserName
* 功能描述： // 设置用户名
* 输入参数： // pchUserName:用户名
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_SetUserName(char * pchUserName);



/**********************************************************************
* 函数名称： // PTZ_SetActiveChannel
* 功能描述： // 设置通道为活动状态
* 输入参数： // lChannelID:通道号
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT PTZ_SetActiveChannel(int lChannelID);



/**********************************************************************
* 函数名称： // PTZ_ShowProtocolConfig
* 功能描述： // 显示云台配置
* 输入参数： // lCmdShow:云台配置的显示方式
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_ShowProtocolConfig(long lCmdShow);



/**********************************************************************
* 函数名称： // PTZ_GetPtzState
* 功能描述： // 得到通道的状态
* 输入参数： // nChannel:通道号
***********************************************************************/
PTZ_PROTOCOL_API PTZ_STATE	PTZ_GetPtzState(int nChannel=-1);


#endif//#ifndef _PTZ_PROTOCOL_H_