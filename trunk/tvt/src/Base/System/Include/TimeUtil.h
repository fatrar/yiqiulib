/**H**************************************************************************
 File            : TimeUtil.h
 Subsystem       : 
 Function Name(s): CTimeUtil
 Author          : YiQiu
 Date            : 2010-3-18  
 Time            : 15:53
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _TIMEUTIL_H_2010_3
#define _TIMEUTIL_H_2010_3
#pragma once


BEGIN_BASE_ENGINE


inline bool operator ==(const FILETIME&t1, const FILETIME&t2) 
{
    return CompareFileTime(&t1, &t2) == 0;
}

inline bool operator !=(const FILETIME&t1, const FILETIME&t2) 
{
    return CompareFileTime(&t1, &t2) != 0;
}

inline bool operator < (const FILETIME&t1, const FILETIME&t2) 
{
    return CompareFileTime(&t1, &t2) < 0;
}

inline bool operator > (const FILETIME&t1, const FILETIME&t2) 
{
    return CompareFileTime(&t1, &t2) > 0;
}

inline long operator ^ (const FILETIME&t1, const FILETIME&t2) 
{
    return CompareFileTime(&t1, &t2);
}

// return ºÁÃë
inline __int64 operator - (const FILETIME&t1, const FILETIME&t2)
{
    return (*((__int64*)&t1)-*((__int64*)&t2))/10000;
}

inline bool IsValidSystemTime(
    const SYSTEMTIME& syt,
    bool bCheckDayOfWeek = true,
    bool bCheckYear = true)
{
    if ( bCheckDayOfWeek )
    {
        if ( syt.wDayOfWeek >= 7 )
        {
            return false;
        }
    }

    if ( bCheckYear )
    {
        if ( syt.wYear < 1900 ||
             syt.wYear > 2100 )
        {
            return false;
        }
    }

    if ( syt.wMonth < 1 || syt.wMonth > 12 ||
         syt.wDay < 1 || syt.wDay > 31 ||
         syt.wHour >= 24 ||
         syt.wMinute >= 60 ||
         syt.wSecond >= 60 ||
         syt.wMilliseconds >= 1000 )
    {
        return false;
    }

    return true;
}

namespace TimeUtil
{

};





END_BASE_ENGINE // namespace


#endif  // _TIMEUTIL_H_2010_


// End of file



