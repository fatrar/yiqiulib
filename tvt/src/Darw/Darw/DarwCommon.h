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

inline BOOL UnLockCursor()
{
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN); 
	return ClipCursor(&CRect(0,0,x,y));
}

enum
{
    Point_Radii = 9,
	//Point_Button_Widht = 15,
	//Point_Button_Height = 15,


	MSG_0 = 0x8100,
	MSG_1,
	// ..

};

#define Pow2(x) ((x)*(x)) 

//template<typename T>
inline double Distance(CPoint& p1, CPoint& p2)
{
    return sqrt( Pow2(double(p1.x-p2.x)) + Pow2(double(p1.y-p2.y)) );
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


class CDrawer :
	public IDrawer
	//public IMsgTranslator
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
		size_t nSize = m_PointQueue.size();
		if ( nSize == 0 )
		{
			return false;
		}

#define SimpleDistance(x)  Distance((point), (x))

		TRACE("Input Point is x=%d, y=%d\n", point.x, point.y);
		int nIndex = 0;
		double nMin = SimpleDistance( *(m_PointQueue[nIndex]) );
		TRACE("Point Info 0: x=%d, y=%d\n", m_PointQueue[0]->x, m_PointQueue[0]->y);
		for (size_t i=1; i<nSize; ++i)
		{
			TRACE("Point Info %d: x=%d, y=%d\n", i, m_PointQueue[i]->x, m_PointQueue[i]->y);
			double nTmp = SimpleDistance( *(m_PointQueue[i]) );
            TRACE("for nTmp = %f\n", nTmp);
			if ( nTmp < nMin )
			{
				nMin = nTmp;
				nIndex = i;
			}
		}

		if ( nMin > (double)Point_Radii )
		{
			return false;
		}
		
		m_nDragIndex = nIndex;
        TRACE("%d\n", m_nDragIndex);
		return true;
	}

protected:
	bool m_bIsOK;                // Is Drew?
	bool m_bDrawing;             // Is Drawing?

	int m_nDragIndex;            // Drag Point Index
	bool m_bDragging;            // Is Dragging?

	deque<CPoint*> m_PointQueue; // Point Queue

	CRect m_LockRect;            // lock mouse in Rect, when Draw or drag
};






// End of file



