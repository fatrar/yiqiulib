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


enum
{
    Every_Device_Channel = 4,
    Max_Device_Num = 8,
    MAX_IV_Channel = 32,
    MAX_Obj_Trace_Point = 64, // 6√Î

    // 1√Î10÷°£¨ª∫¥Ê5√Î£¨4∏ˆÕ®µ¿
    MAX_IV_BUF_Size = 10*5*4,  // all buf size = 4804*200 .= 800K 

    Buf_No_Use      = 0,
    NO_BUF_Ramain   = -1,

};


struct TargetQueue
{
    TargetQueue():nCount(0),nUse(Buf_No_Use),nRef(0){}
    
    WPG_Target Tar[TARGET_MAX_NUM];
    WORD nCount;
    BYTE nRef;
    BYTE nUse;
    CriticalSection cs;

    BYTE AddRef()
    {
        AutoLockAndUnlock(cs);
        return ++nRef;
    }

    BYTE Release()
    {
        AutoLockAndUnlock(cs);
        --nRef;
        if ( nRef == 0 )
        {
            nUse = Buf_No_Use;
        }
        return nRef;
    }
};


//typedef deque<WPG_Target> TargetQueue;


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

// return ∫¡√Î
inline long operator - (const FILETIME&t1, const FILETIME&t2)
{
    return (*((__int64*)&t1)-*((__int64*)&t2))/10000;
}
//typedef deque< WPG_Target > TargetQueue;

struct GroupTarget
{
    GroupTarget(
        const FILETIME& time, 
        const WPG_Target* pTaeget,
        size_t nLen,
        TargetQueue* pBuf )
    {
        this->m_time = time;
        m_TargetQueue = pBuf;
        memcpy(pBuf->Tar, pTaeget, nLen*sizeof(WPG_Target));
        pBuf->nCount = nLen;
        pBuf->nUse = 1;      
    }

  /* ???[] template<typename oper>
    bool operator oper(const FILETIME& a)  const
    { return this->m_time oper a;}*/
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
    TargetQueue* m_TargetQueue;
};


struct IIVDataBuf
{
    virtual TargetQueue* GetData(
        int nChannelID,
        const FILETIME& time )=0;
};

#endif  // End of file