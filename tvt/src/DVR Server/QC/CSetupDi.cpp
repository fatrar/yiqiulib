#include "stdafx.h"
#include "CSetupDi.h"
#include ".\NTDDK\newdev.h"
#include <afxtempl.h>
//#include "cfgmgr32.h"


/******************************************************************************
* �������ƣ� BOOL GetDeviceRegistryHardwareID(SP_DEVINFO_DATA& DeviceInfoData,CString &strHardID)
* ���������� �õ�ָ��Ӳ����ϵͳID
* ��������� DeviceInfoData Ӳ����Ϣ
* ��������� strHardID Ӳ����ID
* �� �� ֵ�� �ɹ�����TRUE,ʧ�ܷ���FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::GetDeviceRegistryHardwareID(SP_DEVINFO_DATA& DeviceInfoData,
												  CStringArray &strHardIDList)
{
	DWORD DataT;
	DWORD buffersize = 0;

	//�ȵõ���Ӳ��ϵͳID�ĳ���

	SetupDiGetDeviceRegistryProperty(DeviceInfoSet,&DeviceInfoData,SPDRP_HARDWAREID,&DataT
		,NULL,0,&buffersize);
	
	//����ָ�����ȵĿռ����ϵͳID
	char *pBuf=new char[buffersize];
	if(!SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID,
		&DataT, (BYTE *)pBuf, buffersize, &buffersize))
	{
		delete []pBuf;
		return FALSE;
	}
	else
	{
		char *p = pBuf;
		while(p[0] != 0)
		{
			CString str = p;
			strHardIDList.Add(str);
			int i = strlen((char *)p);
			p += strlen((char *)p) + sizeof(TCHAR);
		}
		delete []pBuf;
        pBuf = NULL;
		return TRUE;
	}
}


/******************************************************************************
* �������ƣ� BOOL DeinstallDevices(CStringArray &Devices,void (*pFun)(LPARAM),LPARAM para)
* ���������� ж��ָ��������Ӳ��
* ��������� Devices Ӳ����ϵͳID����
*			 pFun  para ÿж��һ��Ӳ����Ҫʹ�ò��� para ִ�к���pFunһ�Σ���������ʾ����
* ��������� ��
* �� �� ֵ�� �ɹ�����TRUE,ʧ�ܷ���FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::DeinstallDevices(CStringArray &Devices,void (*pFun)(LPARAM),
									   LPARAM para)
{
	if (!IsOpen())  return FALSE;

	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for(int j=0;j<Devices.GetSize ();j++)
	{	

		//ö�������豸
		for (int i=0; EnumDeviceInfo(i,DeviceInfoData); i++)
		{
			//�õ��豸ID
			CStringArray idlist;
			if(GetDeviceRegistryHardwareID (DeviceInfoData,idlist))
			{
				for(int k=0;k<idlist.GetSize ();k++)
				{
					if (idlist[k].Find (Devices[j],0) > -1)
						CallClassInstaller(DIF_REMOVE,DeviceInfoData);
				}
			}
		}
		if(pFun)
			pFun(para);
	}
	return TRUE;
}


/******************************************************************************
* �������ƣ� BOOL FindExistingDevice(CString &HardwareId)
* ���������� ����ָ��Ӳ���Ƿ����
* ��������� HardwareId Ӳ����ϵͳID����
* ��������� ��
* �� �� ֵ�� �ɹ�����Ӳ������TRUE,���򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::FindExistingDevice(CString &HardwareId)
{
 	if (!IsOpen())  return FALSE;

  	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//ö�������豸
	for (DWORD i=0; EnumDeviceInfo(i,DeviceInfoData); i++) 
	{
		//�õ��豸ID
		CStringArray idlist;
		if(GetDeviceRegistryHardwareID(DeviceInfoData,idlist))
		{
			for(int k=0;k<idlist.GetSize ();k++)
			{
				if (idlist[k].Find (HardwareId,0) > -1)
					return TRUE;
			}
		}
    }
    return FALSE;
}


/******************************************************************************
* �������ƣ� BOOL InstallDrv(CString Deviceid, CString inffilename,HWND hwnd,void (*pFun)(LPARAM),LPARAM para)
* ���������� ��װָ��ID������Ӳ����Driver
* ��������� Deviceid Ӳ����ϵͳID
*			 inffilename ��װDriver����Ҫ��INF�ļ���
*			 hwnd		������
*			 pRebootRequired ��װ֮�󷵻��Ƿ���Ҫ��������
*			 pFun  para ÿж��һ��Ӳ����Ҫʹ�ò��� para ִ�к���pFunһ�Σ���������ʾ����
* ��������� ��
* �� �� ֵ�� �ɹ�����TRUE,ʧ�ܷ���FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::InstallDrv(CString Deviceid, CString inffilename,HWND hwnd, BOOL *pRebootRequired, void (*pFun)(LPARAM),LPARAM para)
{
	if (!IsOpen()) return FALSE;
	CFileFind fFind;
	if (!fFind.FindFile (inffilename)) return FALSE;

	CList<DWORD,DWORD> InsList;
	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//ö������Ӳ��
	for (int i=0;EnumDeviceInfo(i,DeviceInfoData); i++)
	{
		//�õ�Ӳ����ID
		CStringArray idlist;
		if(GetDeviceRegistryHardwareID (DeviceInfoData,idlist))
		{
			//�ҵ����д�ID��Ӳ��
			for(int k=0;k<idlist.GetSize ();k++)
			{
				if(idlist[k].Find(Deviceid,0) > -1)
				{
					//ͬһ��Ӳ�����豸����������Ա��ֲ�ͬ��ʽ��ϵͳID,��Driverֻ��Ҫ��װһ��
					if(InsList.Find (DeviceInfoData.DevInst)==NULL)
					{
						InsList.AddTail (DeviceInfoData.DevInst);
						BOOL Reboot=FALSE;
						BOOL ret=UpdateDriver(hwnd,idlist[k],inffilename,&Reboot);
						if(!ret) return 0;
						if(pFun)
							pFun(para);
						if(Reboot) *pRebootRequired=TRUE;
					}
				}
			}
		}
	}
	return TRUE;	
}

/*��ע���ļ�ֵby chenlong*/
CString CDeviceInfoList::ReadProductInforFromReg()
{
    //���Ұ�3308��ע����ֵ��λ"3308",3308s��ע����ֵ��λ"3308S"
    HKEY hKey;
    char sz[1024];
    DWORD len=1024;
    if(ERROR_SUCCESS==RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\SuperDVR\\SuperDVR",&hKey))
    {
        CRegKey rg;
        rg.Attach(hKey);
        rg.QueryValue (sz,"ProductCode",&len);
        rg.Close();
        return sz;
    }
    return "-1";  //heliang fix
}

/*дע���ļ�ֵby chenlong*/
void CDeviceInfoList::WriteProductInforToReg(CString str)
{
    HKEY hKey;
    if(ERROR_SUCCESS==RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\SuperDVR\\SuperDVR",&hKey))
    {
        CRegKey rg;
        rg.Attach(hKey);
        LONG ret=rg.SetValue (str,"ProductCode");
        rg.Close();
    }
}
/******************************************************************************
* �������ƣ� BOOL GetDeviceID(CString HardID,BOOL *bSubSystem ,DVRENUM_PRODUCT_TYPE *CardType)
* ���������� �õ�ָ��ϵͳID���豸������ID�б�
* ��������� HardID		Ӳ����ϵͳID
* ��������� HardIDNum	Ӳ������
*			 IDList		����ID�б�
* �� �� ֵ�� �ɹ�����TRUE,���򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::GetDeviceID(CString HardID,BOOL *bSubSystem ,
								  DVRENUM_PRODUCT_TYPE *CardType)
{
	if (!IsOpen())  return FALSE;

//	CList<DWORD, DWORD> devInstList;
	SP_DEVINFO_DATA DeviceInfoData;
	BOOL finded = FALSE;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	//ö������Ӳ��
	for (int i = 0; EnumDeviceInfo(i, DeviceInfoData); i++)
	{
		//�õ�Ӳ����ID
		CStringArray idlist;
		if(GetDeviceRegistryHardwareID(DeviceInfoData, idlist))
		{
			for(int k = 0; k < idlist.GetSize(); k++)
			{
				if(idlist[k].Find(HardID, 0) > -1)
				{
					*CardType = DVR_CARD_TDNULL;

					if(TW6800_VIDEO == HardID)//3004,3008,3016
					{
						if(idlist[k].Find(ID3004, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3004;
						}
						else if(idlist[k].Find(ID3008, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3008;
						}
						else if(idlist[k].Find(ID3016, 0)>-1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3016;
						}
						if (!finded)
						{
							*bSubSystem = FALSE;
							return FALSE;
						}
					}
					if(TW6805_VIDEO == HardID)//djx 2008/1/5 3004_6805
					{
						if (idlist[k].Find(ID3004V2, 0)>-1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3004V2;
						}
						else
						{
							*bSubSystem = FALSE;
							return FALSE;
						}
					}
					if(TW6805_3016== HardID)//zld 2009/6/22 3016_6805/3008_6805
					{
						if (idlist[k].Find(ID3016V2, 0)>-1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3016V2;
						}
						else if (idlist[k].Find(ID3008V2, 0)>-1) 
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3008V2;
						}
						else
						{
							*bSubSystem = FALSE;
							return FALSE;
						}
					}
					if(BT878_VIDEO == HardID)//3116(1.0/2.0)3216 (2.0/3.0)
					{
						if(idlist[k].Find(ID3116V1, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3116V1;
						}
						else if(idlist[k].Find(ID3116V2, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3116V2;
						}
						else if(idlist[k].Find(ID3216V2, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3216V2;
						}
						else if(idlist[k].Find(ID3216V3, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3216V3;
						}
						else if (idlist[k].Find(ID3108, 0) > -1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3108;
						}

						if (!finded)
						{
							*bSubSystem = FALSE;
							return FALSE;
						}
					}
/////////////////////ADD 3316 /////////////
					if( ALOG480 == HardID)//3116(1.0/2.0)3216 (2.0/3.0)
					{
						if(idlist[k].Find(ID3316,0)>-1)
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3316;
						}
						if (!finded)
						{
							*bSubSystem = FALSE;
							return FALSE;
						}
					}
/////////////////////ADD 3316 /////////////

					if(IDUSB_1 == HardID || IDUSB_2 == HardID)//USB
					{
						if(IDUSB_1 == HardID)
							*CardType = DVR_CARD_TDUSB_1;
						else
							*CardType = DVR_CARD_TDUSB_2;
						finded = TRUE;
					}

					if(ID4104 == HardID)
					{
						*CardType = DVR_CARD_TD4104;
						finded = TRUE;
					}

					if(ID4104i == HardID)
					{
						*CardType = DVR_CARD_TD4104i;
						finded = TRUE;
					}

					if(ID4116 == HardID)//ADD4116
					{
						*CardType = DVR_CARD_TD4116;
						finded = TRUE;
					}

					//djx 2008/7/22 4108
					if(ID4108 == HardID)
					{
						*CardType = DVR_CARD_TD4108;
						finded = TRUE;
					}

					//djx 2008/7/22 4108
					//chenlong 09/09/08
					if(ID3308 == HardID || ID3304 == HardID || ID3304S == HardID || ID3308S == HardID)
					{
						if(idlist[k].Find(ID3308, 0) > -1)//zld2009/4/22 3308
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3308;
						}
						else if(idlist[k].Find(ID3304, 0) > -1)//zld2009/4/22 3304
						{
							finded = TRUE;
							*CardType = DVR_CARD_TD3304;
						}
                        else if (ID3304S == HardID)
                        {
                            if (idlist[k].Find(ID3304S, 0) > -1)
                            {
                                finded = TRUE;
                                *CardType = DVR_CARD_TD3304S;
                                CString str = ReadProductInforFromReg();
                                if (str.CompareNoCase("3304S") != 0)
                                {
                                    CString strtemp("3304S");
                                    WriteProductInforToReg(strtemp);
                                }
                            }
                        }                       
                        else if (ID3308S == HardID)
                        {
                            if (idlist[k].Find(ID3308S, 0) > -1)
                            {
                                finded = TRUE;
                                *CardType = DVR_CARD_TD3308S;
                                CString str = ReadProductInforFromReg();
                                if (str.CompareNoCase("3308S") != 0)
                                {
                                    CString strtemp("3308S");
                                    WriteProductInforToReg(strtemp);
                                }
                            }
                        }
                    }

					//zld 2009/4/24 4408
					if(ID4408 == HardID)
					{
						*CardType = DVR_CARD_TD4408;
						finded = TRUE;
					}
					
				}
				if (finded)
				{
					*bSubSystem = TRUE;
					return TRUE;
				}
			}
			
		}
	}
	return FALSE;
//	HardIDNum=devInstList.GetCount();
}


/******************************************************************************
* �������ƣ� BOOL UpdateDriver(HWND hwnd,CString Deviceid,CString infname,BOOL *pRebootRequired)
* ���������� ʹ��ָ��INF�ļ��е���Ϣ��װָ��ϵͳID��Ӳ��Driver
* ��������� hwnd��������
*			 Deviceid�豸��ϵͳID
*			 infname��װ�õ�INF�ļ�
* ��������� pRebootRequired,�����װ�ɹ�,�˲��������Ƿ���Ҫ��������
* �� �� ֵ�� ��װ�ɹ�����TRUE,���򷵻�FALSE
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::UpdateDriver(HWND hwnd,CString Deviceid,CString infname,BOOL *pRebootRequired)
{
	//�Ƿ���ڴ�Ӳ��
	if (FindExistingDevice(Deviceid)) 
	{
        if (!UpdateDriverForPlugAndPlayDevices(hwnd, Deviceid, infname, INSTALLFLAG_FORCE, pRebootRequired))
        {
            return 0; // Install Failure
        }
		else
		{
			return 1;
		}
    }
	else
	{
		return 0;
	}
}
