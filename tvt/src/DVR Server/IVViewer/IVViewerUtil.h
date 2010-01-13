/*H***************************************************************************
 File            : IVViewerUtil.h
 Subsystem       : 
 Function Name(s): CIVViewerUtil
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 14:11
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVVIEWERUTIL_H_2010_
#define _IVVIEWERUTIL_H_2010_
#include "stdafx.h"


typedef deque<WPG_Target> TargetQueue;

//template<typename T>
//class SmartRef :
//    public T
//{
//public:
//    SmartRef():m_nRef(0){}
//public:
//    long AddRef()
//    {
//        return ++m_nRef;
//    }
//
//    long Release()
//    {
//        return --m_nRef;
//    }
//protected:
//    long m_nRef;
//};

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

//typedef deque< WPG_Target > TargetQueue;

struct GroupTarget
{
    GroupTarget(const FILETIME& time, const WPG_Target* pTaeget, size_t nLen)
    {
        this->m_time = time;
        for (size_t i=0; i<nLen; ++i)
        {
            m_TargetQueue.push_back(pTaeget[i]);
        }
    }

    bool operator < (const FILETIME& a) const
    {
        return this->m_time < a;
    }
    bool operator > (const FILETIME& a) const
    {
        return this->m_time > a;
    }
    bool operator == (const FILETIME& a) const
    {
        return this->m_time == a;
    }

    FILETIME m_time;
    TargetQueue m_TargetQueue;
};


struct IIVDataBuf
{
    virtual BOOL GetData(
        int nChannelID,
        const FILETIME& time,
        TargetQueue*& DataQueue)=0;
};

#endif  // End of file