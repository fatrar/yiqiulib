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
    Max_PreAlarm_Time = 60,    // 预录像最大60秒 maxbuf = ((60*10*4) *4808) .= 12MB, 4张卡1秒10帧数据，一个数据4808B
    Single_Device_Buf_Size = Max_PreAlarm_Time*10,
    //MAX_IV_BUF_Size = 10*5*4,  // all buf size = 4804*200 .= 800K 

    Buf_No_Use      = 0,
    NO_BUF_Ramain   = -1,

};

/**
*@note 目标数据的基本单位，这个结构只是在CIVDataBuf中一次创建固定数量的TargetQueue的缓存区。
*      以后在每次有数据来，都是CIVDataBuf中的缓存区找没有使用的，找到后调用AddRef()，表示有数据引用
*      当数据用完后调用Release()，
*/
class BaseTargetQueue
{
public:
    BaseTargetQueue()
        : nCount(0)
        , nUse(Buf_No_Use)
        , nRef(0) {}

    WPG_Target Tar[TARGET_MAX_NUM];
    WORD nCount;
    BYTE nRef;
    BYTE nUse;

    virtual BYTE Release() = 0;

protected:
    CriticalSection cs;
};

class LiveTargetQueue :
    public BaseTargetQueue
{
public:
    LiveTargetQueue()
        : hSaveEvent(NULL){}
    
public:
    /**
    *@note 第一次使用时，调用此函数，作用为初始化各参数等同于Init
    *  调用此函数后，找缓冲时，将不使用这个缓冲
    */
    inline void Use(HANDLE h)
    {
        AutoLockAndUnlock(cs);
        nUse = 1;   // 是否还被使用
        nRef = 0;   // 有多少个对象正在引用
        hSaveEvent=h;
    }

    inline BYTE AddRef()
    {
        AutoLockAndUnlock(cs);
        return ++nRef;
    }

    virtual BYTE Release()
    {
        AutoLockAndUnlock(cs);
        --nRef;
        if ( nRef == 0 )
        {
            //nUse = Buf_No_Use;
            if ( hSaveEvent )
            {
                SetEvent(hSaveEvent);
            }
        }
        return nRef;
    }

protected:
    HANDLE hSaveEvent;
};

struct PlayBackTargetQueue :
    public BaseTargetQueue
{
public:
    BYTE AddRef()
    {
        AutoLockAndUnlock(cs);
        if ( nRef == 0 )
        {
            nUse = 1;
        }
        return ++nRef;
    }
    virtual BYTE Release()
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


/**
*@note 保存一个基本目标数据队列和一个数据的时间
* 这个数据结构作为一个通道数据队列的基本单位，暂时这个数据在插进队列时，
* 先new,然后插进队列，数据用完然后delete
*/
template<typename T>
struct GroupTarget
{
    GroupTarget(
        const FILETIME& time, 
        const WPG_Target* pTaeget,
        size_t nLen,
        T* pBuf )
    {
        m_time = time;
        m_TargetQueue = pBuf;
        memcpy(pBuf->Tar, pTaeget, nLen*sizeof(WPG_Target));
        pBuf->nCount = nLen;
    }

    GroupTarget(
        const FILETIME& time,
        T* pBuf )
    {
        m_time = time;
        m_TargetQueue = pBuf;
    }

    bool operator < (const GroupTarget& a) const
    {
        return this->m_time < a.m_time;
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
    T* m_TargetQueue;
};

typedef GroupTarget<LiveTargetQueue> LiveGroupTarget;
typedef GroupTarget<PlayBackTargetQueue> PlayBackGroupTarget;

struct IIVDataBuf
{
    virtual BaseTargetQueue* GetData(
        int nChannelID,
        const FILETIME& time )=0;
};

template<typename T>
static size_t IVViewerFindBuf(
    size_t& nLastPos,
    T* pTargetBuf,  // BaseTargetQueue
    size_t nMaxBufCount )
{
    // 算法需要检验是否正确
    for (size_t i=nLastPos; i<nMaxBufCount; ++i)
    {
        if ( pTargetBuf[i].nUse == Buf_No_Use )
        {
            // 好像m_nLastPos为MAX_IV_BUF_Size也没问题
            nLastPos = i + 1;
            return i;
        }
    }

    for (size_t j=0; j<nLastPos; ++j)
    {
        if ( pTargetBuf[j].nUse == Buf_No_Use )
        {
            nLastPos = j + 1;
            return j;
        }
    }

    // log
    return NO_BUF_Ramain;
}

#endif  // End of file