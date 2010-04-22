/**CPP*************************************************************************
 File            : IVPlaybackViewer.cpp
 Subsystem       : 
 Function Name(s): CIVPlaybackViewer
 Author          : YiQiu
 Date            : 2010-4-14  
 Time            : 15:39
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "IVPlaybackViewer.h"

IIVDataBuf* CIVPlaybackViewer::s_pIVDataBuf = CIVPlaybackDataBuf::getInstancePtr();


CIVPlaybackViewer::CIVPlaybackViewer(void)
    : m_PlayBackMode(Dir__)
    , m_NeedClearBuf(FALSE)
{
}

CIVPlaybackViewer::~CIVPlaybackViewer(void)
{
}

BaseTargetQueue* CIVPlaybackViewer::GetIVData(
    int nChannelID,
    const FILETIME& time )
{
    return s_pIVDataBuf->GetData(nChannelID,time);
}

void CIVPlaybackViewer::RefrehPoint( 
    ChannelPoint& PointBuf, 
    const BaseTargetQueue* DataQueue )
{
    if ( m_NeedClearBuf )
    {
        PointBuf.clear();
        m_NeedClearBuf = FALSE;
    }

    if ( m_PlayBackMode == Dir__ )
    {
        return CBaseIVViewer<IIVPlayBackViewer>::RefrehPoint(
            PointBuf,
            DataQueue);
    }

    ChannelPoint PointTmpBuf;
    ChannelPoint::iterator iter;
    for (int i=0; i<DataQueue->nCount; ++i)
    {
        // ����о�����ԭ�����Ǹ�ָ�룬��������
        // �������ݷ�����ʱ��ChannelPoint��
        const WPG_Target& tar = DataQueue->Tar[i];
        iter = PointBuf.find(tar.equalId);
        PointList* pPointList  = NULL;
        if ( iter != PointBuf.end() )
        {
            pPointList = iter->second;
            pPointList->push_back(tar.centroid);
            PointBuf.erase(iter);
        }
        else
        {
            // û�ҵ���ֱ��newһ���µ��ڴ�
            pPointList = new PointList();

            ++g_PointList;
            pPointList->push_back(tar.centroid);
        }

        assert(PointTmpBuf[tar.equalId]==NULL);
        PointTmpBuf[tar.equalId] = pPointList;
    }

    // �ͷ���ЩĿ�궪ʧ������
    for ( iter = PointBuf.begin();
        iter!= PointBuf.end();
        ++iter )
    {
        --g_PointList;
        delete iter->second;
    }
    PointBuf.clear();
    PointBuf = PointTmpBuf;
}

void CIVPlaybackViewer::SetPlayBackMode( PlayBack_Mode Mode )
{
    if ( m_PlayBackMode == Mode )
    {
        return;
    }
    m_PlayBackMode = Mode;
    m_NeedClearBuf = TRUE;
}


// End of file