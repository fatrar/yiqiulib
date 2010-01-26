/*******************************************************************************************************************************************
�޸ļ�¼:
�޸�����: 
		  <1> ����һ���µ�PTZ_Init�ӿ�,������������ʵ��ͨ������,ʹ�û�ֻ������ʵ�ʴ��ڵ�ͨ��;
			  �磺��ֻ��4��ͨ��ʱ�����ý�����ֻ����ǰ4��ͨ�����û�ֻ������ǰ4��ͨ��
			  ͬʱԭ����PTZ_Init�ӿ���Ȼ��Ч����ʹ��DLL������ǰ�汾
			  �ӿ����£�
				PTZ_PROTOCOL_API PTZ_RESULT PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lptProtConfPos,int iChanelCnt)
�� �� ��: ����
�޸�����: 2006/06/08

�޸�����: 
  		  <2> ����PTZ_Command�ӿڣ���չ�����������̨��ֱ�ӿ��ƹ���:
			  ԭ��ֻ֧�ֻ����ļ��������Ԥ�õ�/Group/�ٶȵĿ���ֻ����DLL�ڲ����У����������˶����ǵ�ֱ�ӿ�������ӿ�
			  �����صĽӿ�����:
				PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(const PTZ_CMD* pCmd,void* pResult=NULL)
�� �� ��: ����
�޸�����: 2006/07/12
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
#define MAXGROUP  4		//Group����
#define MAX_PRESETNAME_LEN	(20+1)//Ԥ�õ�����󳤶�20 + '\0'
#define MAX_GROUPNAME_LEN	(20+1)//Group����󳤶�20 + '\0'

enum PROTOCOL_TYPE	//Э������	����	2006/07/27
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
		PROTOCOL_VISCA,		//<VISCA1>����Э��
		//{{do not specify value of them
		PROTOCOL_RANGE
		//}}
};

//û���ر������벻Ҫ�Ķ������ö�ٳ����Ķ���˳��
enum{//������붨��
	PTZCOMMAND_STOP=0		//ֹͣת��
		,PTZCOMMAND_LEFT	//��ת
		,PTZCOMMAND_RIGHT	//��ת
		,PTZCOMMAND_UP		//��ת
		,PTZCOMMAND_DOWN	//��ת
		,PTZCOMMAND_NEAR	//���۽�
		,PTZCOMMAND_FAR		//Զ�۽�
		,PTZCOMMAND_ZOOMOUT	//��С
		,PTZCOMMAND_ZOOMIN	//�Ŵ�
		,PTZCOMMAND_IRISOPEN	//��Ȧ��
		,PTZCOMMAND_IRISCLOSE	//��Ȧ��

		//���·ֱ��ǿͻ��˵���Ԥ�õ�1~16������ID
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

		//���·ֱ��ǿͻ���Ѳ��1~4������ID
		,PTZCOMMAND_GOGROUP1
		,PTZCOMMAND_GOGROUP2
		,PTZCOMMAND_GOGROUP3
		,PTZCOMMAND_GOGROUP4

		//���·ֱ��ǿͻ�������Ԥ�õ�1~16������ID(����������)
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

		,PTZCOMMAND_GROUPSET	//�ͻ��˵��������˵����趨����ID.
		,PTZCOMMAND_PRESETSET	//�ͻ��˵��������˵�Ԥ�õ���Ϣ(����)�趨����ID.
		,PTZCOMMAND_SPEEDSET	//�ͻ��˵����������ٶ��趨ID.

		,PTZCOMMAND_GROUPGET	//���������ͻ��˵����趨����ID.
		,PTZCOMMAND_PRESETGET	//�������˵��ͻ��˵�Ԥ�õ���Ϣ(����)�趨����ID.
		,PTZCOMMAND_SPEEDGET	//���������ͻ��˵��ٶ��趨ID.

		,PTZCOMMAND_QUERYCFGRIGHT	//�ͻ���ѯ�ʷ��������Ƿ�����ͻ��������ò���
		,PTZCOMMAND_PERMIT		//������������ͻ��������ò���
		,PTZCOMMAND_REFUSE		//�������˾ܾ��ͻ��������ò���

		,PTZCOMMAND_GETPROTOCOLTYPE	//��ȡЭ�����ʹ���

		,PTZCOMMAND_GETALL	//��ȡĳͨ��PTZ������Ϣ
		//...������׷��������
		,PTZCOMMAND_RANGE
};

//<2>START
typedef struct _PTZ_CMD{	//PTZ����ṹ
		WORD	channel_id;	//ͨ��ID
		WORD	cmd;		//�������
}PTZ_CMD;

typedef struct _PRESET_INFO {//Ԥ�õ���Ϣ�ṹ
		WORD	prstno;					//Ԥ�õ��
		char	name[MAX_PRESETNAME_LEN];//Ԥ�õ�����
}PRESET_INFO;

typedef struct _GROUP_INFO{	//Group��Ϣ�ṹ
		WORD	grpno;		//Group��
		WORD	prstnoList;	//Ԥ�õ��б�
		WORD	dwell;		//Ѳ��ʱ���϶(S)
		char	name[MAX_GROUPNAME_LEN];	//Group����
} GROUP_INFO;


typedef union _PTZ_INFO{//����PTZ�������
		struct //Ԥ�õ���Ϣ(Ԥ�õ������ȡԤ�õ���Ϣʱʹ��)
		{
			BYTE	cnt;//�����б�����ЧԪ������
			BYTE	activePreset;	//��ǰ��̨��ͷָ���Ԥ�õ��(�����ǰû��ָ���κ�Ԥ�õ㣬��Ϊ-1)
			BYTE	optType;	//����ģʽ:����Lilin������Э������Ԥ�õ�Ĳ���ģʽ
								//(ע��: optType = 0: ��ʾһ�����ģʽ���������ƶ���̨����Ȼ��ѡ��һ��Ԥ�õ㱣�棬���(��ѡ)����Ԥ�õ���
								//		 optType = 1: LilinЭ��ģʽ��������ѡ��һ��Ԥ�õ�(������̨��ת�����Ԥ�õ�)��Ȼ���ƶ���̨��Ŀ��λ�ã�
								//						���ΪԤ�õ�ȡ��������(��ʱԤ�õ���������浽����̨))
								//		 optType = 2...255:����Ϊ�������ģʽ
			PRESET_INFO		prstInfoList[MAXPRESET]; //Ԥ�õ���Ϣ�б�
		}presetsInfo;

		struct//Group��Ϣ��Group���ü�ȡGroup��Ϣʱʹ�ã�
		{
			BYTE	cnt; //�����б�����ЧԪ������
			BYTE	activeGroup;	//��ǰ��̨����Ѳ�ӵ�GROUP��(�����ǰû��ִ��Ѳ�Ӳ�������Ϊ-1)
			GROUP_INFO	grpInfoList[MAXGROUP]; //Group��Ϣ�б�
		}groupsInfo;

		struct//��̨ת���ٶ���Ϣ
		{
				WORD	maskSpd;		//�ӵ�0λ����11λ�ֱ��ʾ����12����Ա����Ч�ԣ�1������Ч��0������Ч��
										//�����λ�ͳ�Ա�Ķ�Ӧ��ϵ������SPDMASK_***�ĺ궨��

				BYTE	curPanSpd;		//��ǰˮƽ�ٶ�
				BYTE	curTiltSpd;		//��ǰ��ֱ�ٶ�
				BYTE	curFocusSpd;	//��ǰ�۽��ٶ�
				BYTE	curZoomSpd;		//��ǰ�����ٶ�

				BYTE	minPanSpd;		//��Сˮƽ�ٶ�
				BYTE	minTiltSpd;		//��С��ֱ�ٶ�
				BYTE	minFocusSpd;	//��С�۽��ٶ�
				BYTE	minZoomSpd;		//��С�����ٶ�

				BYTE	maxPanSpd;		//���ˮƽ�ٶ�				
				BYTE	maxTiltSpd;		//�����ֱ�ٶ�				
				BYTE	maxFocusSpd;	//���۽��ٶ�				
				BYTE	maxZoomSpd;		//��������ٶ�
		}speedsInfo;

		struct{
			WORD	right;	//�ͻ���Ȩ��
		}rightInfo;//Ȩ����Ϣ

		struct{
			int	prtclType;	//Э������
		}protocolInfo;

		//ĳͨ��PTZ������Ϣ
		struct{
			struct //Ԥ�õ���Ϣ(Ԥ�õ������ȡԤ�õ���Ϣʱʹ��)
			{
				BYTE	cnt;//�����б�����ЧԪ������
				BYTE	activePreset;	//��ǰ��̨��ͷָ���Ԥ�õ��(�����ǰû��ָ���κ�Ԥ�õ㣬��Ϊ-1)
				BYTE	optType;	//����ģʽ:����Lilin������Э������Ԥ�õ�Ĳ���ģʽ
									//(ע��: optType = 0: ��ʾһ�����ģʽ���������ƶ���̨����Ȼ��ѡ��һ��Ԥ�õ㱣�棬���(��ѡ)����Ԥ�õ���
									//		 optType = 1: LilinЭ��ģʽ��������ѡ��һ��Ԥ�õ�(������̨��ת�����Ԥ�õ�)��Ȼ���ƶ���̨��Ŀ��λ�ã�
									//						���ΪԤ�õ�ȡ��������(��ʱԤ�õ���������浽����̨))
									//		 optType = 2...255:����Ϊ�������ģʽ
				PRESET_INFO		prstInfoList[MAXPRESET]; //Ԥ�õ���Ϣ�б�
			}presetsInfo;

			struct//Group��Ϣ��Group���ü�ȡGroup��Ϣʱʹ�ã�
			{
				BYTE	cnt; //�����б�����ЧԪ������
				BYTE	activeGroup;	//��ǰ��̨����Ѳ�ӵ�GROUP��(�����ǰû��ִ��Ѳ�Ӳ�������Ϊ-1)
				GROUP_INFO	grpInfoList[MAXGROUP]; //Group��Ϣ�б�
			}groupsInfo;

			struct//��̨ת���ٶ���Ϣ
			{
					WORD	maskSpd;		//�ӵ�0λ����11λ�ֱ��ʾ����12����Ա����Ч�ԣ�1������Ч��0������Ч��
											//�����λ�ͳ�Ա�Ķ�Ӧ��ϵ������SPDMASK_***�ĺ궨��

					BYTE	curPanSpd;		//��ǰˮƽ�ٶ�
					BYTE	curTiltSpd;		//��ǰ��ֱ�ٶ�
					BYTE	curFocusSpd;	//��ǰ�۽��ٶ�
					BYTE	curZoomSpd;		//��ǰ�����ٶ�

					BYTE	minPanSpd;		//��Сˮƽ�ٶ�
					BYTE	minTiltSpd;		//��С��ֱ�ٶ�
					BYTE	minFocusSpd;	//��С�۽��ٶ�
					BYTE	minZoomSpd;		//��С�����ٶ�

					BYTE	maxPanSpd;		//���ˮƽ�ٶ�				
					BYTE	maxTiltSpd;		//�����ֱ�ٶ�				
					BYTE	maxFocusSpd;	//���۽��ٶ�				
					BYTE	maxZoomSpd;		//��������ٶ�
			}speedsInfo;

			struct{//Ȩ����Ϣ
				WORD	right;	//�ͻ���Ȩ��
			}rightInfo;

			struct{
				int	prtclType;	//Э������
			}protocolInfo;
		}AllInfo;
		//��������չ�������������ڴ˴�
} PTZ_INFO;

//������maskSpd�ĸ���ȡֵ�궨�壬֧�ָ����ٶȵĵ���/���Ϸ��ʣ���������ʹ����һϵ�к������泣��
#define SPDMASK_MINPAN	0x0001	//minPanSpd��Ч
#define SPDMASK_MAXPAN	0x0002	//maxPanSpd��Ч
#define SPDMASK_CURPAN	0x0004	//curPanSpd��Ч
#define SPDMASK_PAN		(SPDMASK_MINPAN | SPDMASK_MAXPAN | SPDMASK_CURPAN)//ˮƽ�ٶ�(minPanSpd/maxPanSpd/curPanSpd)��Ч

#define SPDMASK_MINTILT	0x0008	//minTiltSpd��Ч
#define SPDMASK_MAXTILT	0x0010	//maxTiltSpd��Ч
#define SPDMASK_CURTILT	0x0020	//curTiltSpd��Ч
#define SPDMASK_TILT	(SPDMASK_MINTILT | SPDMASK_MAXTILT | SPDMASK_CURTILT)//��ֱ(minTiltSpd/maxTiltSpd/curTiltSpd)��Ч

#define SPDMASK_MINFOCUS	0x0040	//minFocusSpd��Ч
#define SPDMASK_MAXFOCUS	0x0080	//maxFocusSpd��Ч
#define SPDMASK_CURFOCUS	0x0100	//curFocusSpd��Ч
#define SPDMASK_FOCUS		(SPDMASK_MINFOCUS | SPDMASK_MAXFOCUS | SPDMASK_CURFOCUS)//�۽�(minFocusSpd/maxFocusSpd/curFocusSpd)��Ч

#define SPDMASK_MINZOOM		0x0200	//minZoomSpd��Ч
#define SPDMASK_MAXZOOM		0x0400	//maxZoomSpd��Ч
#define SPDMASK_CURZOOM		0x0800	//curZoomSpd��Ч
#define SPDMASK_ZOOM		(SPDMASK_MINZOOM | SPDMASK_MAXZOOM | SPDMASK_CURZOOM)//����(minZoomSpd/maxZoomSpd/curZoomSpd)��Ч

#define SPDMASK_ALL			(SPDMASK_PAN | SPDMASK_TILT | SPDMASK_FOCUS | SPDMASK_ZOOM)//ȫ����Ч
#define SPDMASK_MIN			(SPDMASK_MINPAN | SPDMASK_MINTILT | SPDMASK_MINFOCUS | SPDMASK_MINZOOM)//��Сֵ��Ч
#define SPDMASK_MAX			(SPDMASK_MAXPAN | SPDMASK_MAXTILT | SPDMASK_MAXFOCUS | SPDMASK_MAXZOOM)//���ֵ��Ч
#define SPDMASK_CUR			(SPDMASK_CURPAN | SPDMASK_CURTILT | SPDMASK_CURFOCUS | SPDMASK_CURZOOM)//��ǰֵ��Ч
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
* �������ƣ� // PTZ_Free
* ���������� //�ͷ�DLL
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Free();



/**********************************************************************
* �������ƣ� // PTZ_Init
* ���������� // ���г�ʼ��
* ��������� // isServer:�ж��Ƿ���Server��;hwndParent:�����ڵľ��;
			 // SendFuc:�������紫�����ݵĻص�����;
			 // lptProtConfPos:Э��ѡ�����õĴ���λ��
			 // iChanelCnt:��Ƶͨ������
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lpProtConfPos);
PTZ_PROTOCOL_API PTZ_RESULT PTZ_Init(DWORD isServer,HWND hwndParent,DWORD SendFunc,LPPOINT lptProtConfPos,int iChanelCnt);//<1>



/**********************************************************************
* �������ƣ� // PTZ_OnReceiveData
* ���������� // ��������
* ��������� // SockID:ʶ����;pvData:�洢���ݵĻ�����;dwDataLen:���ݳ���
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_OnReceiveData(DWORD SockID,void* pvData,DWORD dwDataLen);



/**********************************************************************
* �������ƣ� // PTZ_GotoPreset
* ���������� // ��ʾԤ�õ�
* ��������� // point:ָ�������Ի�������Ͻ�λ��
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_GotoPreset(POINT point);



/**********************************************************************
* �������ƣ� // PTZ_Command
* ���������� // ��������
* ��������� // dwCommand:������
			 // pCmd[IN]:������Ϣ
			 // pResult[OUT]:����ػ�����
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(DWORD dwCommand,int nChannelID=-1);
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_Command(const PTZ_CMD* pCmd,CSocket* pSock=NULL);//<2>


/**********************************************************************
* �������ƣ� // PTZ_SpeedDlg
* ���������� // ����Ԥ�õ���ٶ�
* ��������� // point:ָ�������Ի�������Ͻ�λ��
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_SpeedDlg(POINT point,int nPosType=0);



/**********************************************************************
* �������ƣ� // PTZ_PresetSetDlg
* ���������� // ����Ԥ�õ�
* ��������� // point:ָ�������Ի�������Ͻ�λ��
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_PresetSetDlg(POINT point,int nPosType=0);



/**********************************************************************
* �������ƣ� // PTZ_SetUserName
* ���������� // �����û���
* ��������� // pchUserName:�û���
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_SetUserName(char * pchUserName);



/**********************************************************************
* �������ƣ� // PTZ_SetActiveChannel
* ���������� // ����ͨ��Ϊ�״̬
* ��������� // lChannelID:ͨ����
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT PTZ_SetActiveChannel(int lChannelID);



/**********************************************************************
* �������ƣ� // PTZ_ShowProtocolConfig
* ���������� // ��ʾ��̨����
* ��������� // lCmdShow:��̨���õ���ʾ��ʽ
***********************************************************************/
PTZ_PROTOCOL_API PTZ_RESULT	PTZ_ShowProtocolConfig(long lCmdShow);



/**********************************************************************
* �������ƣ� // PTZ_GetPtzState
* ���������� // �õ�ͨ����״̬
* ��������� // nChannel:ͨ����
***********************************************************************/
PTZ_PROTOCOL_API PTZ_STATE	PTZ_GetPtzState(int nChannel=-1);


#endif//#ifndef _PTZ_PROTOCOL_H_