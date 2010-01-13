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
    virtual BOOL GetData(
        int nChannelID,
        const FILETIME& time,
        TargetQueue*& DataQueue );

    // IIVDataSender
public:
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen );

    // IIVDataSaver
public:
    // ��Ƶ�ļ���һ��дʱ�ص���������������ļ������·���Ϳ�ʼ��ʱ��
    virtual BOOL Open(
        int nChannelID,
        const char* pPath,
        const FILETIME& time );

    // ��Ƶ�ļ��ر�ʱ���ص�
    virtual BOOL Close(
        int nChannelID,
         const FILETIME& time );

    virtual BOOL DeleteIVFile(
        const char* pPath);

protected:
    static size_t WINAPI SaveFileThread(void* pParm);
    size_t SaveFileLoopFun();

protected:
    class ChannelTarget
    {
    public:
        ChannelTarget()
        {
            iterIndex = TargetList.end();
        }

        GroupTarget* Find(const FILETIME& time);

        inline void PoshBack(GroupTarget* pGroupTarget);

    public:
        list<GroupTarget*> TargetList;
        list<GroupTarget*>::iterator iterIndex;
    };

    typedef map<int, ChannelTarget> AllChannelTarget;

    AllChannelTarget m_TargetMap;
    CriticalSection m_cs;

    HANDLE m_Thread;
    HANDLE m_Event[3]; // Open/close File + 
};




#endif  // End of file