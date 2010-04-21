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
//public:
//    typedef LiveGroupTarget TGroupTarget;
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
    *@note �������������Ĺ����ǽ����ݷŲ�ѯ���к͸��ݲ�ѯ���в�ѯ����
    *      ��ѯ���е��߳������ϲ��� 
    */
    TGroupTarget* Find(const FILETIME& time);
    void PushBack(TGroupTarget* pGroupTarget){TargetList.push_back(pGroupTarget);}

    /**
    *@note �������������Ĺ������ϲ㷢�����ļ��򿪻�رգ�����Ϣ�������
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
        const LiveTargetQueue* m_TargetQueue )
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
    typedef list<TGroupTarget*> TTargetList;
    typedef TTargetList::iterator TargetListIter;
    typedef deque<FileInfo> FileInfoList;

private:

    TTargetList TargetList;      // ���ܵĻ�û����ʾ�����ݶ���
    TTargetList TargetSaveList;  // �Ѿ����Ż��������Ѳ�������ʾ�������ݶ���

    ofstream Writer;
    DWORD dwPrePos;
    DWORD dwCurPos;

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
    BOOL PraseHeadToMap(const IVFileHead& Head);

    size_t GetPos(const FILETIME& time);

    size_t FindBuf();

    BOOL ReadSome(FILETIME& CurrentFrameTime);
private:
    ifstream m_Reader;
    //BOOL IsFoundFile;
    DWORD m_dwCurrentPos;
    FILETIME m_BeginTime;
    FILETIME m_EndTime;

    // ��һ��λ�÷ŵ�һ֡����Ϣ��
    // �����������һ�����Ϣ
    deque<IVFileDataIndex> m_DataIndex;
    size_t m_CurrentIndexPos;
    
    PlayBackTargetQueue* m_pTargetBuf;
    size_t m_nBufLastPos;

    set<PlayBackGroupTarget> m_IVSomeData;
    FILETIME m_BufStartTime;
    FILETIME m_BufEndTime;

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



