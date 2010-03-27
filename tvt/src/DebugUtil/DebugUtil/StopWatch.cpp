/**CPP*************************************************************************
 File            : StopWatch.cpp
 Subsystem       : 
 Function Name(s): CStopWatch
 Author          : YiQiu
 Date            : 2010-3-22  
 Time            : 14:06
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "StopWatch.h"

CStopWatchBase::CStopWatchIniter CStopWatchBase::Initer;
LARGE_INTEGER CStopWatchBase::s_nCpuClcok;


CStopWatchBase::CStopWatchIniter::CStopWatchIniter()
{
    QueryPerformanceFrequency(&s_nCpuClcok);
}

CStopWatch::CStopWatch(string strFunName)
{
    QueryPerformanceCounter(&m_nEnterTime);
    m_strFunName = strFunName;
}


//CStopWatchCallTest::CStopWatchCallTest()
//{
//
//}


LARGE_INTEGER CStopWatchCallTest::s_nPreClcok = {0};

//void CStopWatchCallTest::Init()
//{
//    QueryPerformanceFrequency(&s_nCpuClcok);
//    s_nPreClcok.QuadPart = 0;
//}



// End of file