@echo off
if "%1" =="D" goto DEBUG
IF NOT EXIST "..\Package\QC" md "..\Package\QC"
IF NOT EXIST "..\Package\QC\protocol files" md "..\Package\QC\protocol files"
@copy .\Release\InstallDrv.exe "..\Package\QC\InstallDrv.exe" /Y
@copy .\Release\QC.exe "..\Package\QC\QC.exe" /Y
@copy .\Release\AlarmCard.dll "..\Package\QC\AlarmCard.dll" /Y
@copy .\Release\AME411Codec.dll "..\Package\QC\AME411Codec.dll" /Y
@copy .\Release\DEV_HC_DM642.dll "..\Package\QC\DEV_HC_DM642.dll" /Y
@copy .\Release\DEV_HC_DM642_16.dll "..\Package\QC\DEV_HC_DM642_16.dll" /Y
@copy .\Release\DEV_HC_DM642_8.dll "..\Package\QC\DEV_HC_DM642_8.dll" /Y
@copy .\Release\TImageLib.dll "..\Package\QC\TImageLib.dll" /Y
@copy .\Release\DEV_SC_AL16.dll "..\Package\QC\DEV_SC_AL16.dll" /Y
@copy .\Release\DEV_SC_CXA.dll "..\Package\QC\DEV_SC_CXA.dll" /Y
@copy .\Release\DEV_SC_COM.dll "..\Package\QC\DEV_SC_COM.dll" /Y
@copy .\Release\Protocol.dll "..\Package\QC\Protocol.dll" /Y
@copy .\Release\Config.ini "..\Package\QC\Config.ini" /Y
@copy .\Release\Result.ini "..\Package\QC\Result.ini" /Y
@copy .\Release\db.mdb "..\Package\QC\db.mdb" /Y
@copy .\Release\DBB.MDB "..\Package\QC\DBB.MDB" /Y
@copy .\Release\QC使用指南.doc "..\Package\QC\QC使用指南.doc" /Y
@copy .\Release\说明.doc "..\Package\QC\说明.doc" /Y
@copy .\Release\log.bak "..\Package\QC\log.bak" /Y
@copy .\Release\DEV_SC_MV04.dll "..\Package\QC\DEV_SC_MV04.dll" /Y
@copy .\Release\DEV_SC_MV08.dll "..\Package\QC\DEV_SC_MV08.dll" /Y
@copy .\Release\DEV_HC_8D.DLL "..\Package\QC\DEV_HC_8D.DLL" /Y
@copy .\Release\DEV_HC_8C.DLL "..\Package\QC\DEV_HC_8C.DLL" /Y
@copy .\Release\protocolfiles\Lilin.dll "..\Package\QC\protocol files\Lilin.dll" /Y
@copy .\Release\protocolfiles\Minking.dll "..\Package\QC\protocol files\Minking.dll" /Y
@copy .\Release\protocolfiles\Neon.dll "..\Package\QC\protocol files\Neon.dll" /Y
@copy .\Release\protocolfiles\PelcoD.dll "..\Package\QC\protocol files\PelcoD.dll" /Y
@copy .\Release\protocolfiles\PelcoP.dll "..\Package\QC\protocol files\PelcoP.dll" /Y
@copy .\Release\protocolfiles\Star.dll "..\Package\QC\protocol files\Star.dll" /Y
@copy .\Release\protocolfiles\Vido.dll "..\Package\QC\protocol files\Vido.dll" /Y
@copy .\Release\protocolfiles\VISCA.dll "..\Package\QC\protocol files\VISCA.dll" /Y
@rem===============================
@exit
:DEBUG
@exit
