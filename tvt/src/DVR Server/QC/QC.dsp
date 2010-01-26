# Microsoft Developer Studio Project File - Name="QC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=QC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QC.mak" CFG="QC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "QC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ClWork/QC", PRFBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QC - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 .\NTDDK\newdev.lib SetupAPI.lib ddraw.lib dxguid.lib winmm.lib AMD411Codec.lib vfw32.lib Protocol.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=CopyFiles.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "QC - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\NTDDK\newdev.lib SetupAPI.lib ddraw.lib dxguid.lib winmm.lib AMD411Codec.lib vfw32.lib Protocol.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "QC - Win32 Release"
# Name "QC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlarmTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodecMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetupDi.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\MyWaveOut.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PtzTest.cpp
# End Source File
# Begin Source File

SOURCE=.\QC.cpp
# End Source File
# Begin Source File

SOURCE=.\QC.rc
# End Source File
# Begin Source File

SOURCE=.\QCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SignalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestCard.cpp
# End Source File
# Begin Source File

SOURCE=.\TextStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3004.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3008.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3016.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3108.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3116.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3216.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3304.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3308.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlarmDefine.h
# End Source File
# Begin Source File

SOURCE=.\AlarmTest.h
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\CodecMgr.h
# End Source File
# Begin Source File

SOURCE=.\CSetupDi.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\DEVICEControl\DeviceControl.h
# End Source File
# Begin Source File

SOURCE=.\DeviceDriver.h
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\ModifyUser.h
# End Source File
# Begin Source File

SOURCE=.\MyWaveOut.h
# End Source File
# Begin Source File

SOURCE=.\NewUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\PtzTest.h
# End Source File
# Begin Source File

SOURCE=.\QC.h
# End Source File
# Begin Source File

SOURCE=.\QCDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SignalDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestCard.h
# End Source File
# Begin Source File

SOURCE=.\TextStatic.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriver.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3004.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3008.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3016.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3108.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3116.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3216.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3304.h
# End Source File
# Begin Source File

SOURCE=.\VideoDriverTD3308.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\QC.ico
# End Source File
# Begin Source File

SOURCE=.\res\QC.rc2
# End Source File
# Begin Source File

SOURCE=.\res\VIEWMODE1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VIEWMODE16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VIEWMODE4.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
