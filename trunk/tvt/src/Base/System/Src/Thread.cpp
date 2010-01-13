/*H***************************************************************************
 File            : Thread.cpp
 Subsystem       : 
 Function Name(s): CThread
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 16:36
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "Thread.h"


using namespace OCI;

CriticalSection::CriticalSection()
{
    InitializeCriticalSection(&critical_section);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection(&critical_section);
}

void CriticalSection::Lock()
{
    EnterCriticalSection(&critical_section);
}

void CriticalSection::Unlock()
{
    LeaveCriticalSection(&critical_section);
}

//
// **************** AutoCriticalSection ******************
//
AutoCriticalSection::AutoCriticalSection(
    CRITICAL_SECTION& critical_section )
    : m_critical(critical_section)
{
    EnterCriticalSection(&m_critical);
}

AutoCriticalSection::AutoCriticalSection(
    CriticalSection& critical_section )
    : m_critical(critical_section.critical_section)
{
    EnterCriticalSection(&m_critical);
}

AutoCriticalSection::~AutoCriticalSection()
{
    LeaveCriticalSection(&m_critical);
}






// End of file