/**H**************************************************************************
 File            : ChannelTarget.h
 Subsystem       : 
 Function Name(s): CChannelTarget
 Author          : YiQiu
 Date            : 2010-3-24  
 Time            : 11:08
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _CHANNELTARGET_H_2010_3
#define _CHANNELTARGET_H_2010_3


struct CIVDataBuf::FileInfo
{
    FileInfo(
        const char* pPath, 
        const FILETIME& time)
        :Path(pPath),OpenTime(time),IsColse(FALSE){}
    FileInfo(){}
    FileInfo& operator = (const FileInfo& a)
    {
        Path      = a.Path;
        OpenTime  = a.OpenTime;
        CloseTime = a.CloseTime;
        IsColse   = a.IsColse;
        return *this;
    }
    string Path;
    FILETIME OpenTime;
    FILETIME CloseTime;
    BOOL IsColse;
};


class CIVDataBuf::ChannelTarget
{
public:
    ChannelTarget()
        : dwPrePos(0)
        , dwCurPos(0)
        , dwCount(0){}

    ~ChannelTarget()
    {
        StlHelper::STLDeleteSequence(TargetList);
        StlHelper::STLDeleteSequence(TargetSaveList);
    }

    GroupTarget* Find(const FILETIME& time);

    void PushBack(GroupTarget* pGroupTarget);

    void NewFileComing(
        const char* pPath,
        const FILETIME& time);

    void FillHeadToFile(
        const FILETIME& OpenTime )
    {
        FileHead.BeginTime = OpenTime;
        Writer.write((char*)&FileHead, sizeof(IVFileHead));
        UpdatePos();
    }

    bool FileClose(const FILETIME& time);

    void TrySaveData(int nPreAlarmTime);

protected:
    enum
    {
        Head_Index_Interval = 10,
    };

protected:
    void DropSomeData(int nPreAlarmTime);

    void SaveData(
        int nPreAlarmTime,
        FileInfo* Info = NULL);

    void SaveDataHeadToFile(
        IVFileDataHead& DataHead, 
        WORD nTargetCount,
        const FILETIME& t);

    void SaveTargetToFile(
        const TargetQueue* m_TargetQueue )
    {
        size_t nSize = m_TargetQueue->nCount*sizeof(WPG_Target);
        Writer.write((char*)&m_TargetQueue->Tar, nSize);
    }

    void UpdatePos()
    {
        dwPrePos = 0;
        dwCurPos = sizeof(IVFileHead);
    }

    void UpdateFileInfo()
    {
        FilePathList.pop_front();
    }

    void ResetFileHead()
    {
        FileHead.FileFlag = g_dwIVFileFail;
        FileHead.dwIndexNum = 0;
    }

    void UpdateDataIndex(
        const FILETIME& t,
        DWORD DataOffset);

    void UpdateDataIndexToFile();

protected:
    typedef deque<FileInfo> FileInfoList;

public:

    TTargetList TargetList;      // 智能的还没有显示的数据队列
    TTargetList TargetSaveList;  // 已经播放或者数据已不可能显示智能数据队列

    ofstream Writer;
    DWORD dwPrePos;
    DWORD dwCurPos;

    IVFileHead FileHead;
    DWORD dwCount;
    deque<IVFileDataIndex> MoreDataIndex;

    CriticalSection cs;
    FileInfoList FilePathList;
};









#endif  // _CHANNELTARGET_H_2010_


 // End of file



