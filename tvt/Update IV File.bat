@echo **************************************
@echo            Author heliang
@echo **************************************
@echo Start Copy...

cd %cd%
@echo Copy Include
xcopy "\\yiqiu\include\DVR Server\DEV_HC_DM642i" .\DEV_HC_DM642i\ /s /e /Y
xcopy "\\yiqiu\include\DVR Server\DEVICECONTROL" .\DEVICECONTROL\ /s /e /Y
xcopy "\\yiqiu\include\DVR Server\IVCfgMgr" .\IVCfgMgr\    /s /e /Y
xcopy "\\yiqiu\include\DVR Server\IVUI"     .\IVUI\        /s /e /Y
xcopy "\\yiqiu\include\DVR Server\IVViewer" .\IVViewer\ /s /e /Y
xcopy "\\yiqiu\include\DVR Server\SnapShotUI" .\SnapShotUI\ /s /e /Y


@echo Copy Lib
xcopy \\yiqiu\lib\IVUI.lib .\lib\ /s /e /Y
xcopy \\yiqiu\lib\IVViewer.lib .\lib\ /s /e /Y
xcopy \\yiqiu\lib\SnapShotUI.lib .\lib\ /s /e /Y
xcopy \\yiqiu\lib\IVCfgMgr.lib .\lib\ /s /e /Y


@echo Copy Binary File

xcopy \\yiqiu\bin\release\Base.dll    .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\System.dll  .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\Windows.dll .\IVRlease\ /s /e /Y

xcopy \\yiqiu\bin\release\DEV_HC_DM642i.dll .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\IVCfgMgr.dll      .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\IVUI.dll          .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\IVViewer.dll      .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\Player.dll        .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\SnapShotUI.dll    .\IVRlease\ /s /e /Y

xcopy \\yiqiu\bin\release\IVTest_UI.exe .\IVRlease\ /s /e /Y
xcopy \\yiqiu\bin\release\AlarmCard.dll .\IVRlease\ /s /e /Y

@echo Copy Finish...
pause