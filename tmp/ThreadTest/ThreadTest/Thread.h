/**H**************************************************************************
 File            : Thread.h
 Subsystem       : 
 Function Name(s): CThread
 Author          : YiQiu
 Date            : 2011-11-28  
 Time            : 17:16
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen Team Oleaster.
***************************************************************************H*/
#ifndef _THREAD_H_2011_11
#define _THREAD_H_2011_11
//#include "Common.h"
//#include "DebugUtil.h"
#define Assert 

namespace Thread
{
typedef unsigned int ID;
typedef unsigned long DWORD;

#ifdef WIN32
    typedef void* HANDLE;
    typedef HANDLE THANDLE;
#elif defined(__APPLE__) || defined(__SYMBIAN32__) || defined(Andriod)
    typedef int THANDLE;
#else
#   error Thread unsupport system
#endif


struct IRunnable
{
    virtual DWORD Run(void* p) = 0;
};

class CThread
{
public:
    /**
    *@note ����ֱ��
    */
    CThread()
        : m_Handle(0)
        , m_IsCreate(false){}
    ~CThread(void)
    {
        Assert(0 == m_Handle);
    };

    enum Priority
    {
        Priority_Realtime,
        Priority_Highest,
        Priority_HigherNormal,
        Priority_Normal,
        Priority_BelowerNormal,
        Priority_Lowest,
        Priority_IDLE,
        Priority_Count,
    };
public:
    inline bool IsCreate(){return m_IsCreate;};

    /**
    *@note �����̣߳������ǰ�Ѿ���������ɱ��ԭ���Ǹ����ٴ���.�߳̿�ʼ��
    */
    void Start(IRunnable* pRunable, void* pParam);

    /**
    *@note ɱ���̣߳������߳��Ƿ���Զ��
    *@remark ���������������������߳̽���Ӧ�����̺߳�����������
    */
    int End(DWORD nErrorCode);

    /**
    *@note ��ͣ�̣߳����ú���߳̽����ᱻ����CPUʱ��Ƭ��
    *@remark #1 ����̻߳�û�д�������Rlease��Ӱ�죬Debug��Assert
    *@remark #2 �÷������ĳ�̼߳����ᵼ������  
    */
    void Pause();

    /**
    *@note �ָ��̣߳����ú���߳̽����ᱻ����CPUʱ��Ƭ
    *@remark ����̻߳�û�д�������û�б���ͣ����Rlease��Ӱ�죬Debug��Assert
    */
    void Resume();

    /**
    *@note �����̵߳��Ż���
    */
    void SetPriority(Priority p);
    
    /**
    *@note �õ��߳�ID
    */
    ID GetId() const;

    /**
    *@note һֱ�ȴ�������߳̽���
    */
    void Join();

    /**
    *@note �ȴ�ָ��ʱ�䣬�����ʱ��ʱ���򷵻�
    *@param nWaitTime �ȴ���ʱ�䣬�Ժ���Ϊ��λ 
    *@return trueΪ��ʱ���ڽ�����flaseΪ��ʱ����
    */
    bool TimedJoin(DWORD nWaitTime);

    /**
    *@note �ȴ�ĳ���¼���ֱ���¼������򷵻أ�����趨��ʱ����û�з���Ҳ����
    *@param  ��
    *@param nWaitTime �ȴ������ʱ�䣬�Ժ���Ϊ��λ 
    *@return trueΪ��ʱ���ڽ�����flaseΪ��ʱ����
    */
    bool EventJoin(DWORD nWaitTime);

    // �õ��û���Ӳ���߳���������AMD�ľ��Ǽ��м��̣߳�Intel����г��߳̾���*2
    //static unsigned hardware_concurrency();

    //typedef platform-specific-type native_handle_type;
    //native_handle_type native_handle();

    //void interrupt();
    // interruption_requested() const;

    // backwards compatibility
    //bool operator==(const thread& other) const;
    //bool operator!=(const thread& other) const;

protected:
    static DWORD __stdcall Run(void* p)
    {
        CThread* pThis = (CThread*)p;
        return pThis->m_pRunable->Run(p);
    };

protected:
    bool m_IsCreate;
    THANDLE m_Handle;
    Priority m_Priority;
    IRunnable* m_pRunable;
};


namespace ThisThread
{
    /**
    *@note ˯ָ��ʱ�䣬֮��ֱ�ӻ��ʱ��Ƭ
    */
    void Sleep(DWORD nSleepTime);

    /**
    *@note �ó���ǰ��ʱ��Ƭ
    */
    void Yield();

    /**
    *@note �˳��̣߳������CThread��һ����CThread��ǿ��ɱ��
    */
    void End(DWORD nErrorCode);

    /**
    *@note �õ���ǰ�̵߳�ID
    */
    ID GetId();
};

}









#endif  // _THREAD_H_2011_11


// End of file



