// AccessDB.cpp: implementation of the CAccessDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "qc.h"
#include "AccessDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccessDB::CAccessDB()
{
	Initialize();
}

CAccessDB::~CAccessDB()
{
	Uninitialize();
}

void CAccessDB::Initialize()
{
//	TRACE("--------------------------------------CoInitialize\n");
//	ASSERT(S_OK == ::CoInitialize(NULL));
	CreateDirectory(".\\log\\", NULL);
	CString strSource;
	strSource = CTime::GetCurrentTime().Format(".\\log\\log %Y-%m-%d.mdb");
	CopyFile(".\\log.bak", strSource, TRUE);
	CString strConn = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
	strConn += strSource;
	strConn += ";Jet OLEDB:Database Password=access";
	bstr_t bstrConn = strConn.AllocSysString();
	try
	{
		m_pConnection.CreateInstance(__uuidof(Connection));
		m_pConnection->Open(bstrConn, "", "", -1);
		m_pRecordSet.CreateInstance(__uuidof(Recordset));
		m_pRecordSet->Open("Select * from log", m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		m_pRecordSetMode.CreateInstance(__uuidof(Recordset));
		m_pRecordSetMode->Open("Select * from info", m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR Initialize : %s\n", e.ErrorMessage());
	}
}

void CAccessDB::Uninitialize()
{
//	TRACE("--------------------------------------CoUninitialize\n");
	try
	{
		m_pRecordSet->Close();
		m_pConnection->Close();
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR Uninitialize : %s\n", e.ErrorMessage());
	}

//	::CoUninitialize();
}

//增加一条新的测试纪录
void CAccessDB::AddData(CARD_INFOR info)
{
	CString strTemp;
	try
	{
		m_pRecordSet->AddNew();
		if (info.card_serial_number == "")
			strTemp = "NULL";
		else
			strTemp = info.card_serial_number;
		m_pRecordSet->PutCollect("Card_Serial_Number", (LPCSTR)strTemp);

		strTemp.Format("%6s", info.disname);						//djx 报警板类型写入数据库不再写无
		if (!_test_table[info.card_type_id][ID_TEST_ID_TYPE_VIEW]  && info.card_type_id!=DVR_CARD_ALARM_CARD)
			strTemp = "无";
		m_pRecordSet->PutCollect("Type", (LPCSTR)strTemp);
		
		if (info.card_id != "")
		{
			if (info.card_id == IDUSB_1 || info.card_id == IDUSB_2)
				strTemp = info.card_id;
			else
				strTemp.Format("%14s", info.card_id.Mid(22, info.card_id.GetLength() - 22));
		}
		else
			strTemp = "NULL";
		if (!_test_table[info.card_type_id][ID_TEST_ID_VIEW])
			strTemp = "无";
		m_pRecordSet->PutCollect("Card_ID", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_SUB_ID_BUMBER])
			strTemp = info.resSubId ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("SubSYS_ID", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_DRIVER_INSTALL])
			strTemp = info.resDriver ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Driver", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_PASS_WORD])
			strTemp = info.resPassWord ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Password", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_ALARM_IN])
			strTemp = info.resAlarmIn ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Alarm_In", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_ALARM_OUT])
			strTemp = info.resAlarmOut ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Alarm_Out", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_ALARM_RELAY])
			strTemp = info.resAlarmRelay ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Alarm_Relay", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_CALL_MONITOR])
			strTemp = info.resCallMonitor ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Call_Monitor", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_SET_SWITCH])
			strTemp = info.resSetSwitch ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Set_Switch", (LPCSTR)strTemp);

		if (_test_table[info.card_type_id][ID_TEST_RESET])	
			strTemp = "好";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Reset", (LPCSTR)strTemp);

		//djx 08/3/3
		if (_test_table[info.card_type_id][ID_TEST_AUDIO])	
			strTemp = info.resAudio ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Audio_Test", (LPCSTR)strTemp);

		//djx
		if (_test_table[info.card_type_id][ID_TEST_ALARM_PTZ])	
			strTemp = info.resAlarmPTZ ? "好" : "坏";
		else
			strTemp = "无";
		m_pRecordSet->PutCollect("Alarm_ptz", (LPCSTR)strTemp);

		strTemp = info.resResult ? "接受" : "拒收";
		m_pRecordSet->PutCollect("Result", (LPCSTR)strTemp);

		strTemp = CTime::GetCurrentTime().Format("%Y年%m月%d日%H时%M分%S秒");
		m_pRecordSet->PutCollect("Record_Time", (LPCSTR)strTemp);

		strTemp = info.RecordUser;
		m_pRecordSet->PutCollect("User", (LPCSTR)strTemp);

		m_pRecordSet->Update();
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR AddData : %s\n", e.ErrorMessage());
	}
}

//修改Reset项
void CAccessDB::ModifyReset()
{
	CString strTemp;
	try
	{
		strTemp = "坏";
		m_pRecordSet->Update("Reset", (LPCSTR)strTemp);
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR ModifyReset : %s\n", e.ErrorMessage());
	}
}

long CAccessDB::GetMode()
{
	_variant_t varTemp;
	long nMode;
	try
	{
		varTemp = m_pRecordSetMode->GetCollect("PALNTSC");
		nMode = long(varTemp);
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR SaveMode : %s\n", e.ErrorMessage());
	}
	return nMode;
}

void CAccessDB::SaveMode(long nMode)
{
	try
	{
		m_pRecordSetMode->Update("PALNTSC", nMode);
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR SaveMode : %s\n", e.ErrorMessage());
	}
}

//修改Result项
void CAccessDB::ModifyResult()
{
	CString strTemp;
	try
	{
		strTemp = "拒收";
		m_pRecordSet->Update("Result", (LPCSTR)strTemp);
	}
	catch(_com_error e)
	{
		TRACE("ADO ERROR ModifyReset : %s\n", e.ErrorMessage());
	}
}
