/********************************************************************
	created:	2010/01/09
	created:	9:1:2010   14:53
	filename: 	f:\tvt\src\DVR Server\DEV_HC_DM642i\CommonUtil.h
	file path:	f:\tvt\src\DVR Server\DEV_HC_DM642i
	file base:	CommonUtil
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/

/*H***************************************************************************
 File            : CommonUtil.h
 Subsystem       :  

Function Name(s): CCommonUtil
Author          : YiQiu
Date            : 2010-1-12  
Time            : 11:18
Description     : 

Revision        : 

History
-------


Copyright (c) xxxx Ltd.
***************************************************************************H*/


#ifndef COMMONUTIL_H_2010
#define COMMONUTIL_H_2010









#endif  // End of file

//

struct IIVStatistic
{
    // �Ƿ���ͳ�ƹ���,����ͨ���ţ�int nChannelID����ͬ��
    virtual BOOL IsHaveStatisticRule(int nChannelID)=0;

    // ����ͳ��
    virtual BOOL ResetStatistic(int nChannelID)=0;
    
    // bFlag=true��ʼͳ�ƣ�flaseֹͣ
    virtual BOOL StartStatistic(int nChannelID, bool bFlag)=0;

    // �õ�ͳ��״̬
    virtual BOOL GetStatisticState(int nChannelID, bool& bFlag)=0;
};

// Alarm �ص�������ݣ�����bool�����Ӧ���Ƿ����
struct AlarmOutTable
{
    bool EMail:1;
    bool EMap:1;
    bool TelphoneCall:1;
    bool SnapShot:1;
    bool Sensor:1;
    bool Relay:1;
    bool Buzzer:1;
    bool FullScreen:1;
    bool PlaySound:1;
    bool resvr1:1;
    bool resvr2:1;
    bool resvr3:1;
};

// �������ܵİ�������
enum IVRuleType
{
    // .. ʡ��
};

// ������Ϊͨ�������ĸ�Ϊʱ����������Ϊ�ص����������û�����
typedef BOOL (*AlarmCallBack)(const AlarmOutTable&,IVRuleType,int,const FILETIME&,void* pParm);
struct IIVDeviceSetter
{
    // ����Pre Alarmʱ��
    virtual void TellPreAlarmTime(int time)=0;

    // ���ļ�ѭ��ʱ���ã����ɾ�����ܶ�Ӧ�������ļ�
    virtual void DeleteIVFile(const char* pPath)=0;

    // ����Alarm����ʱ�ĸ��ϲ�Ļص�,pParmΪ�û�����Ĳ������ص��ᴫ��
    virtual void SetIVAlarmOutCallBack(AlarmCallBack AlarmCallBackFn, void* pParm)=0;
    
    // �����������ݷ��͵Ļص�����IVLiveFactory�õ����ָ��
    virtual void SetIVDataCallBack(const IIVDataSender* pIVDataSender)=0;
};

struct IIVDeviceBase
{
    // ���ͨ���Ƿ���������
    virtual BOOL IsUse(int nChannelID)=0;
    
    // �����ͨ��ʹ������
    virtual BOOL Use(int nChannelID, bool bState)=0;
    
    // bState=true��ʾĿ���·������������
    virtual BOOL ShowObjTrace(bool bState)=0;

    // �õ�Ŀ���·���Ƿ�������ʾ
    virtual BOOL GetObjTraceState(bool& bState)=0;

    // �Ƿ��п��е������豸
    virtual BOOL IsHaveFreeDevice(void)=0;
};

// �õ���Ӧ�Ķ���ָ��
namespace DeviceFactory
{
    static IIVDeviceBase* GetIVDeviceBase(void);
    static IIVDeviceSetter* GeIVDeviceSetter(void);
    static IIVStatistic* GetIVStatistic(void);
};

struct IIVDataSender
{
    // �������ݲɼ�����Ļص�
    virtual BOOL OnIVDataSend(int nChannelID, int Data)=0;
};

struct IIVDataSaver
{
    // ��Ƶ�ļ���һ��дʱ�ص���������������ļ������·���Ϳ�ʼ��ʱ��
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    // ��Ƶ�ļ��ر�ʱ���ص�
    virtual BOOL Close(int nChannelID, int time)=0;
};

struct IIVViewer
{
    // ��ʾĿ����ο��·��ʱ�Ļص���
    // ����DC�����ڲ��ŵ�����֡��ʱ��
    virtual BOOL Paint(int nChannelID, const CDC& dc, const FILETIME& nTime)=0;
};

// �õ���Ӧ�Ķ���ָ��
namespace IVLiveFactory
{
    static IIVViewer* GetViewer(void);
    static IIVDataSaver* GetDataSaver(void);
    static IIVDataSender* GetDataSender(void);
};

struct IVDataFound
{
    // ��Ƶ�ļ���ʱ�ص�������ͨ������Ƶ�ļ�·������ʱ���
    virtual int Open(int nChannelID, const char* pPath, const FILETIME& time)=0;
    
    // ��Ƶ�ļ��ر�ʱ�ص�
    virtual int Close(int nChannelID, int time)=0;
};

// �õ���Ӧ�Ķ���ָ��
namespace IVPlaybackFactory
{
    static IIVViewer* GetViewer(void);
    static IVDataFound* GetDataFound(void);
};