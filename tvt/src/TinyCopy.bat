
@echo Copy Base
F:
cd F:\tvt\src
xcopy ".\Base\Base\Include" ".\Include\Base\Base\Include\" /s /e /Y
xcopy ".\Base\System\Include" ".\Include\Base\System\Include\"  /s /e /Y
xcopy ".\Base\Windows\Include" ".\Include\Base\Windows\Include\"  /s /e /Y

@echo Copy DVR Server
cd F:\tvt\src
xcopy ".\DVR Server\SnapShotUI\Include" ".\Include\DVR Server\SnapShotUI\Include\"  /s /e /Y
xcopy ".\DVR Server\DEV_HC_DM642i\Include" ".\Include\DVR Server\DEV_HC_DM642i\Include\" /s /e /Y
xcopy ".\DVR Server\IVCfgMgr\Include" ".\Include\DVR Server\IVCfgMgr\Include\" /s /e /Y
xcopy ".\DVR Server\IVUI\Include" ".\Include\DVR Server\IVUI\Include\" /s /e /Y
xcopy ".\DVR Server\IVViewer\Include" ".\Include\DVR Server\IVViewer\Include\" /s /e /Y

@echo Copy Finish...
pause