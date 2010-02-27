#if !defined(AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_)
#define AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextStatic.h : header file
//

#include "..\..\Base\Include\Common.h"

#ifdef WINDOWS_EXPORTS
    #define TEXTSTATIC_API API_EXPORT
    #define TEXTSTATIC_CLASS CLASS_EXPORT
#else    
    #define TEXTSTATIC_API API_IMPORT
    #define TEXTSTATIC_CLASS CLASS_IMPORT
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
/////////////////////////////////////////////////////////////////////////////
// CTextStatic window

class TEXTSTATIC_CLASS CTextStatic : public CStatic
{
    DECLARE_DYNAMIC(CTextStatic)
// Construction
public:
	CTextStatic();
	virtual ~CTextStatic();
    
    void SetFont(int High=14,int Wide=8);
	void SetAlarmColor(COLORREF color);

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTextStatic)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextStatic)
	virtual afx_msg void OnPaint();
	virtual afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	COLORREF m_color;
	COLORREF m_colorTmp;
	CFont	 *m_pCFont;
	DECLARE_MESSAGE_MAP()
};

class TEXTSTATIC_CLASS CAlarmTextStatic : 
    public CTextStatic
{
    DECLARE_DYNAMIC(CAlarmTextStatic)
public:
    CAlarmTextStatic(DWORD dwRefreshRate=700, DWORD dwLastTime=3000);
public:
    void OnAlarmOccur();
    void SetLastTime(DWORD dwLastTime){m_dwLastTime=dwLastTime;}
    void SetRefreshRate(DWORD dwRefreshRate){m_dwRefreshRate=dwRefreshRate;}

protected:
    UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse,
        void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD));

    BOOL KillTimer(UINT_PTR nIDEvent);

    virtual afx_msg void OnPaint();
    virtual afx_msg void OnTimer(UINT nIDEvent);
private:
    DWORD m_dwAlarmRecentTime;
    DWORD m_dwLastTime;
    BOOL m_IsTimerOpen;
    DWORD m_dwRefreshRate;
};


};  // Windows

}; // System

END_BASE_ENGINE

#endif // !defined(AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_)
