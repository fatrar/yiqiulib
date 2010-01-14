; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIVTree
LastTemplate=CTreeCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IVUI_Test.h"

ClassCount=7
Class1=CIVUI_TestApp
Class2=CIVUI_TestDlg

ResourceCount=3
Resource2=IDD_IVUI_TEST_DIALOG
Resource1=IDR_MAINFRAME
Class5=CIVAlarmOutWnd
Class3=CIVRuleWnd
Class4=CIVSchuduleWnd
Class6=CIVSwtichTab
Class7=CIVTree
Resource3=IDD_IVUI_TEST_DIALOG (English (U.S.))

[CLS:CIVUI_TestApp]
Type=0
HeaderFile=IVUI_Test.h
ImplementationFile=IVUI_Test.cpp
Filter=N

[CLS:CIVUI_TestDlg]
Type=0
HeaderFile=IVUI_TestDlg.h
ImplementationFile=IVUI_TestDlg.cpp
Filter=D



[DLG:IDD_IVUI_TEST_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CIVUI_TestDlg

[DLG:IDD_IVUI_TEST_DIALOG (English (U.S.))]
Type=1
Class=CIVUI_TestDlg
ControlCount=2
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BUTTON1,button,1342242816

[CLS:CIVRuleWnd]
Type=0
HeaderFile=IVRuleWnd.h
ImplementationFile=IVRuleWnd.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CIVSchuduleWnd]
Type=0
HeaderFile=IVSchuduleWnd.h
ImplementationFile=IVSchuduleWnd.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CIVSchuduleWnd

[CLS:CIVAlarmOutWnd]
Type=0
HeaderFile=IVAlarmOutWnd.h
ImplementationFile=IVAlarmOutWnd.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CIVAlarmOutWnd

[CLS:CIVSwtichTab]
Type=0
HeaderFile=IVSwtichTab.h
ImplementationFile=IVSwtichTab.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CIVSwtichTab

[CLS:CIVTree]
Type=0
HeaderFile=IVTree.h
ImplementationFile=IVTree.cpp
BaseClass=CTreeCtrl
Filter=W

