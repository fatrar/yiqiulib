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
    MAX_Obj_Trace_Point = 64, // 6秒

    // 1秒10帧，缓存5秒，4个通道
    MAX_IV_BUF_Size = 10*5*4,  // all buf size = 4804*200 .= 800K 

    Buf_No_Use      = 0,
    NO_BUF_Ramain   = -1,

};

/**
*@note 目标数据的基本单位，这个结构只是在CIVDataBuf中一次创建固定数量的TargetQueue的缓存区。
*      以后在每次有数据来，都是CIVDataBuf中的缓存区找没有使用的，找到后调用AddRef()，表示有数据引用
*      当数据用完后调用Release()，
*/
struct TargetQueue
{
    TargetQueue()
        :nCount(0),
        nUse(Buf_No_Use),
        nRef(0),
        hSaveEvent(NULL){}
    
    WPG_Target Tar[TARGET_MAX_NUM];
    WORD nCount;
    BYTE nRef;
    BYTE nUse;
    HANDLE hSaveEvent;
    CriticalSection cs;

    inline void Use(HANDLE h)
    {
        AutoLockAndUnlock(cs);
        nUse = 1;
        nRef = 0;
        hSaveEvent=h;
    }

    inline BYTE AddRef()
    {
        AutoLockAndUnlock(cs);
        return ++nRef;
    }

    inline BYTE Release()
    {
        AutoLockAndUnlock(cs);
        --nRef;
        if ( nRef == 0 )
        {
            nUse = Buf_No_Use;
            if ( hSaveEvent )
            {
                SetEvent(hSaveEvent);
            }
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


//typedef deque< WPG_Target > TargetQueue;

/**
*@note 保存一个基本目标数据队列和一个数据的时间
* 这个数据结构作为一个通道数据队列的基本单位，暂时这个数据在插进队列时，
* 先new,然后插进队列，数据用完然后delete
*/
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

    long operator ^ (const FILETIME& a) const
    {
        return this->m_time ^ a;
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