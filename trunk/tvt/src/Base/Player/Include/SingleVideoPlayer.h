/*H***************************************************************************
 File            : SingleVideoPlayer.h
 Subsystem       : 
 Function Name(s): CSingleVideoPlayer
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 18:09
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _SINGLEVIDEOPLAYER_H_2010_2
#define _SINGLEVIDEOPLAYER_H_2010_2



struct IVideoPlayCallBack
{
    virtual BOOL OnVideoPlay(
        HDC dc,
        const tagRECT* rect,    // 针对屏幕坐标
        const FILETIME* pTime,
        HWND hwnd ) = 0;
};


struct IDirectDraw7;
struct IDirectDrawSurface7;
struct IDirectDrawClipper;


class CSingleVideoPlayer
{
public:
    virtual BOOL InitDirectDraw(
        HWND hwnd,  
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL    // 如果为空，使用窗口的区域，这个区域为相对窗口本身
        ) = 0;

    void UnitDirectDraw();

    void SetBackColour(DWORD dwColour){ m_dwBackColour = dwColour; }

    void SetVideoPlayCallback(IVideoPlayCallBack* p){m_pIVideoPlayCallBack=p;}

    void ShowBack();

protected :
    CSingleVideoPlayer();

protected :
    BOOL InitDirectDrawSome(
        HWND hwnd,
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL);

protected:
    BOOL InitYUVBack(DWORD dwWidth, DWORD dwHeight);
    BOOL InitRGBBack(DWORD dwWidth, DWORD dwHeight);

protected:
    HWND m_hWnd;
    RECT m_ShowRect;
    DWORD m_dwBackColour;
    IVideoPlayCallBack* m_pIVideoPlayCallBack;

    IDirectDraw7 *m_pDD7;
    IDirectDrawSurface7 *m_pddsPrimary;
    IDirectDrawClipper* m_lpClipper;     // Pointer to the clipper 
    IDirectDrawSurface7* m_pDDSBack;
};


class CYUVSingleVideoPlayer :
    public CSingleVideoPlayer
{
public:
    virtual BOOL InitDirectDraw(
        HWND hwnd,
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL);

    void Show(
        const tagRECT* rect, 
        const BYTE *pBuf, 
        DWORD format,
        int src420Subtype=0,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL );
};

class CRGBSingleVideoPlayer :
    public CSingleVideoPlayer
{
public:
    virtual BOOL InitDirectDraw(
        HWND hwnd,
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL);

    void Show(
        const tagRECT* rect,
        const BYTE *pBuf,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL);
};


#endif  // _SINGLEVIDEOPLAYER_H_2010_


// End of file


