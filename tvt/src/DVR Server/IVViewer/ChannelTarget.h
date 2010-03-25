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

    /**
    *@note �������������Ĺ����ǽ����ݷŲ�ѯ���к͸��ݲ�ѯ���в�ѯ����
    *      ��ѯ���е��߳������ϲ��� 
    */
    GroupTarget* Find(const FILETIME& time);
    void PushBack(GroupTarget* pGroupTarget){TargetList.push_back(pGroupTarget);}

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
    enum
    {
        /**
        *@note 5�뱣��һ����������������(�ļ�����0.5Hour)����360��������
        *  ��ô��200������£���160������
        */
        Head_Index_Interval = 10*5,
    };

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
    typedef deque<FileInfo> FileInfoList;

public:

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






/**
*@note  
*@param	  
*@param   
*@param  
*@param  
*@return 
*/


#endif  // _CHANNELTARGET_H_2010_


 // End of file



