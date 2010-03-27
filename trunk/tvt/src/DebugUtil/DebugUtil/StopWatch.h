/**H**************************************************************************
 File            : StopWatch.h
 Subsystem       : 
 Function Name(s): CStopWatch
 Author          : YiQiu
 Date            : 2010-3-22  
 Time            : 14:02
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _STOPWATCH_H_2010_3
#define _STOPWATCH_H_2010_3
#pragma once


#ifdef DEBUGUTIL_EXPORTS
#else
    #if !defined(_DEBUG)
        #pragma comment(lib, "DebugUtil.lib")
        #pragma message("Automatically linking with DebugUtil.lib")
    #elif defined(_DEBUG) /*&& defined(MEMORY_CHECK)*/
        #ifndef DEBUGUTIL_LINK
        #define DEBUGUTIL_LINK
            #pragma comment(lib, "DebugUtil_Debug.lib")
            #pragma message("Automatically linking with DebugUtil_Debug.lib") 
        #endif
    #endif
#endif


class CStopWatchBase
{
private:
    static class CStopWatchIniter
    {
    public:
        CStopWatchIniter();
    } Initer;
protected:
    static LARGE_INTEGER s_nCpuClcok;
};

class CStopWatch :
    public CStopWatchBase
{
public:
    CStopWatch(string strFunName);
    ~CStopWatch()
    {
        LARGE_INTEGER nLeaveTime;
        QueryPerformanceCounter(&nLeaveTime);
        double nTime = double(nLeaveTime.QuadPart-m_nEnterTime.QuadPart)/s_nCpuClcok.QuadPart;
        TRACE("%s time is %f\n", m_strFunName, nTime);
    }
private:
    string m_strFunName;
    LARGE_INTEGER m_nEnterTime;
};


class CStopWatchCallTest:
    public CStopWatchBase
{
public:
    CStopWatchCallTest()
    {
        LARGE_INTEGER nEnterTime;
        QueryPerformanceCounter(&nEnterTime);
        if ( s_nPreClcok.QuadPart == 0 )
        {
            s_nPreClcok = nEnterTime;
        }
        else
        {
            /*double a = pow(nEnterTime.QuadPart-s_nPreClcok.QuadPart, 2);
            double b = pow(s_nCpuClcok.QuadPart, 2);
            double c = a/b;
            double nTime = c*c;*/

            double nTime = long double(nEnterTime.QuadPart-s_nPreClcok.QuadPart)/s_nCpuClcok.QuadPart;
            //if ( nTime > 0.15 )
            {
                TRACE("time is %lf\n", nTime);
            }
            s_nPreClcok = nEnterTime;
        }
    }
private:  
    static LARGE_INTEGER s_nPreClcok;
};


//#define StartStopWatch()  CStopWatch ____stop(__FUNCTION__)
#define StartStopWatch() 


template<int t>
class CFrameCount :
    public CStopWatchCallTest
{
public:
    CFrameCount()
    {
        LARGE_INTEGER nEnterTime;
        QueryPerformanceCounter(&nEnterTime);
        if ( s_nStartClcok.QuadPart == 0 )
        {
            s_nStartClcok = nEnterTime;
            s_nPreClcok = nEnterTime;
            TRACE("Frame Test Start!\n");
        }
        else
        {
            double nTwoFrameTime = double(nEnterTime.QuadPart-s_nPreClcok.QuadPart)/s_nCpuClcok.QuadPart;
            s_nPreClcok = nEnterTime;
            double nTatalFrameTime = double(nEnterTime.QuadPart-s_nStartClcok.QuadPart)/s_nCpuClcok.QuadPart;
            TRACE(
                "last Two Frame Between is %f\nTatal Average Frame Ratio Time is %f\n",
                nTwoFrameTime, s_dwFrameCount/nTatalFrameTime);
        }
         ++s_dwFrameCount;
    }
protected:
    static DWORD s_dwFrameCount;
    static LARGE_INTEGER s_nPreClcok;
    static LARGE_INTEGER s_nStartClcok;
};

template<int t>
DWORD CFrameCount<t>::s_dwFrameCount = 0;

template<int t>
LARGE_INTEGER CFrameCount<t>::s_nPreClcok = {0};

template<int t>
LARGE_INTEGER CFrameCount<t>::s_nStartClcok = {0};














#endif  // _STOPWATCH_H_2010_


// End of file



