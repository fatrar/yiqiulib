@echo off
if "%1" =="D" goto DEBUG
IF NOT EXIST "..\Package\Release" md "..\Package\Release"
@copy ..\Release\DEV_HC_DM642i.dll "..\Package\Release\DEV_HC_DM642i.dll" /Y
@rem===============================
@exit
:DEBUG
IF NOT EXIST "..\Package\Debug" md "..\Package\Debug"
@copy ..\Debug\DEV_HC_DM642i.dll "..\Package\Debug\DEV_HC_DM642i.dll" /Y
@exit
