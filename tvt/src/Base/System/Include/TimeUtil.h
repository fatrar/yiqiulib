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

// return ∫¡√Î
inline __int64 operator - (const FILETIME&t1, const FILETIME&t2)
{
    return (*((__int64*)&t1)-*((__int64*)&t2))/10000;
}


namespace TimeUtil
{

};





END_BASE_ENGINE // namespace


#endif  // _TIMEUTIL_H_2010_


// End of file



