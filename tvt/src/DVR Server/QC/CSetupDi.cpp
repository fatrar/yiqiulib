#include "stdafx.h"
#include "CSetupDi.h"
#include ".\NTDDK\newdev.h"
#include <afxtempl.h>
//#include "cfgmgr32.h"


/******************************************************************************
* 函数名称： BOOL GetDeviceRegistryHardwareID(SP_DEVINFO_DATA& DeviceInfoData,CString &strHardID)
* 功能描述： 得到指定硬件的系统ID
* 输入参数： DeviceInfoData 硬件信息
* 输出参数： strHardID 硬件的ID
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::GetDeviceRegistryHardwareID(SP_DEVINFO_DATA& DeviceInfoData,
												  CStringArray &strHardIDList)
{
	DWORD DataT;
	DWORD buffersize = 0;

	//先得到该硬件系统ID的长度

	SetupDiGetDeviceRegistryProperty(DeviceInfoSet,&DeviceInfoData,SPDRP_HARDWAREID,&DataT
		,NULL,0,&buffersize);
	
	//分配指定长度的空间接收系统ID
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
* 函数名称： BOOL DeinstallDevices(CStringArray &Devices,void (*pFun)(LPARAM),LPARAM para)
* 功能描述： 卸载指定的所有硬件
* 输入参数： Devices 硬件的系统ID数组
*			 pFun  para 每卸载一个硬件需要使用参数 para 执行函数pFun一次，方便于提示进度
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
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

		//枚举所有设备
		for (int i=0; EnumDeviceInfo(i,DeviceInfoData); i++)
		{
			//得到设备ID
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
* 函数名称： BOOL FindExistingDevice(CString &HardwareId)
* 功能描述： 查找指定硬件是否存在
* 输入参数： HardwareId 硬件的系统ID数组
* 输出参数： 无
* 返 回 值： 成功发现硬件返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::FindExistingDevice(CString &HardwareId)
{
 	if (!IsOpen())  return FALSE;

  	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//枚举所有设备
	for (DWORD i=0; EnumDeviceInfo(i,DeviceInfoData); i++) 
	{
		//得到设备ID
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
* 函数名称： BOOL InstallDrv(CString Deviceid, CString inffilename,HWND hwnd,void (*pFun)(LPARAM),LPARAM para)
* 功能描述： 安装指定ID的所有硬件的Driver
* 输入参数： Deviceid 硬件的系统ID
*			 inffilename 安装Driver所需要的INF文件名
*			 hwnd		窗体句柄
*			 pRebootRequired 安装之后返回是否需要重启电脑
*			 pFun  para 每卸载一个硬件需要使用参数 para 执行函数pFun一次，方便于提示进度
* 输出参数： 无
* 返 回 值： 成功返回TRUE,失败返回FALSE
* 修改日期    版本号     修改人	     修改内容
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

	//枚举所有硬件
	for (int i=0;EnumDeviceInfo(i,DeviceInfoData); i++)
	{
		//得到硬件的ID
		CStringArray idlist;
		if(GetDeviceRegistryHardwareID (DeviceInfoData,idlist))
		{
			//找到含有此ID的硬件
			for(int k=0;k<idlist.GetSize ();k++)
			{
				if(idlist[k].Find(Deviceid,0) > -1)
				{
					//同一个硬件在设备管理器里可以表现不同形式的系统ID,但Driver只需要安装一次
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

/*读注册表的键值by chenlong*/
CString CDeviceInfoList::ReadProductInforFromReg()
{
    //暂且把3308的注册表键值定位"3308",3308s的注册表键值定位"3308S"
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

/*写注册表的键值by chenlong*/
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
* 函数名称： BOOL GetDeviceID(CString HardID,BOOL *bSubSystem ,DVRENUM_PRODUCT_TYPE *CardType)
* 功能描述： 得到指定系统ID的设备数量和ID列表
* 输入参数： HardID		硬件的系统ID
* 输出参数： HardIDNum	硬件数量
*			 IDList		所有ID列表
* 返 回 值： 成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
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
	//枚举所有硬件
	for (int i = 0; EnumDeviceInfo(i, DeviceInfoData); i++)
	{
		//得到硬件的ID
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
* 函数名称： BOOL UpdateDriver(HWND hwnd,CString Deviceid,CString infname,BOOL *pRebootRequired)
* 功能描述： 使用指定INF文件中的信息安装指定系统ID的硬件Driver
* 输入参数： hwnd主窗体句柄
*			 Deviceid设备的系统ID
*			 infname安装用的INF文件
* 输出参数： pRebootRequired,如果安装成功,此参数返回是否需要重启电脑
* 返 回 值： 安装成功返回TRUE,否则返回FALSE
* 修改日期    版本号     修改人	     修改内容
* -----------------------------------------------
******************************************************************************/

BOOL CDeviceInfoList::UpdateDriver(HWND hwnd,CString Deviceid,CString infname,BOOL *pRebootRequired)
{
	//是否存在此硬件
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
