/********************************************************************
	created:	2009/12/26
	created:	26:12:2009   9:41
	filename: 	f:\code\Darw\Darw\PointButton.h
	file path:	f:\code\Darw\Darw
	file base:	PointButton
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/
#pragma once


struct IMsgTranslator;

// CPointButton
class CPointButton : public CButton
{
	DECLARE_DYNAMIC(CPointButton)

public:
	CPointButton(IMsgTranslator* pMsgTranslator);
	virtual ~CPointButton();

public:


protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	IMsgTranslator* m_pMsgTranslator;
};


