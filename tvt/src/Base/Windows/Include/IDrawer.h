/*H***************************************************************************
 File            : IDrawer.h
 Subsystem       : 
 Function Name(s): CIDrawer
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 11:09
 Description     : 
 
 Revision        : 

 History
 -------

 Note             ：
 为了不让问题复杂化，命名空间只是对外部的接口部分使用。
 对库使用命名空间，目地就是模块化和防止重定义（命名冲突），
 但是会带来很多冗余的代码，使得代码显得很不优雅（很不简洁，简单就是美），
 Boost库就是一个例子，太多命名空间了，看的都晕，
 所以我在这改良为只针对对外接口的定义部分

 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IDRAWER_H_2010_2
#define _IDRAWER_H_2010_2

#include "..\..\Base\Include\Common.h"


#ifdef WINDOWS_EXPORTS
    #define IDRAWER_API API_EXPORT
    #define IDRAWER_CLASS CLASS_EXPORT
#else    
    #define IDRAWER_API API_IMPORT
    #define IDRAWER_CLASS CLASS_IMPORT
    #ifndef WINDOWS_LINK
        #define WINDOWS_LINK
        #pragma comment(lib, "Windows.lib")
        #pragma message("Automatically linking with Windows.dll") 
    #endif
#endif

BEGIN_BASE_ENGINE

namespace Windows
{

enum IDrawerGraphType
{
    IDrawer_Rectangle,
    IDrawer_Polygon,
    IDrawer_Line,
    IDrawer_ArrowLine,
    IDrawer_ArrowLineEx,
};


struct IDrawer
{
    enum
    {
        Polygon_Default_Max_Point = 36,
    };

    virtual ~IDrawer(){};
    virtual size_t GetUserInput(CPoint* pPointBuf, size_t nCount) = 0;
    virtual size_t PointCount() = 0;

    virtual void Clear() = 0;
    virtual void SetDefault(const CPoint* pPoint, size_t nCount) = 0;

    virtual void SetLineColour(DWORD dwColor)=0;
    virtual void SetLineWidth(int dwWidth)=0;

    virtual void Enable(BOOL bEnbale) = 0;
    virtual BOOL IsEnable()=0;
    enum DrawCommond
    {
        // Only ArrowLine & ArrowLineEx
        Line_Show_Left  = 0x1,
        Line_Show_Right = 0x2,
        Line_Show_All   = Line_Show_Left | Line_Show_Right,
        Get_Line_Dir,

        // Only ArrowLineEx
        Line_Left_Add,
        Line_Right_Add,
        Line_Reset_Add,

        // Only Polygon 
        Set_Max_Point   = 0x10,  
    };

    virtual void SendCommond(DrawCommond c, void* p1=NULL, void* p2=NULL) {};
};

struct IDrawModeNotify
{
    virtual void OnChange(IDrawer* p) = 0;
};

struct IDrawContainer : 
    public CWnd
{
public:
    //void SetSelectCursor(HWND Cursor);
    //void SetEditCursor();
    virtual void SetDrawModeNotify(IDrawModeNotify* pDrawMode) = 0;

    virtual IDrawer* Add(IDrawerGraphType t) = 0;
    virtual BOOL Remove(IDrawer* pDrawer)=0;
    virtual IDrawer* GetSelect()=0;
    virtual void SwtichTool(IDrawer* pDrawer)=0;

    virtual void EnableToolExcept(IDrawer* pDrawer, BOOL bEnbale)=0;
    virtual void EnableToolAndNotEnableOther(IDrawer* pDrawer, BOOL bEnbale)=0;
};


IDRAWER_API IDrawContainer* CreateDrawContainer();
IDRAWER_API void DestoryDrawContainer(IDrawContainer* p);

};  // Windows

END_BASE_ENGINE


#endif  // _IDRAWER_H_2010_


 // End of file



