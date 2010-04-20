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

class CIVLiveDataBuf;

struct CIVLiveDataBuf::FileInfo
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


class CIVLiveDataBuf::ChannelTarget
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

    /**
    *@note 下面两个函数的功能是将数据放查询队列和根据查询队列查询数据
    *      查询队列的线程锁由上层做 
    */
    GroupTarget* Find(const FILETIME& time);
    void PushBack(GroupTarget* pGroupTarget){TargetList.push_back(pGroupTarget);}

    /**
    *@note 下面两个函数的功能是上层发现有文件打开或关闭，将信息传到这边
    */
    void NewFileComing(
        const char* pPath,
        const FILETIME& time)
    {
        AutoLockAndUnlock(cs);
        FilePathList.push_back( FileInfo(pPath, time) );
    }
    bool FileClose(const FILETIME& time);

    void TrySaveData(int nPreAlarmTime);
protected:


protected:
    void FillHeadToFile(
        const FILETIME& OpenTime )
    {
        FileHead.BeginTime = OpenTime;
        Writer.write((char*)&FileHead, sizeof(IVFileHead));
        UpdatePos();
    }

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

    void UpdateDataIndexToFile(const FILETIME& EndTime);

protected:
    typedef list<GroupTarget*> TTargetList;
    typedef deque<FileInfo> FileInfoList;

private:

    TTargetList TargetList;      // 智能的还没有显示的数据队列
    TTargetList TargetSaveList;  // 已经播放或者数据已不可能显示智能数据队列

    ofstream Writer;
    DWORD dwPrePos;
    DWORD dwCurPos;

    /**
    *@note 下面三个参数主要为了保存索引数据，即IV文件头
    *@param FileHead 算法在默认分配一个IV文件头，每次写文件头(包括打开文件和关闭),
    *     都用这数据做缓存数据写进文件缓存
    *@param dwCount 用来多少个数据才保存一次文件索引，
    *     默认定义为Head_Index_Interval，这个值只是记录有多少次了
    *@param MoreDataIndex 毕竟还是有可能FileHead(默认Max_IVData_Index)的索引数据会写完，
    *     写FileHead写完了，就将索引放进MoreDataIndex，
    *     这个时候在文件保存时需要将FileHead和MoreDataIndex的索引数据在平均丢弃，
    *     算法见对于处理函数UpdateDataIndexToFile的注解
    */
    IVFileHead FileHead;
    DWORD dwCount;
    deque<IVFileDataIndex> MoreDataIndex;

    CriticalSection cs;
    FileInfoList FilePathList;
};


class CIVPlaybackDataBuf;

//#define _FixIVFile

class CIVPlaybackDataBuf::ChannelTarget
{
public:
#ifdef _FixIVFile
    static void Init();
    static void Unit();
#endif // _FixIVFile

    BOOL Open(
        const char* pPath,
        const FILETIME& time);

    BOOL Close(const FILETIME& time);

    BOOL MoveTo(const FILETIME& time);

protected:
    template<IVFileVersionDefine Version>
    BOOL PraseHeadToMap(const IVFileHead& Head);

    size_t GetPos(const FILETIME& time);
    //size_t GuessPos(const FILETIME& time);
private:
    ifstream Reader;
    //BOOL IsFoundFile;
    DWORD dwCurrentPos;
    FILETIME BeginTime;
    FILETIME EndTime;
    deque<IVFileDataIndex> DataIndex;

    BYTE* m_pDataBuf;
#ifdef _FixIVFile
protected:
    static DWORD WINAPI OnFixFileThread(void* p);

private:
    static HANDLE s_hFixFileThread;
    static DWORD s_dwFixFileThread;
#endif // _FixIVFile

};


#endif  // _CHANNELTARGET_H_2010_


 // End of file



