/*H***************************************************************************
 File            : IVViewer.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 10:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVVIEWER_H_2010_
#define _IIVVIEWER_H_2010_


struct IIVDataSaver
{
    // ��Ƶ�ļ���һ��дʱ�ص���������������ļ������·���Ϳ�ʼ��ʱ��
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    // ��Ƶ�ļ��ر�ʱ���ص�
    virtual BOOL Close(int nChannelID, const FILETIME& time)=0;

    // ɾ����Ƶ�ļ��ص����Ӷ�֪ͨ�������ɾ�������ļ���
    virtual BOOL DeleteIVFile(const char* pPath)=0;

    virtual BOOL TellPreAlarmTime(int time)=0;
};

struct IIVViewer
{
    // ��ʾĿ����ο��·��ʱ�Ļص���
    // ����DC�����ڲ��ŵ�����֡��ʱ��
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time)=0;

    // bState=true��ʾĿ���·������������
    virtual BOOL ShowObjTrace(int nChannelID, bool bState)=0;

    // �õ�Ŀ���·���Ƿ�������ʾ
    virtual BOOL GetObjTraceState(int nChannelID, bool& bState)=0;

    // ������ʾ��Щ����Ŀ�껹�ǹ켣
    virtual void SetDataShowState(int nChannelID, int nState) = 0;
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
    virtual int Close(int nChannelID, const FILETIME& time)=0;
};

// �õ���Ӧ�Ķ���ָ��
namespace IVPlaybackFactory
{
    static IIVViewer* GetViewer(void);
    static IVDataFound* GetDataFound(void);
};




#endif  // End of file



