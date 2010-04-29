

cd %cd%
@echo Copy Base

xcopy ".\Base\Base\Include" ".\Include\Base\Base\Include\" /s /e /Y
xcopy ".\Base\System\Include" ".\Include\Base\System\Include\"  /s /e /Y
xcopy ".\Base\Windows\Include" ".\Include\Base\Windows\Include\"  /s /e /Y

@echo Copy DVR Server
cd "DVR Server"
xcopy ".\SnapShotUI\Include" "..\Include\DVR Server\SnapShotUI\Include\"  /s /e /Y
xcopy ".\DEV_HC_DM642i\Include" "..\Include\DVR Server\DEV_HC_DM642i\Include\" /s /e /Y
xcopy ".\IVCfgMgr\Include" "..\Include\DVR Server\IVCfgMgr\Include\" /s /e /Y
xcopy ".\IVUI\Include" "..\Include\DVR Server\IVUI\Include\" /s /e /Y
xcopy ".\IVViewer\Include" "..\Include\DVR Server\IVViewer\Include\" /s /e /Y
xcopy ".\DEVICECONTROL\IIVDataSender.h" "..\Include\DVR Server\DEVICECONTROL\" /s /e /Y
xcopy ".\DEVICECONTROL\IIVDevice.h" "..\Include\DVR Server\DEVICECONTROL\" /s /e /Y
xcopy ".\DEVICECONTROL\WPG_EventOccurrence.h" "..\Include\DVR Server\DEVICECONTROL\" /s /e /Y
@echo Copy Finish...
pause