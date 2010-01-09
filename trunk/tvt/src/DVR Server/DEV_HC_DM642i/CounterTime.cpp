// CounterTime.cpp: implementation of the CCounterTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CounterTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCounterTime::CCounterTime()
{
	LARGE_INTEGER litmp;
	SYSTEMTIME sysTime;

	QueryPerformanceFrequency(&litmp);
	m_dfFrequency = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	m_llRefCounter = litmp.QuadPart;

	GetLocalTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &m_ftRefTime);
}

CCounterTime::~CCounterTime()
{

}

FILETIME CCounterTime::GetCurrentTime()
{
	LARGE_INTEGER litmp;
	LONGLONG llCurrentCounter;
	double dfMinusCounter;
	double dfMinusTime;
	ULONGLONG llCurrentTime;

	QueryPerformanceCounter(&litmp);
	llCurrentCounter = litmp.QuadPart;
	dfMinusCounter = (double)(llCurrentCounter - m_llRefCounter);
	dfMinusTime = (dfMinusCounter / m_dfFrequency) * 10000000;
	llCurrentTime = (*(ULONGLONG *)&m_ftRefTime + (ULONGLONG)dfMinusTime);

	return *(FILETIME *)&llCurrentTime;
}
