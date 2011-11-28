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
    *@note 构造直接
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
    *@note 创建线程，如果以前已经创建则先杀掉原先那个，再创建.线程开始跑
    */
    void Start(IRunnable* pRunable, void* pParam);

    /**
    *@note 杀掉线程，不管线程是否还在远行
    *@remark 不建议调用这个，正常的线程结束应该是线程函数正常跑完
    */
    int End(DWORD nErrorCode);

    /**
    *@note 暂停线程，调用后该线程将不会被分配CPU时间片，
    *@remark #1 如果线程还没有创建，则Rlease无影响，Debug报Assert
    *@remark #2 该方法如果某线程加锁会导致死锁  
    */
    void Pause();

    /**
    *@note 恢复线程，调用后该线程将不会被分配CPU时间片
    *@remark 如果线程还没有创建或者没有被暂停，则Rlease无影响，Debug报Assert
    */
    void Resume();

    /**
    *@note 设置线程的优化级
    */
    void SetPriority(Priority p);
    
    /**
    *@note 得到线程ID
    */
    ID GetId() const;

    /**
    *@note 一直等待他这个线程结束
    */
    void Join();

    /**
    *@note 等待指定时间，如果超时改时间则返回
    *@param nWaitTime 等待的时间，以毫秒为单位 
    *@return true为在时间内结束，flase为超时返回
    */
    bool TimedJoin(DWORD nWaitTime);

    /**
    *@note 等待某个事件，直到事件发生则返回，如果设定的时间内没有发生也返回
    *@param  ？
    *@param nWaitTime 等待的最大时间，以毫秒为单位 
    *@return true为在时间内结束，flase为超时返回
    */
    bool EventJoin(DWORD nWaitTime);

    // 得到该机器硬件线程数，比如AMD的就是几盒几线程，Intel如果有超线程就是*2
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
    *@note 睡指定时间，之后直接获得时间片
    */
    void Sleep(DWORD nSleepTime);

    /**
    *@note 让出当前的时间片
    */
    void Yield();

    /**
    *@note 退出线程，这个跟CThread不一样，CThread是强制杀掉
    */
    void End(DWORD nErrorCode);

    /**
    *@note 得到当前线程的ID
    */
    ID GetId();
};

}









#endif  // _THREAD_H_2011_11


// End of file



