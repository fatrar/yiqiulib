/********************************************************************
	created:	2009/12/24
	created:	24:12:2009   14:32
	filename: 	F:\code\Darw\Darw\IDrawer.h
	file path:	F:\code\Darw\Darw
	file base:	IDrawer
	file ext:	h
	author:		heliang
	
	purpose:	
*********************************************************************/


#ifndef IDRAWER_H__
#define IDRAWER_H__

#include "stdafx.h"
//#include <afxwin.h>
//#include <GdiplusTypes.h>


// ��׼ö�ٶ���Ϊȫ��д���»��ߣ����ִ������й��˿���д��ϰ�ߵ����⣬
// �����Ҹ���Ϊÿ�������ִ�д���»������Ӹ��ʣ�
// ��windowsƽ̨�������������������ͻ������
enum DrawerCommand
{
	Line_Arrow_Change,
};

enum DrawerErr
{
	No_Error,
	Not_Found_Commad,
	Param_Input_Err,
};

struct IDrawer
{
    // Mouse event and Graph Render
	virtual void OnMouseDown(CDC* pdc, CPoint& point, CRect& pLockRect) = 0;
	virtual void OnMouseMove(CDC* pdc, CPoint& point) = 0;
	virtual void OnMouseUp(CDC* pdc, CPoint& point) = 0;
	virtual void OnPaint(CDC* pdc) = 0;
	virtual DrawerErr OnCommand(DrawerCommand command, WPARAM wParam = 0, LPARAM lParam = 0) = 0; 
};



//#define A(x) T_##x
//#define LoopDefine(0) MSG_0 
//#define LoopDefine(x) LoopDefine(x-1)+1


#endif // IDRAWER_H__