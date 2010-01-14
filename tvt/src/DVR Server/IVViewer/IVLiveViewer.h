/*H***************************************************************************
 File            : IVLiveViewer.h
 Subsystem       : 
 Function Name(s): CIVLiveViewer
 Author          : YiQiu
 Date            : 2010-1-14  
 Time            : 17:49
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVLIVEVIEWER_H_2010_
#define _IVLIVEVIEWER_H_2010_




class CIVLiveViewer :
    public IIVViewer
{
public:
    CIVLiveViewer(void);
    virtual ~CIVLiveViewer(void);

    // IIVViewer
public:
    // ��ʾĿ����ο��·��ʱ�Ļص���
    // ����DC�����ڲ��ŵ�����֡��ʱ��
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time);

public:
    static IIVDataBuf* s_pIVDataBuf;

private:
    typedef deque<WPG_PointF> PointList;
    typedef map<size_t, PointList> ChannelPoint;
    typedef map<int, ChannelPoint> AllChannelPoint;
    AllChannelPoint m_PointRecord;
};





#endif  // End of file




