/*H***************************************************************************
 File            : IVLiveViewer.cpp
 Subsystem       : 
 Function Name(s): CIVLiveViewer
 Author          : YiQiu
 Date            : 2010-1-14  
 Time            : 17:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVLiveViewer.h"
#include "IVDataBuf.h"

IIVDataBuf* CIVLiveViewer::s_pIVDataBuf = CIVDataBuf::getInstancePtr();

CIVLiveViewer::CIVLiveViewer(void)
{
}

CIVLiveViewer::~CIVLiveViewer(void)
{
}

inline void TranslateToRect(
    const RECT ClientRect,
    const WPG_RectangleF& WPG_Rect,
    int& left, int& top, int& right, int& bottom)
{
    int nWidth = ClientRect.right-ClientRect.left;
    int nHeight = ClientRect.bottom-ClientRect.bottom;
    left = nWidth*WPG_Rect.x;
    top = nHeight*WPG_Rect.y;
    right = nWidth*(WPG_Rect.x+WPG_Rect.width);
    bottom = nHeight*(WPG_Rect.y+WPG_Rect.height);
}

BOOL CIVLiveViewer::Paint(
    int nChannelID, 
    const HDC dc,
    const RECT& rect,
    const FILETIME& time )
{
    if ( NULL == dc )
    {
        return FALSE;
    }

    TargetQueue* DataQueue = NULL;
    BOOL bRc = s_pIVDataBuf->GetData(nChannelID, time, DataQueue);
    if ( bRc )
    {
        return FALSE;
    }

    //RECT ObjRect;
    int left, top, right, bottom;
    TargetQueue::iterator iter = DataQueue->begin();
    for (; iter!=DataQueue->end();++iter)
    {
        WPG_Target& Tar = *iter;
        TranslateToRect(rect,Tar.boundingBox, left, top, right, bottom);
        ::Rectangle(dc, left, top, right, bottom);
    }


    return TRUE;
}





// End of file