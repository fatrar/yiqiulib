// TestRecord.h: interface for the CTestRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTRECORD_H__470F8A54_FA3C_4D19_832E_2C4ED7B261B5__INCLUDED_)
#define AFX_TESTRECORD_H__470F8A54_FA3C_4D19_832E_2C4ED7B261B5__INCLUDED_

#include "Define.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTestRecord  
{
public:
	CString m_path;
	void SetPath(CString str);
	DWORD m_pos;
	void ModifyResetInfor();
	CString MakeHeadItem();
	CString MakeWriterInfor();
	CFile m_fWriter;
	CARD_INFOR m_infor;
	BOOL CheckExist(CString file_name);
	CString MakeFileName();
	CString m_Record_File_Name;
	void Recording();
	void SendWriterInforInitial(CARD_INFOR infor);
//	CARD_INFOR m_card_infor;
	CTestRecord();
	virtual ~CTestRecord();

};

#endif // !defined(AFX_TESTRECORD_H__470F8A54_FA3C_4D19_832E_2C4ED7B261B5__INCLUDED_)
