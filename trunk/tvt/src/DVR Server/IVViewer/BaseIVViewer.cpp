/*CPP*************************************************************************
File            : BaseIVViewer.cpp
Subsystem       : 
Function Name(s): CBaseIVViewer

Description     : 
Author          : Yiqiu
Date            : 2010/01/18
Revision        : 

History
-------


Copyright (c) xx Tech Co.,Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "BaseIVViewer.h"



CBaseIVViewer::CBaseIVViewer(void)
{
    m_pViewerBuf = new ViewerBuf[Max_Device_Num];
}

CBaseIVViewer::~CBaseIVViewer(void)
{
    for ( int i= 0; i<Max_Device_Num; ++i )
    {
        STLDeleteAssociate(m_pViewerBuf[i].PointBuf);
    }
    safeDeleteArray(m_pViewerBuf);
}

inline void TranslateToRect(
    const RECT ClientRect,
    const WPG_RectangleF& WPG_Rect,
    int& left, int& top, int& right, int& bottom)
{
    int nWidth = ClientRect.right-ClientRect.left;
    int nHeight = ClientRect.bottom-ClientRect.top;
    left = nWidth*WPG_Rect.x + ClientRect.left;
    top = nHeight*WPG_Rect.y + ClientRect.top;
    right = nWidth*(WPG_Rect.x+WPG_Rect.width) + ClientRect.left;
    bottom = nHeight*(WPG_Rect.y+WPG_Rect.height) + ClientRect.top;
}

bool IsVaildWPGTarget(const WPG_Target& Data)
{
    const WPG_RectangleF& Rect = Data.boundingBox;
    if ( Rect.height > 1 ||
         Rect.width > 1 ||
         Rect.x > 1 ||
         Rect.y > 1 )
    {
        return false;
    }
    return true;
}

BOOL CBaseIVViewer::Paint(
    int nChannelID,
    const HDC dc, 
    const RECT& rect, 
    const FILETIME& time )
{
    if ( NULL == dc )
    {
        return FALSE;
    }
  
    WPG_Target* pTarBuf = NULL;
    int nTarCount = 0;
    BOOL bNeedFresh = FALSE;
    int nDeviceID = nChannelID/Every_Device_Channel;

    TargetQueue* DataQueue = GetIVData(nChannelID, time);
    if ( DataQueue == NULL )
    {  
        if ( nChannelID != m_pViewerBuf[nDeviceID].ChannelID )
        {
            m_pViewerBuf[nDeviceID].ChannelID = -1;
            return FALSE;
        }
        
        pTarBuf = m_pViewerBuf[nDeviceID].TarBuf;
        nTarCount = m_pViewerBuf[nDeviceID].nTarCount;
    }
    else
    {
        m_pViewerBuf[nDeviceID].ChannelID = nChannelID;
        pTarBuf = DataQueue->Tar;
        nTarCount = DataQueue->nCount;
        bNeedFresh = TRUE;
    }

    HGDIOBJ hObject = ::GetStockObject(NULL_BRUSH);
    HGDIOBJ hOldObj = ::SelectObject(dc, hObject);
    //RECT ObjRect;
    int left, top, right, bottom;
    for (int i =0; i<nTarCount;++i)
    {
        WPG_Target& Tar = pTarBuf[i];
        
        // ���˳�����Ƶ���ڵ�Ŀ�꣬
        // ������SDK��Buf���ݽ����¹��ˣ���Ȼ�����Ŀ�궪ʧ
        if ( !IsVaildWPGTarget(Tar) )
        {
            continue;
        }

        TranslateToRect(rect,Tar.boundingBox, left, top, right, bottom);
        ::Rectangle(dc, left, top, right, bottom);
    }
    ::SelectObject(dc, hOldObj);

    if ( bNeedFresh )
    {
        ViewerBuf& viewBuf = m_pViewerBuf[nDeviceID];
        viewBuf.ChannelID = nChannelID;
        viewBuf.time = time;
        memcpy(viewBuf.TarBuf, DataQueue->Tar, DataQueue->nCount*sizeof(WPG_Target));
        viewBuf.nTarCount = DataQueue->nCount;
        DataQueue->Release();
        RefrehPoint(m_pViewerBuf[nDeviceID].PointBuf, DataQueue, time);
    }

    DrawTrace(dc, m_pViewerBuf[nDeviceID].PointBuf, rect);

    return TRUE;
}

BOOL CBaseIVViewer::ShowObjTrace( int nChannelID, bool bState )
{
    return TRUE;
}

BOOL CBaseIVViewer::GetObjTraceState( int nChannelID, bool& bState )
{
    return TRUE;
}

void CBaseIVViewer::SetDataShowState( int nChannelID, int nState )
{

}

// �����ⲿ�����ڴ�й©��Ҫ������¡�
void CBaseIVViewer::RefrehPoint( 
    ChannelPoint& PointBuf, 
    const TargetQueue* DataQueue,
    const FILETIME& time )
{
    ChannelPoint PointTmpBuf;
    for (int i=0; i<DataQueue->nCount; ++i)
    {
        // ����о�����ԭ�����Ǹ�ָ�룬��������
        // �������ݷ�����ʱ��ChannelPoint��
        const WPG_Target& tar = DataQueue->Tar[i];
        PointList* pPointList = PointBuf[tar.equalId];
        if ( pPointList )
        {
            pPointList->push_back(tar.centroid);
            PointBuf.erase(tar.equalId);
        }
        else
        {
            // û�ҵ���ֱ��newһ���µ��ڴ�
            pPointList = new PointList();
            pPointList->push_back(tar.centroid);
        }

        PointTmpBuf[tar.equalId] = pPointList;
    }

    // �ͷ���ЩĿ�궪ʧ������
    ChannelPoint::iterator iter;
    for ( iter = PointBuf.begin();
          iter!= PointBuf.end();
          ++iter )
    {
        delete iter->second;
    }
    PointBuf.clear();
    PointBuf = PointTmpBuf;
}

void CBaseIVViewer::DrawTrace( 
    const HDC dc,
    const ChannelPoint& PointBuf,
    const RECT& rect  )
{
    POINT p;
    for ( ChannelPoint::const_iterator iter = PointBuf.begin();
          iter != PointBuf.end();
          ++iter )
    {
        PointList* pPointList = iter->second;
        if ( pPointList == NULL )
        {
            // Trace;
            continue;
        }

        if ( pPointList->size() == 0 ||
             pPointList->size() == 1 )
        {
            continue;
        }

        PointList::iterator listIter = pPointList->begin();
        int x = listIter->x * (rect.right-rect.left) + rect.left;
        int y = listIter->y * (rect.bottom-rect.top) + rect.top;
        MoveToEx(dc, x, y, &p);
        for ( ++listIter; listIter != pPointList->end();
              ++listIter )
        {
            x = listIter->x * (rect.right-rect.left) + rect.left;
            y = listIter->y * (rect.bottom-rect.top) + rect.top;
            LineTo(dc, x, y);
        }
    }
}

// End fo file

