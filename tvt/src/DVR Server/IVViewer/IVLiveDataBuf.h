/*H***************************************************************************
 File            : IVLiveDataBuf.h
 Subsystem       : 
 Function Name(s): CIVLiveDataBuf
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 13:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVLIVEDATABUF_H_2010_
#define _IVLIVEDATABUF_H_2010_


// ģ�Ͳ��÷��߳���Ϣģʽ��
// �������ַ�ʽ�и�ȱ�ݣ�����֪ͨ�ȽϺã�ȡ���ݲ���ô�á��������ڵ�GetData
// ���߳���Ϣ���ڿ��ܻ�ʧ�ܣ�����Ҫдһ�ݲ������ķ�ʽ��ʵ�ֵĴ���
// ���ݶ���ʹ��deque��Ȼ��ȥ��¼���ʵ����������ж���pop��pushʱ����¼ƫ��
// ʵ�ʲ������ݲ�������+offset����ĳЩ��������Ȼ������offset��
class CIVLiveDataBuf :
    public Singleton<CIVLiveDataBuf>,
    public IIVDataBuf, 
    public IIVDataSender,
    public IIVDataSaver
{
public:
    CIVLiveDataBuf(void);
    virtual ~CIVLiveDataBuf(void);

    // IIVDataBuf
public:
    virtual TargetQueue* GetData(
        int nChannelID,
        const FILETIME& time );

    // IIVDataSender
public:
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen );

    virtual BOOL Init(
        int nDeviceCount,
        int nEveryDeviceChannelNum );

    virtual BOOL Unit();

    // IIVDataSaver
public:
    // ��Ƶ�ļ���һ��дʱ�ص���������������ļ������·���Ϳ�ʼ��ʱ��
    virtual BOOL Open(
        int nChannelID,
        const char* pPath,
        const FILETIME& time );

    // ��Ƶ�ļ��ر�ʱ���ص�
    virtual BOOL Close(
        int nChannelID,
        const FILETIME& time );

    virtual BOOL DeleteIVFile(
        const char* pPath);

    virtual BOOL TellPreAlarmTime(int time);

protected:
    enum IVBufEvent
    {
        SaveFile_Event = 0,
        OpenFile_Event = 1,
        CloseFile_Evnet = 2,
        Event_Count,
    };

    static size_t WINAPI SaveFileThread(void* pParm);
    size_t SaveFileLoopFun();

    inline HANDLE GetMyWantEvent(IVBufEvent e, int nChannelID);
    inline DWORD GetEventCount();
    
    void DoSaveFileEvent(DWORD dwChannel);
    void DoOpenFileEvevt(DWORD dwChannel);
    void DoCloseFileEvent(DWORD dwChannel);

    int FindBuf();

protected:
    struct FileInfo;
    class ChannelTarget;

protected:
    //typedef map<int, ChannelTarget> AllChannelTarget;
    typedef ChannelTarget* AllChannelTarget;

    AllChannelTarget m_TargetMap;
    CriticalSection m_cs;

   
    HANDLE m_Thread;
    size_t m_nThreadID;

    // if m_nDeviceCount=1,m_nEveryDeviceChannelNum=2
    // m_Event[2*3+1] = {
    //       WAIT_OBJECT_0,
    //      _Save,_Open,_Close,    // chan0
    //      _Save,_Open,_Close  }; // chan1
    HANDLE* m_Event; // Save File, Open/close File +
    DWORD m_nDeviceCount;
    DWORD m_nEveryDeviceChannelNum;
    BOOL m_IsInit;

    int m_nPreAlarmTime;
    DWORD m_dwMaxBufCount;

protected:
    TargetQueue* m_pTargetBuf;
    WORD m_nLastPos;
};



#endif  // End of file