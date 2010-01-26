// TestRecord.cpp: implementation of the CTestRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "QCdlg.h"
#include "TestRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestRecord::CTestRecord()
{

}

CTestRecord::~CTestRecord()
{

}

void CTestRecord::SendWriterInforInitial(CARD_INFOR infor)
{
		m_infor=infor;
		m_Record_File_Name=MakeFileName();
		if (CheckExist(m_Record_File_Name))	
		{
			m_fWriter.Open(m_Record_File_Name,CFile::modeReadWrite);
		}
		else
		{
			CString str;
			str=MakeHeadItem();
			m_fWriter.Open(m_Record_File_Name,CFile::modeReadWrite|CFile::modeCreate);
			Sleep(10);
			m_fWriter.Write(str,str.GetLength());
		}
}

void CTestRecord::Recording()
{
	CString str;
	str=MakeWriterInfor();
	m_fWriter.SeekToEnd();
	m_fWriter.Write(str,str.GetLength());
	m_fWriter.Close();
}



CString CTestRecord::MakeFileName()
{
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString file_name;
	file_name.Format("log\\%d%2d%2d.csv",sysTime.wYear,sysTime.wMonth,sysTime.wDay);
	file_name.Replace(" ","0");
	file_name=m_path+file_name;
	return file_name;
}

BOOL CTestRecord::CheckExist(CString file_name)
{
	CFile f;
	BOOL ret;
	ret=f.Open(file_name,CFile::modeRead);
	if (ret)
		f.Close();
	return ret;
}

CString CTestRecord::MakeWriterInfor()
{
	CString str;
	CString sSn;
	CString sCard_type;
	CString sCard_ID;
	CString sSubsys;
	CString sDriver;
	CString sPassword;
	CString sAlarmIn;
	CString sAlarmOut;
	CString sAlarmRelay;
	CString sCallMonitro;
	CString sDisplay;
	CString sReset;
	CString sResult;
	CString sEditTime;
	CString sEditUser;

	SYSTEMTIME sysTime;
//	GetSystemTime(&sysTime);
	GetLocalTime(&sysTime);
//-------------------------------------

	CString stemp;
	if (m_infor.card_serial_number=="")
		sSn="      NULL     ";
	else
		sSn=m_infor.card_serial_number;

	stemp.Format("%6s",m_infor.disname);
	if (_test_table[m_infor.card_type_id][ID_TEST_ID_TYPE_VIEW])
		sCard_type=stemp;
	else
		sCard_type=" �� ";

//	stemp.Format("%36s",m_infor.card_id.Mid(21,m_infor.card_id.GetLength()-21));
	stemp.Format("%14s",m_infor.card_id.Mid(22,m_infor.card_id.GetLength()-22));
	if (_test_table[m_infor.card_type_id][ID_TEST_ID_VIEW])
		sCard_ID=stemp;
	else
		sCard_ID="                ��                ";

	if (_test_table[m_infor.card_type_id][ID_TEST_SUB_ID_BUMBER])
		sSubsys=m_infor.resSubId?"��":"��";
	else
		sSubsys="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_DRIVER_INSTALL])
		sDriver=m_infor.resDriver?"��":"��";
	else
		sDriver="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_PASS_WORD])
		sPassword=m_infor.resPassWord?"��":"��";
	else
		sPassword="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_ALARM_IN])
		sAlarmIn=m_infor.resAlarmIn?"��":"��";
	else
		sAlarmIn="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_ALARM_OUT])
		sAlarmOut=m_infor.resAlarmOut?"��":"��";
	else
		sAlarmOut="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_ALARM_RELAY])
		sAlarmRelay=m_infor.resAlarmRelay?"��":"��";//?" Ok  ":"Error";
	else
		sAlarmRelay="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_CALL_MONITOR])
		sCallMonitro=m_infor.resCallMonitor?"��":"��";
	else
		sCallMonitro="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_SET_SWITCH])
		sDisplay=m_infor.resSetSwitch?"��":"��";
	else
		sDisplay="��";
	if (_test_table[m_infor.card_type_id][ID_TEST_RESET])	
		sReset="��";
	else
		sReset="��";

	sResult=m_infor.resResult?"����":"����";//?"Accept":"Reject" ;
	sEditTime.Format("%d��%2d��%2d��%2dʱ%2d��%2d��",sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	sEditTime.Replace(" ","0");
	char username[50]={"����"};
//	DWORD nLen;
//	GetUserName((LPSTR)username,&nLen);
//	GetUserInfor(username);
//	username="";
	sEditUser=username;
//	stemp.Format("[sSn]:%s,      [Card_type]:%s,        [Card_ID]:%s,          [SubSystemID]:%s,            [Driver]:%s,             [Password]:%s,             [AlarmIn]:%s,           [AlarmOut]:%s,             [AlarmRelay]:%s,             [CallMonitro]:%s,        [Display]:%s,      [Reset]:",sSn,sCard_type,sCard_ID,sSubsys,sDriver,sPassword,sAlarmIn,sAlarmOut,sAlarmRelay,sCallMonitro,sDisplay);
	stemp.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,",sSn,sCard_type,sCard_ID,sSubsys,sDriver,sPassword,sAlarmIn,sAlarmOut,sAlarmRelay,sCallMonitro,sDisplay);
	m_pos=stemp.GetLength();
//	str.Format(  "[sSn]:%s,      [Card_type]:%s,        [Card_ID]:%s,          [SubSystemID]:%s,            [Driver]:%s,             [Password]:%s,             [AlarmIn]:%s,           [AlarmOut]:%s,             [AlarmRelay]:%s,             [CallMonitro]:%s,        [Display]:%s,      [Reset]:%s,    [Result]:%s,   [EditTime]:%s,   [EditUser]:%s\r\n",sSn,sCard_type,sCard_ID,sSubsys,sDriver,sPassword,sAlarmIn,sAlarmOut,sAlarmRelay,sCallMonitro,sDisplay,sReset,sResult,sEditTime,sEditUser);
	str.Format(  "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",sSn,sCard_type,sCard_ID,sSubsys,sDriver,sPassword,sAlarmIn,sAlarmOut,sAlarmRelay,sCallMonitro,sDisplay,sReset,sResult,sEditTime,sEditUser);
	m_pos=m_fWriter.GetLength()+m_pos;
//-------------------------------------
	return str;
}


CString CTestRecord::MakeHeadItem()
{
	CString str;
//	str.Format(" SN_NO  , Card Type  ,                Card ID                  , SubSystemCheck	, DriverCheck , PasswordCheck ,	Check Alarm In , Check Alarm Out , Check Alarm Relay , Call Monitor , Display ,	Reset , Result, Edit Time ,Edit User\n");
	str.Format("��Ʒ���к�,������ ,��ID,��ϵͳID,������װ ,���� ,�������� ,������� ,�̵������� ,Call Monitor ,��ʾ ,�Զ��������� ,�ʼ��� ,��¼ʱ�� ,��¼�û�\n");
	return str;
}

void CTestRecord::ModifyResetInfor()
{
	CString FAL=",��";//",Error";
	m_fWriter.Open(m_Record_File_Name,CFile::modeReadWrite);
	if (m_pos<m_fWriter.GetLength())
	{
		m_fWriter.Seek(m_pos-1,CFile::begin);
		m_fWriter.Write(FAL,FAL.GetLength());
	}
	m_fWriter.Close();

}

void CTestRecord::SetPath(CString str)
{
	m_path=str;
	m_path.Replace("QB","QC");
//	::CreateDirectory(m_path+"log\\",NULL);
}
