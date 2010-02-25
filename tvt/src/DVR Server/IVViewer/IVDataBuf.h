/*H***************************************************************************
 File            : IVDataBuf.h
 Subsystem       : 
 Function Name(s): CIVDataBuf
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 13:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVDATABUF_H_2010_
#define _IVDATABUF_H_2010_


// ģ�Ͳ��÷��߳���Ϣģʽ��
// �������ַ�ʽ�и�ȱ�ݣ�����֪ͨ�ȽϺã�ȡ���ݲ���ô�á��������ڵ�GetData
// ���߳���Ϣ���ڿ��ܻ�ʧ�ܣ�����Ҫдһ�ݲ������ķ�ʽ��ʵ�ֵĴ���
// ���ݶ���ʹ��deque��Ȼ��ȥ��¼���ʵ����������ж���pop��pushʱ����¼ƫ��
// ʵ�ʲ������ݲ�������+offset����ĳЩ��������Ȼ������offset��
class CIVDataBuf :
    public Singleton<CIVDataBuf>,
    public IIVDataBuf, 
    public IIVDataSender,
    public IIVDataSaver
{
public:
    CIVDataBuf(void);
    virtual ~CIVDataBuf(void);

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

    //// bState=true��ʾĿ���·������������
    //virtual BOOL ShowObjTrace(
    //    int nChannelID,
    //    bool bState);

    //// �õ�Ŀ���·���Ƿ�������ʾ
    //virtual BOOL GetObjTraceState(
    //    int nChannelID,
    //    bool& bState);

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
    static size_t WINAPI SaveFileThread(void* pParm);
    size_t SaveFileLoopFun(HANDLE h);

    
    void DoSaveFileEvent(DWORD dwChannel);
    void DoOpenFileEvevt(DWORD dwChannel);
    void DoCloseFileEvent(DWORD dwChannel);

    int FindBuf();
protected:
    class ChannelTarget
    {
    public:
        ChannelTarget()
        {
            //iterIndex = TargetList.end();
        }
        ~ChannelTarget()
        {
            STLDeleteSequence(TargetList);
            STLDeleteSequence(TargetSaveList);
        }

        GroupTarget* Find(const FILETIME& time);

        inline void PushBack(GroupTarget* pGroupTarget);

        long GetTimeMaxBetween();


        bool SaveSome();

    public:
        list<GroupTarget*> TargetList;
        //list<GroupTarget*>::iterator iterIndex;
        //CFile File;

        list<GroupTarget*> TargetSaveList;
        string strNewFilePath;
    };

    //typedef map<int, ChannelTarget> AllChannelTarget;
    typedef ChannelTarget* AllChannelTarget;

    AllChannelTarget m_TargetMap;
    CriticalSection m_cs;

   
    HANDLE m_Thread;
    size_t m_nThreadID;

    HANDLE* m_Event; // Save File, Open/close File +
    int m_nDeviceCount;
    int m_nEveryDeviceChannelNum;
    BOOL m_IsInit;

    int m_nPreAlarmTime;

protected:
    TargetQueue* m_pTargetBuf;
    WORD m_nBufRemain;
    WORD m_nLastPos;
};




#endif  // End of file