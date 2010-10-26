# Microsoft Developer Studio Project File - Name="libResPacker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libResPacker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libResPacker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libResPacker.mak" CFG="libResPacker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libResPacker - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libResPacker - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libResPacker - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libResPacker___Win32_Release"
# PROP BASE Intermediate_Dir "libResPacker___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\inc\\" /I "..\Common\inc\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_USE_LZMA_COMPRESS_" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Release\libResPacker.lib"

!ELSEIF  "$(CFG)" == "libResPacker - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libResPacker___Win32_Debug"
# PROP BASE Intermediate_Dir "libResPacker___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\inc\\" /I "..\Common\inc\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_USE_LZMA_COMPRESS_" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Debug\libResPacker.lib"

!ENDIF 

# Begin Target

# Name "libResPacker - Win32 Release"
# Name "libResPacker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ResPacker.cpp
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\IResPacker.h
# End Source File
# Begin Source File

SOURCE=.\src\ResPacker.h
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.h
# End Source File
# End Group
# Begin Group "lzma"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\lzma\LzFind.c
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzFind.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaEnc.c
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaEnc.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaLib.c
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaLib.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\LzmaUtil.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzma\Types.h
# End Source File
# End Group
# End Target
# End Project
