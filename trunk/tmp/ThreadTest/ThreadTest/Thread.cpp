#include "StdAfx.h"
#include "Thread.h"

#include <Windows.h>
namespace Thread
{

void CThread::Start(
    IRunnable* pRunable, void* pParam)
{
    Assert(NULL != pRunable);
    m_pRunable = pRunable;
    m_Handle = ::CreateThread(NULL, 0, &CThread::Run, pParam, 0, NULL); 
}

int CThread::End( DWORD nErrorCode )
{
    int nRet = TerminateThread(m_Handle, nErrorCode);
    CloseHandle(m_Handle);
    m_Handle = NULL;
    return nRet;
}

void CThread::Pause()
{
    ::SuspendThread(m_Handle);
}

void CThread::Resume()
{
    ::ResumeThread(m_Handle);
}

void CThread::SetPriority( Priority p )
{   
    static int PriorityTable[Priority_Count] = 
    {
        THREAD_PRIORITY_TIME_CRITICAL,
        THREAD_PRIORITY_HIGHEST,
        THREAD_PRIORITY_ABOVE_NORMAL,
        THREAD_PRIORITY_NORMAL,
        THREAD_PRIORITY_BELOW_NORMAL,
        THREAD_PRIORITY_LOWEST,
        THREAD_PRIORITY_IDLE
    };
    ::SetThreadPriority(m_Handle, PriorityTable[p]);
}

Thread::ID CThread::GetId() const
{
    return GetThreadId(m_Handle);
}

void CThread::Join()
{
    WaitForSingleObject(m_Handle, INFINITE);
    CloseHandle(m_Handle);
    m_Handle = NULL;
}

bool CThread::TimedJoin( DWORD nWaitTime )
{
    bool bRet = (WAIT_TIMEOUT != WaitForSingleObject(m_Handle, nWaitTime));
    CloseHandle(m_Handle);
    m_Handle = NULL;
    return bRet;
}

}