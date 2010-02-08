/********************************************************************
	created:	2009/12/26
	created:	26:12:2009   9:53
	filename: 	f:\code\Darw\Darw\DarwCommon.h
	file path:	f:\code\Darw\Darw
	file base:	DarwCommon
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/
#pragma once


#include "IDrawer.h"
#include <math.h>
#include <deque>
using namespace std;

//enum MouseMsg
//{
//	MouseMsg_LButtonDown,
//	MouseMsg_LButtonUp,
//	MouseMsg_MouseMove,
//};
//
//struct IMsgTranslator
//{
//	virtual void Translate(MouseMsg msg, CPoint& point, int nID) = 0;
//};





inline BOOL LockCursor(CRect& LockRect){ return ClipCursor(&LockRect); }

#ifdef Support_Dyn_Resolution
#   define OCI_Static
#else
#   define OCI_Static static
#endif

static BOOL UnLockCursor()
{
	OCI_Static int x = GetSystemMetrics(SM_CXSCREEN);
	OCI_Static int y = GetSystemMetrics(SM_CYSCREEN); 
	return ClipCursor(&CRect(0,0,x,y));
}

enum
{
    Point_Radii = 9,
	Point_Color = 0x00ff00,
    Max_Point = 36,

	MSG_0 = 0x8100,
	MSG_1,
	// ..

};

#define Pow2(x) ((x)*(x)) 

#define TracePoint(const CPoint& point)  TRACE("Input Point is x=%d, y=%d\n", point.x, point.y);

//template<typename T>
inline double Distance(const CPoint& p1, const CPoint& p2)
{
    return sqrt( Pow2(double(p1.x-p2.x)) + Pow2(double(p1.y-p2.y)) );
}

int IsDargPoint(CPoint& point, const deque<CPoint>& TestQueue)
{
    size_t nSize = TestQueue.size();
    if ( nSize == 0 )
    {
        return -1;
    }

#define SimpleDistance(x)  Distance((point), (x))

    TracePoint(point);
    int nIndex = 0;
    double nMin = SimpleDistance( TestQueue[nIndex] );
    TracePoint(TestQueue[0]);
    for (size_t i=1; i<nSize; ++i)
    {
        TracePoint( TestQueue[i] );
        double nTmp = SimpleDistance( TestQueue[i] );
        TRACE("for nTmp = %f\n", nTmp);
        if ( nTmp < nMin )
        {
            nMin = nTmp;
            nIndex = i;
        }
    }

    if ( nMin > (double)Point_Radii )
    {
        return -1;
    }

    return nIndex;
}

//inline CRect& CalculateRect(int x, int y, CRect& rect)
//{
//	int x1 = x - Point_Button_Widht/2; 
//	int y1 = y - Point_Button_Height/2; 
//	int x2 = x + Point_Button_Widht/2; 
//	int y2 = y + Point_Button_Height/2; 
//	rect.SetRect(x1, y1, x2, y2);
//	return rect;
//}
//template<size_t MaxPoint = 36>
class IDrawer :
    public CWnd
{
public:
    virtual size_t GetUserInput(CPoint (&szPoint)[Max_Point]) = 0;
};

//template<size_t MaxPoint = 36>
class CDrawer :
	public IDrawer
{
public:
	CDrawer()
		: m_bDrawing(false)
		, m_nDragIndex(-1)
		, m_bDragging(false)
		, m_bIsOK(false) {}

	~CDrawer()
	{
		// [] STLDelete(m_PointQueue);
	}


protected:
	bool IsDargPoint(CPoint& point)
	{
        m_nDragIndex = ::IsDargPoint(point, m_PointQueue);
        return  m_nDragIndex != -1 ;
	}

protected:
	bool m_bIsOK;               // Is Drew?
	bool m_bDrawing;            // Is Drawing?

	int m_nDragIndex;           // Drag Point Index
	bool m_bDragging;           // Is Dragging?

	deque<CPoint> m_PointQueue; // Point Queue
};






// End of file



