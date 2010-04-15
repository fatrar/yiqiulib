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

#include "..\..\Base\Include\Common.h"


#ifdef PLAYER_EXPORTS
    #define SINGLEVIDEOPLAYER_API   API_EXPORT
    #define SINGLEVIDEOPLAYER_CLASS CLASS_EXPORT
#else    
    #define SINGLEVIDEOPLAYER_API API_IMPORT
    #define SINGLEVIDEOPLAYER_CLASS CLASS_IMPORT
    #ifndef PLAYER_LINK
        #define PLAYER_LINK
        #pragma comment(lib, "Player.lib")
        #pragma message("Automatically linking with Player.dll") 
    #endif
#endif


struct IDirectDraw7;
struct IDirectDrawSurface7;
struct IDirectDrawClipper;


BEGIN_BASE_ENGINE


struct IVideoPlayCallBack
{
    virtual BOOL OnVideoPlay(
        HDC dc,
        const tagRECT* rect,    // 针对屏幕坐标
        const FILETIME* pTime,
        HWND hwnd,
        int nFlag,
        DWORD dwUserData ) = 0;   // 0为临时的， 1为永久的那个
};



class SINGLEVIDEOPLAYER_CLASS CSingleVideoPlayer
{
public:
    virtual BOOL InitDirectDraw(
        HWND hwnd,  
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL    // 如果为空，使用窗口的区域，这个区域为相对窗口本身
        ) = 0;

    void UnitDirectDraw();

    void SetBackColour(DWORD dwColour){ m_dwBackColour = dwColour; }

    void SetVideoPlayCallback(
        IVideoPlayCallBack* p,
        DWORD dwUserData )
    {
        m_dwUserData = dwUserData;
        m_pIVideoPlayCallBack=p;
    }

    void ShowBack();

    inline void OnVideoPlay(
        const tagRECT* rect, 
        const FILETIME* pTime,
        IVideoPlayCallBack* p,
        DWORD dwUserData);

public:
    enum YUVFormat
    {
        YUV420	   = 0,
        YUV422	   = 1,
        YUV422PACK = 2,
    };

protected :
    CSingleVideoPlayer();

protected :
    BOOL InitDirectDrawSome(
        HWND hwnd,
        DWORD dwWidth, DWORD dwHeight,
        const RECT* ShowRect = NULL);

protected:
    BOOL InitYUVBack(DWORD dwWidth, DWORD dwHeight);
    BOOL InitRGBBack(DWORD dwWidth, DWORD dwHeight, DWORD BitDepth=24);

    BOOL ReInitYUVBack(DWORD dwWidth, DWORD dwHeight);
    BOOL ReInitRGBBack(DWORD dwWidth, DWORD dwHeight);
protected:
    HWND m_hWnd;
    RECT m_ShowRect;
    DWORD m_dwBackColour;
    IVideoPlayCallBack* m_pIVideoPlayCallBack;

    IDirectDraw7 *m_pDD7;
    IDirectDrawSurface7 *m_pddsPrimary;
    IDirectDrawClipper* m_lpClipper;     // Pointer to the clipper 
    IDirectDrawSurface7* m_pDDSBack;

    DWORD m_dwWidth, m_dwHeight;

    DWORD m_dwUserData;
};


class SINGLEVIDEOPLAYER_CLASS CYUVSingleVideoPlayer :
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
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0);
};

class SINGLEVIDEOPLAYER_CLASS CYUVSingleVideoPlayerEx :
    public CYUVSingleVideoPlayer
{
public:
    void Show(
        const tagRECT* rect, 
        const BYTE *pBuf, 
        DWORD dwWidth, DWORD dwHeight,
        DWORD format,
        int src420Subtype=0,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0 );

protected:
    void Show(
        const tagRECT* rect, 
        const BYTE *pBuf, 
        DWORD format,
        int src420Subtype=0,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0 ) {};
};

class SINGLEVIDEOPLAYER_CLASS CRGBSingleVideoPlayer :
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
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0);
};

class SINGLEVIDEOPLAYER_CLASS CRGBSingleVideoPlayerEx :
    public CRGBSingleVideoPlayer
{
public:
    void Show(
        const tagRECT* rect,
        const BYTE *pBuf,
        DWORD dwWidth, DWORD dwHeight,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0);

protected:
    void Show(
        const tagRECT* rect,
        const BYTE *pBuf,
        const FILETIME* pTime = NULL,
        IVideoPlayCallBack* p = NULL,
        DWORD dwUserData = 0){};
};


END_BASE_ENGINE

#endif  // _SINGLEVIDEOPLAYER_H_2010_


// End of file


