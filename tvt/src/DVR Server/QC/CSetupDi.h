// Helper class with SetupDi.. fns
#ifndef _CSETUPDI_HPP
#define _CSETUPDI_HPP
#include ".\NTDDK\setupapi.h" // for SetupDiXxx functions.

class CDeviceInfoList {
protected:
public:
	BOOL GetDeviceID(CString HardID,BOOL *bSubSystem ,DVRENUM_PRODUCT_TYPE *CardType);
	BOOL InstallDrv(CString Deviceid, CString inffilename,HWND hwnd, BOOL *pRebootRequired, void (*pFun)(LPARAM),LPARAM para);
	BOOL DeinstallDevices(CStringArray &Devices,void (*pFun)(LPARAM),LPARAM para);
    CString ReadProductInforFromReg();//by chenlong
    void WriteProductInforToReg(CString str);//by chenlong

	// Create a Device Information Set with all present devices.
	CDeviceInfoList() : DeviceInfoSet(INVALID_HANDLE_VALUE)
	{
		DeviceInfoSet = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT ); // All devices present on system
	}
	~CDeviceInfoList() { if (IsOpen()) SetupDiDestroyDeviceInfoList(DeviceInfoSet);}

	// To know if the Query is open
	bool IsOpen() const 
	{ 
		return (DeviceInfoSet!=INVALID_HANDLE_VALUE); 
	}

	// To enum the Device Info
	bool EnumDeviceInfo(ULONG i,SP_DEVINFO_DATA& DeviceInfoData)
	{
		return SetupDiEnumDeviceInfo(DeviceInfoSet, i, &DeviceInfoData) == TRUE;
	}
	bool EnumDriverInfo(SP_DEVINFO_DATA &DeviceInfoData,DWORD  DriverType,ULONG i,SP_DRVINFO_DATA& DriverInfoData)
	{
		return SetupDiEnumDriverInfo(DeviceInfoSet,&DeviceInfoData,DriverType,i,&DriverInfoData)==TRUE;
	}
	bool SetSelectedDriver(SP_DEVINFO_DATA& DeviceInfoData,SP_DRVINFO_DATA& DriverInfoData)
	{
		return SetupDiSetSelectedDriver(DeviceInfoSet,&DeviceInfoData,&DriverInfoData)==TRUE;
	}
	bool SetClassInstallParams(SP_DEVINFO_DATA& DeviceInfoData,SP_CLASSINSTALL_HEADER& classInstHeader,DWORD bsize)
	{
		return SetupDiSetClassInstallParams(DeviceInfoSet,&DeviceInfoData,&classInstHeader,bsize)==TRUE;
	}
	bool InstallDevice(SP_DEVINFO_DATA& DeviceInfoData)
	{
		return SetupDiInstallDevice(DeviceInfoSet,&DeviceInfoData)==TRUE;
	}
	bool InstallDeviceInterfaces(SP_DEVINFO_DATA& DeviceInfoData)
	{
		return SetupDiInstallDeviceInterfaces(DeviceInfoSet,&DeviceInfoData)==TRUE;
	}
	bool DestroyDriverInfoList(SP_DEVINFO_DATA& DeviceInfoData,DWORD drivertype)
	{
		return SetupDiDestroyDriverInfoList(DeviceInfoSet,&DeviceInfoData,drivertype)==TRUE;
	}

	// To call the class installer/uninstaller
	bool CallClassInstaller(UINT code,SP_DEVINFO_DATA& DeviceInfoData)
	{
		return SetupDiCallClassInstaller(code, DeviceInfoSet,&DeviceInfoData)==TRUE;
	}

private:
	HDEVINFO DeviceInfoSet;	// Handle to the Set
	// To release a Query
private:
	// To Get the Device registry Hardware IDs.
	BOOL GetDeviceRegistryHardwareID(SP_DEVINFO_DATA& DeviceInfoData,CStringArray &strHardIDList);
	BOOL FindExistingDevice(CString &HardwareId);
	BOOL UpdateDriver(HWND hwnd,CString Deviceid,CString infname,BOOL *pRebootRequired);
	BOOL GetPCIDeviceHardwareID(CString &HwEntry, WORD Vendor, WORD ID[], DWORD IDCount);
};

#endif