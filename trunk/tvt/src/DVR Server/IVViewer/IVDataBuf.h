/*H***************************************************************************
 File            : IVDataBuf.h
 Subsystem       : 
 Function Name(s): CIVDataBuf
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 13:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVDATABUF_H_2010_
#define _IVDATABUF_H_2010_


// 模型采用发线程消息模式，
// 但是这种方式有个缺陷，就是通知比较好，取数据不怎么好。比如现在的GetData
// 发线程消息由于可能会失败，所以要写一份采用锁的方式就实现的代码
// 数据队列使用deque，然后去记录访问的索引，当有队列pop或push时，记录偏移
// 实际操作数据采用索引+offset，当某些特殊的情况然后清零offset。
class CIVDataBuf :
    public Singleton<CIVDataBuf>,
    public IIVDataBuf, 
    public IIVDataSender,
    public IIVDataSaver
{
public:
    CIVDataBuf(void);
    virtual ~CIVDataBuf(void);

    // IIVDataBuf
public:
    virtual TargetQueue* GetData(
        int nChannelID,
        const FILETIME& time );

    // IIVDataSender
public:
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen );

    virtual BOOL Init(
        int nDeviceCount,
        int nEveryDeviceChannelNum );

    virtual BOOL Unit();

    //// bState=true显示目标和路径，否则隐藏
    //virtual BOOL ShowObjTrace(
    //    int nChannelID,
    //    bool bState);

    //// 得到目标和路径是否正在显示
    //virtual BOOL GetObjTraceState(
    //    int nChannelID,
    //    bool& bState);

    // IIVDataSaver
public:
    // 视频文件第一次写时回调，告诉智能这边文件保存的路径和开始的时间
    virtual BOOL Open(
        int nChannelID,
        const char* pPath,
        const FILETIME& time );

    // 视频文件关闭时，回调
    virtual BOOL Close(
        int nChannelID,
        const FILETIME& time );

    virtual BOOL DeleteIVFile(
        const char* pPath);

    virtual BOOL TellPreAlarmTime(int time);

protected:
    enum IVBufEvent
    {
        SaveFile_Event = 0,
        OpenFile_Event = 1,
        CloseFile_Evnet = 2,
        Event_Count,
    };

    static size_t WINAPI SaveFileThread(void* pParm);
    size_t SaveFileLoopFun();

    inline HANDLE GetMyWantEvent(IVBufEvent e, int nChannelID);
    inline DWORD GetEventCount();
    
    void DoSaveFileEvent(DWORD dwChannel);
    void DoOpenFileEvevt(DWORD dwChannel);
    void DoCloseFileEvent(DWORD dwChannel);

    int FindBuf();

protected:
    class ChannelTarget
    {
    public:
        ChannelTarget()
        {
            //iterIndex = TargetList.end();
        }
        ~ChannelTarget()
        {
            StlHelper::STLDeleteSequence(TargetList);
            StlHelper::STLDeleteSequence(TargetSaveList);
        }

        GroupTarget* Find(const FILETIME& time);

        inline void PushBack(GroupTarget* pGroupTarget);

        long GetTimeMaxBetween();

    public:

        list<GroupTarget*> TargetList;      // 智能的还没有显示的数据队列
        //CFile File;

        list<GroupTarget*> TargetSaveList;  // 已经播放或者数据已不可能显示智能数据队列
        string strNewFilePath;
    };

    //typedef map<int, ChannelTarget> AllChannelTarget;
    typedef ChannelTarget* AllChannelTarget;

    AllChannelTarget m_TargetMap;
    CriticalSection m_cs;

   
    HANDLE m_Thread;
    size_t m_nThreadID;

    // if m_nDeviceCount=1,m_nEveryDeviceChannelNum=2
    // m_Event[2*3+1] = {
    //       WAIT_OBJECT_0,
    //      _Save,_Open,_Close,    // chan0
    //      _Save,_Open,_Close  }; // chan1
    HANDLE* m_Event; // Save File, Open/close File +
    DWORD m_nDeviceCount;
    DWORD m_nEveryDeviceChannelNum;
    BOOL m_IsInit;

    int m_nPreAlarmTime;

protected:
    TargetQueue* m_pTargetBuf;
    WORD m_nLastPos;
};




#endif  // End of file