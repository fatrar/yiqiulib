/*H***************************************************************************
 File            : IIVDevice.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 10:27
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVDEVICE_H_2010_
#define _IIVDEVICE_H_2010_


#ifdef IIVDEVICE_EXPORTS
    #define IIVDEVICE_API extern "C" __declspec(dllexport)
#else    
    #define IIVDEVICE_API extern "C" __declspec(dllimport)
#endif


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
    bool AlarmRecord:1;
    bool resvr2:1;
    bool resvr3:1;
};

// �������ܵİ�������
enum IVRuleType
{
    IV_Invade,              // ����
    IV_Leave_Disappear,     // ��ʧ/�뿪
    IV_LeftBehind,          // ����
    IV_Loiters,             // �ǻ�
    IV_Statistic,           // ����ͳ��
    IV_Vehicle_Retrograde,  // ��������
    IV_Illegal_Parking,     // �Ƿ�ͣ��
    IV_Stage_Change,        // �����任
};

// ������Ϊͨ�������ĸ�Ϊʱ����������Ϊ�ص����������û�����
typedef BOOL (*AlarmCallBackFn)(const AlarmOutTable&,IVRuleType,int,const FILETIME&,void* pParm);

struct IIVDeviceSetter
{
    // ����Alarm����ʱ�ĸ��ϲ�Ļص�,pParmΪ�û�����Ĳ������ص��ᴫ��
    virtual void SetIVAlarmOutCallBack(AlarmCallBackFn pAlarmCallBackFn, void* pParm)=0;

    // �����������ݷ��͵Ļص�����IVLiveFactory�õ����ָ��
    virtual void SetIVDataCallBack(const IIVDataSender* pIVDataSender)=0;
};


namespace DeviceFactory
{
    IIVDEVICE_API IIVDeviceBase* GetIVDeviceBase(void);
    IIVDEVICE_API IIVDeviceSetter* GeIVDeviceSetter(void);
    IIVDEVICE_API IIVStatistic* GetIVStatistic(void);
};




#endif  // End of file



