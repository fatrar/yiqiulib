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

 Note             ��
 Ϊ�˲������⸴�ӻ��������ռ�ֻ�Ƕ��ⲿ�Ľӿڲ���ʹ�á�
 �Կ�ʹ�������ռ䣬Ŀ�ؾ���ģ�黯�ͷ�ֹ�ض��壨������ͻ����
 ���ǻ�����ܶ�����Ĵ��룬ʹ�ô����Եúܲ����ţ��ܲ���࣬�򵥾���������
 Boost�����һ�����ӣ�̫�������ռ��ˣ����Ķ��Σ�
 �������������Ϊֻ��Զ���ӿڵĶ��岿��

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

namespace System
{
namespace Windows
{

class IDrawer :
    public CWnd
{
public:
    enum
    {
        Polygon_Default_Max_Point = 36,
    };
    
    virtual ~IDrawer(){};
    virtual size_t GetUserInput(CPoint* pPointBuf, size_t nCount) = 0;
    virtual size_t PointCount() = 0;
    virtual void SetColour(DWORD dwColor) = 0;
    virtual void Clear() = 0;
    virtual void SetDefault(const CPoint* pPoint, size_t nCount) = 0;

    enum DrawCommond
    {
        // Only ArrowLine
        Line_Show_Left  = 0x1,
        Line_Show_Right = 0x2,
        Line_Show_All   = Line_Show_Left | Line_Show_Right,

        // Only Polygon 
        Set_Max_Point   = 0x10,  
    };

    virtual void SendCommond(DrawCommond c, void* p1, void* p2) {};
};


enum IDrawerGraphType
{
    IDrawer_Rectangle,
    IDrawer_Polygon,
    IDrawer_Line,
    IDrawer_ArrowLine,
};

namespace DrawerFactory
{
    IDRAWER_API IDrawer* CreateDrawer(IDrawerGraphType t);
    IDRAWER_API void DestoryDrawer(IDrawer* p);
}

};  // Windows
};  // System

END_BASE_ENGINE


#endif  // _IDRAWER_H_2010_


 // End of file


