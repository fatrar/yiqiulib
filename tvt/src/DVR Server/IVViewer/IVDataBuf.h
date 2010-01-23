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
    virtual BOOL GetData(
        int nChannelID,
        const FILETIME& time,
        TargetQueue*& DataQueue );

    // IIVDataSender
public:
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen );

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
    size_t SaveFileLoopFun();

protected:
    class ChannelTarget
    {
    public:
        ChannelTarget()
        {
            iterIndex = TargetList.end();
        }

        GroupTarget* Find(const FILETIME& time);

        inline void PushBack(GroupTarget* pGroupTarget);

    public:
        list<GroupTarget*> TargetList;
        list<GroupTarget*>::iterator iterIndex;
        //CFile File;
        string strNewFilePath;
    };

    typedef map<int, ChannelTarget> AllChannelTarget;

    AllChannelTarget m_TargetMap;
    CriticalSection m_cs;

   
    HANDLE m_Thread;
    HANDLE m_Event[3]; // Open/close File + 
};




#endif  // End of file