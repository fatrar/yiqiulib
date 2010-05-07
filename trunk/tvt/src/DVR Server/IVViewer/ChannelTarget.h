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

struct FileSectionTime
{
    CFileTime BeginTime;
    CFileTime EndTime;
};

struct CIVLiveDataBuf::FileInfo
{
    FileInfo(
        const char* pPath)
        : strPath(pPath), bIsColse(FALSE){}
    FileInfo(){}
    FileInfo& operator = (const FileInfo& a)
    {
        strPath      = a.strPath;
        FinishSection = a.FinishSection;
        UnFinishSection = a.UnFinishSection;
        bIsColse   = a.bIsColse;
        return *this;
    }

    typedef deque<FileSectionTime> SectionTimeList;

    string strPath;
    SectionTimeList FinishSection;
    SectionTimeList UnFinishSection;
    CFileTime FrameBeginTime;
    CFileTime FrameEndTime;
    BOOL bIsColse;
};


class CIVLiveDataBuf::ChannelTarget
{
//public:
//    typedef LiveGroupTarget TGroupTarget;
public:
    ChannelTarget()
        : dwPrePos(0)
        , dwCurPos(0)
        , dwCount(0)
        , bIsFirstFrame(TRUE) {}

    ~ChannelTarget()
    {
        StlHelper::STLDeleteSequence(TargetList);
        StlHelper::STLDeleteSequence(TargetSaveList);
    }

    /**
    *@note �������������Ĺ����ǽ����ݷŲ�ѯ���к͸��ݲ�ѯ���в�ѯ����
    *      ��ѯ���е��߳������ϲ��� 
    */
    TGroupTarget* Find(const FILETIME& time);
    void PushBack(TGroupTarget* pGroupTarget){TargetList.push_back(pGroupTarget);}

    /**
    *@note �������������Ĺ������ϲ㷢�����ļ��򿪻�رգ�����Ϣ�������
    */
    void NewFileComing(
        const char* pPath )
    {
        AutoLockAndUnlock(cs);
        FilePathList.push_back( FileInfo(pPath) );
    }
    bool FileClose(const char* pPath);

    void TrySaveData(int nPreAlarmTime);

    void FinallySave();

    bool StartSection(
        const char* pPath, 
        const FILETIME& time);

    bool EndSection(
        const char* pPath, 
        const FILETIME& time);

protected:
    void SaveByFileIsSetClose(
        int nPreAlarmTime,
        FileInfo& Info);

    void SaveByFileIsNoSetClose(
        int nPreAlarmTime,
        FileInfo& Info);

    void FinallySaveAFile(
        FileInfo& Info);

protected:
    void DropSomeData(int nPreAlarmTime);
    void DropSomeData(int nPreAlarmTime, const FILETIME& Now);

    void UpdatePos()
    {
        dwPrePos = 0;
        dwCurPos = sizeof(IVFileHead);
    }

    void ResetFileHead()
    {
        FileHead.FileFlag = g_dwIVFileFail;
        FileHead.dwIndexNum = 0;
    }

    void UpdateDataIndex(
        const FILETIME& t,
        DWORD DataOffset);

protected:
    void FillHeadToFile(
        const FILETIME& OpenTime )
    {
        FileHead.BeginTime = OpenTime;
        Writer.write((char*)&FileHead, sizeof(IVFileHead));
        UpdatePos();
    }

    //void SaveData(
    //    int nPreAlarmTime,
    //    FileInfo* Info = NULL);

    void SaveDataHeadToFile(
        IVFileDataHead& DataHead, 
        WORD nTargetCount,
        const FILETIME& t);

    void SaveTargetToFile(
        const LiveTargetQueue* m_TargetQueue )
    {
        size_t nSize = m_TargetQueue->nCount*sizeof(WPG_Target);
        Writer.write((char*)&m_TargetQueue->Tar, nSize);
    }

    // Save a Data head + a Group Target Data
    void SaveGroupDataToFile(
        FileInfo& Info,
        FileSectionTime& SectionTime,
        IVFileDataHead& DataHead,
        TGroupTarget* pGroupTarget )
    {
        if ( bIsFirstFrame )
        {
            SectionTime.BeginTime = pGroupTarget->m_time;
            bIsFirstFrame = FALSE;
        }

        if ( !Writer.is_open() )
        {
            Info.FrameBeginTime = pGroupTarget->m_time;
            string strPath = Info.strPath;
            strPath += c_szIVFileExt;
            Writer.open(
                strPath.c_str(),
                ios::binary | ios::trunc | ios::out );
            assert( Writer.is_open() );
            FillHeadToFile(SectionTime.BeginTime);
        }

        Info.FrameEndTime = pGroupTarget->m_time;
        SaveDataHeadToFile(
            DataHead,
            pGroupTarget->m_TargetQueue->nCount,
            pGroupTarget->m_time);
        SaveTargetToFile(pGroupTarget->m_TargetQueue);
    }

    void UpdateDataIndexToFile(const FileInfo& Info);  

protected:
    typedef list<TGroupTarget*> TTargetList;
    typedef TTargetList::iterator TargetListIter;
    typedef list<FileInfo> FileInfoList;

private:

    TTargetList TargetList;      // ���ܵĻ�û����ʾ�����ݶ���
    TTargetList TargetSaveList;  // �Ѿ����Ż��������Ѳ�������ʾ�������ݶ���

    ofstream Writer;
    DWORD dwPrePos;
    DWORD dwCurPos;
    BOOL bIsFirstFrame;

    /**
    *@note ��������������ҪΪ�˱����������ݣ���IV�ļ�ͷ
    *@param FileHead �㷨��Ĭ�Ϸ���һ��IV�ļ�ͷ��ÿ��д�ļ�ͷ(�������ļ��͹ر�),
    *     ��������������������д���ļ�����
    *@param dwCount �������ٸ����ݲű���һ���ļ�������
    *     Ĭ�϶���ΪHead_Index_Interval�����ֵֻ�Ǽ�¼�ж��ٴ���
    *@param MoreDataIndex �Ͼ������п���FileHead(Ĭ��Max_IVData_Index)���������ݻ�д�꣬
    *     дFileHeadд���ˣ��ͽ������Ž�MoreDataIndex��
    *     ���ʱ�����ļ�����ʱ��Ҫ��FileHead��MoreDataIndex������������ƽ��������
    *     �㷨�����ڴ�����UpdateDataIndexToFile��ע��
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
    ChannelTarget();
    ~ChannelTarget();

#ifdef _FixIVFile
    static void Init();
    static void Unit();
#endif // _FixIVFile

    BOOL Open(
        const char* pPath,
        const FILETIME& time);

    BOOL Close(const FILETIME& time);

    BOOL MoveToAndReadSome(const FILETIME& time);

    TGroupTarget* Find(const FILETIME& time);

protected:
    template<IVFileVersionDefine Version>
    BOOL ParseHeadToMap(const IVFileHead& Head);

    size_t GetPos(const FILETIME& time);

    size_t FindBuf();

    BOOL ReadSome(FILETIME& CurrentFrameTime);

    void ClearTempData();
private:
    ifstream m_Reader;
    //BOOL IsFoundFile;
    DWORD m_dwCurrentPos;
    CFileTime m_BeginTime;
    CFileTime m_EndTime;

    // ��һ��λ�÷ŵ�һ֡����Ϣ��
    // �����������һ�����Ϣ
    deque<IVFileDataIndex> m_DataIndex;
    size_t m_CurrentIndexPos;
    
    PlayBackTargetQueue* m_pTargetBuf;
    size_t m_nBufLastPos;

    set<PlayBackGroupTarget> m_IVSomeData;
    CFileTime m_BufStartTime;
    CFileTime m_BufEndTime;

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



