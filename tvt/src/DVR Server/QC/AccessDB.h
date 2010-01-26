// AccessDB.h: interface for the CAccessDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSDB_H__97D5F675_3B8E_4581_95F8_1CB47C20FE3D__INCLUDED_)
#define AFX_ACCESSDB_H__97D5F675_3B8E_4581_95F8_1CB47C20FE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAccessDB  
{
public:
	void ModifyResult();
	void SaveMode(long nMode);
	long GetMode();
	void ModifyReset();
	void AddData(CARD_INFOR info);
	void Uninitialize();
	void Initialize();
	CAccessDB();
	virtual ~CAccessDB();

protected:
	_RecordsetPtr m_pRecordSet;
	_RecordsetPtr m_pRecordSetMode;
	_ConnectionPtr m_pConnection;
};

#endif // !defined(AFX_ACCESSDB_H__97D5F675_3B8E_4581_95F8_1CB47C20FE3D__INCLUDED_)
