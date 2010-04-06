/**CPP*************************************************************************
 File            : PlayerWnd.cpp
 Subsystem       : 
 Function Name(s): CPlayerWnd
 Author          : YiQiu
 Date            : 2010-3-26  
 Time            : 14:50
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "PlayerWnd.h"


//BOOL YUV2RGB(
//    LPBYTE pRGBBuff, LPBYTE pYUVBuff,
//    long lWidth, long lHeight,
//    int convert)
//{
//    BYTE* pRGB = pRGBBuff;
//    BYTE* pYUV = pYUVBuff;
//    BYTE* tmp = pRGBBuff;
//    BYTE* tmpYUV = pYUVBuff;
//    int j=0;
//    int row=0;
//    int x=pow(2,convert);
//
//    for(int i=0;i<lHeight;i+=x)
//    {
//        if(convert>0) pYUV+=lWidth*2*(2*convert-1);
//        pRGB=tmp+(lWidth/x)*((lHeight/x)-1-row)*3;
//        row+=1;
//        for(j=0;j<lWidth;j+=2*x)
//        {
//            pRGB[0]=
//                max(0,min(255,1.164*(pYUV[0]-16)+1.596*(pYUV[1]-128)));
//            pRGB[1]=
//                max(0,min(255,1.164*(pYUV[0]-16)-0.392*(pYUV[3]-128)-0.813*(pYUV[1]-128)));
//            pRGB[2]=max(0,min(255,1.164*(pYUV[0]-16)+2.017*(pYUV[3]-128)));
//
//            pRGB[3]=max(0,min(255,1.164*(pYUV[2*x]-16)+1.596*(pYUV[1]-128)));
//            pRGB[4]=max(0,min(255,1.164*(pYUV[2*x]-16)-0.392*(pYUV[3]-128)-0.813*(pYUV[1]-128)));
//            pRGB[5]=max(0,min(255,1.164*(pYUV[2*x]-16)+2.017*(pYUV[3]-128)));
//            pRGB+=6;pYUV+=4*x;
//        }
//
//    }
//    return TRUE;
//}

void YUV2RGB(PBYTE pYUV, PBYTE pRGB, DWORD dwWidth, DWORD dwHeight)
{
    int nPixel;
    int nRGBPixel;
    int nTemp;

    //分别计算YUV平面数据中YUV分量的起始地址
    BYTE*	pY = pYUV;
    BYTE*	pU = pYUV + dwWidth*dwHeight;
    BYTE*	pV = pYUV + dwWidth*dwHeight*3/2;

    //YUV平面数据ToRGB
    nPixel = 0;
    for (int i = dwHeight - 1; i >= 0; i--)
    {
        for (int j = 0; j < dwWidth; j++)
        {
            nRGBPixel = j + i * dwWidth;
            nTemp = pY[nPixel] + 1.772 * (pU[nPixel/2] - 128);
            pRGB[3 * nRGBPixel + 0] = nTemp;
            if (nTemp > 255)
                pRGB[3 * nRGBPixel + 0] = 255;
            else 
                if (nTemp < 0)
                    pRGB[3 * nRGBPixel + 0] = 0;

            nTemp = pY[nPixel] - 0.34414 * (pU[nPixel/2] - 128) - 0.71414 * (pV[nPixel/2] - 128);
            pRGB[3 * nRGBPixel + 1] = nTemp;
            if (nTemp > 255)
                pRGB[3 * nRGBPixel + 1] = 255;
            else if (nTemp < 0)
                pRGB[3 * nRGBPixel + 1] = 0;

            nTemp = pY[nPixel] + 1.402 * (pV[nPixel/2] - 128);
            pRGB[3 * nRGBPixel + 2] = nTemp;
            if (nTemp > 255)
                pRGB[3 * nRGBPixel + 2] = 255;
            else if (nTemp < 0)
                pRGB[3 * nRGBPixel + 2] = 0;

            nPixel++;
        }
    }
}

CPlayerWnd::CPlayerWnd(void)
{
}

CPlayerWnd::~CPlayerWnd(void)
{
}

LRESULT CPlayerWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_Player.UnitDirectDraw();

#ifndef YUV_Play
    delete[] m_pRGBBuf;
#endif

    DestroyWindow();
    return 0;
}

LRESULT CPlayerWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT CPlayerWnd::OnEraseBkgnd(HDC hdc)
{
    return FALSE;
    //return TRUE; // background is erased
}

LRESULT CPlayerWnd::OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);
    m_Player.ShowBack();
    return 0;
}

LRESULT CPlayerWnd::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    
    return TRUE; // set focus to default control
}

void CPlayerWnd::Init(
    HWND hwnd, CRect& rect,
    DWORD dwWidth, DWORD dwHeight)
{
    Create(hwnd, rect);
    ShowWindow(SW_SHOW);
    m_Player.InitDirectDraw(
        *this, dwWidth, dwHeight );

#ifndef YUV_Play
    m_pRGBBuf = new BYTE[dwWidth*dwHeight*3];
    //m_Player.SetVideoPlayCallback(this, -1);
#endif
}

BOOL CPlayerWnd::OnVideoPlay(
    HDC dc,
    const tagRECT* rect,    // 针对屏幕坐标
    const FILETIME* pTime,
    HWND hwnd, int nFlag,
    DWORD dwUserData )
{
    static IIVLiveViewer* pIVLiveViewer = IVLiveFactory::GetLiveViewer();
    pIVLiveViewer->Paint(dwUserData, dc, *rect, *pTime);
    pIVLiveViewer->PaintRule(dwUserData, dc, *rect);
    return TRUE;
}

void CPlayerWnd::VideoComing(
    DWORD dwVideoFormat,
    FRAMEBUFSTRUCT *bufs )
{
    if ( !IsWindow() )
    {
        return;
    }
    // 为了支持多分辨显示，及CIF变为D1等，不用static
    ///*static*/ DWORD s_dwWidth = g_pFormat[dwVideoFormat][0];
    ///*static*/ DWORD s_dwHeight = g_pFormat[dwVideoFormat][1];
#ifndef YUV_Play
    YUV2RGB(
        bufs->pBuf,
        m_pRGBBuf,      
        bufs->width,
        bufs->height);

    //static bool s_bSavePic = true;
    //if( s_bSavePic )
    //{
    //    static int j = 0;
    //    static char szBuf[MAX_PATH] = {0};
    //    sprintf_s(szBuf, "C:\\%d.jpg", j++);
    //    ofstream Write(szBuf, ios::binary);
    //    Write.write((char*)m_pRGBBuf, 352*288*3);
    //    Write.close();
    //    s_bSavePic = false;
    //}
#endif
    int nChannel = bufs->ChannelIndex;
    CRect Rect;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);

    FILETIME* pTime = (FILETIME*)&bufs->FrameTime;

#ifdef YUV_Play
    m_Player.Show(
        Rect, bufs->pBuf,
        bufs->width, bufs->height,
        CSingleVideoPlayer::YUV422,
        0, pTime,
        this, bufs->ChannelIndex );
#else
    m_Player.Show(
        Rect,
        m_pRGBBuf,
        bufs->width,
        bufs->height,
        pTime,
        this, bufs->ChannelIndex );
#endif
}


// End of file