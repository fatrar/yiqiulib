// CounterTime.h: interface for the CCounterTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COUNTERTIME_H__A43AD5A8_936B_4292_B4DE_21A394D860F7__INCLUDED_)
#define AFX_COUNTERTIME_H__A43AD5A8_936B_4292_B4DE_21A394D860F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCounterTime  
{
public:
	FILETIME GetCurrentTime();
	CCounterTime();
	virtual ~CCounterTime();

protected:
	FILETIME m_ftRefTime;
	LONGLONG m_llRefCounter;
	double m_dfFrequency;
};

#endif // !defined(AFX_COUNTERTIME_H__A43AD5A8_936B_4292_B4DE_21A394D860F7__INCLUDED_)
