; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPTView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PT.h"
LastPage=0

ClassCount=10
Class1=CPTApp
Class2=CPTDoc
Class3=CPTView
Class4=CMainFrame

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_HISCORES (Dutch (Belgium))
Resource4=IDD_ABOUTBOX (English (U.S.))
Class6=CPTBegin
Class7=CPTOptions
Resource5=IDR_MAINFRAME (English (U.S.))
Class8=CPTHiScores
Class9=CPTMainBegin
Class10=CPTStartDlg
Resource6=IDD_OPTIONS
Resource7=IDD_OPTIONS (Dutch (Belgium))
Resource8=IDD_HISCORES

[CLS:CPTApp]
Type=0
HeaderFile=PT.h
ImplementationFile=PT.cpp
Filter=N
LastObject=CPTApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CPTDoc]
Type=0
HeaderFile=PTDoc.h
ImplementationFile=PTDoc.cpp
Filter=N
LastObject=CPTDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CPTView]
Type=0
HeaderFile=PTView.h
ImplementationFile=PTView.cpp
Filter=C
LastObject=CPTView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=PT.cpp
ImplementationFile=PT.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_APP_ABOUT
CommandCount=11
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_GAME_HISCORES
Command3=ID_GAME_OPTIONS
Command4=ID_APP_EXIT
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342312448

[CLS:CPTBegin]
Type=0
HeaderFile=PTBegin.h
ImplementationFile=PTBegin.cpp
BaseClass=CDialog
Filter=D
LastObject=CPTBegin

[DLG:IDD_OPTIONS]
Type=1
Class=CPTOptions
ControlCount=11
Control1=IDC_Points,edit,1216413824
Control2=IDC_Lifes,edit,1216413824
Control3=IDC_Level,edit,1216413824
Control4=IDC_STATIC,static,1342177294
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_Name,edit,1350631552
Control8=IDC_DIFF,edit,1350639746
Control9=IDC_DIFFSPIN,msctls_updown32,1342177334
Control10=IDC_SOUND,button,1342242819
Control11=IDC_STATIC,static,1342177294

[CLS:CPTOptions]
Type=0
HeaderFile=PTOptions.h
ImplementationFile=PTOptions.cpp
BaseClass=CDialog
Filter=D
LastObject=CPTOptions
VirtualFilter=dWC

[DLG:IDD_HISCORES]
Type=1
Class=CPTHiScores
ControlCount=27
Control1=IDC_ACCURACY,static,1342308352
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SHOTBULLETS,static,1342308352
Control10=IDC_TROOPS,static,1342308352
Control11=IDC_TROOPC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_HELIC,static,1342308352
Control17=IDC_HELIS,static,1342308352
Control18=IDC_CHUTES,static,1342308352
Control19=IDC_BOMBC,static,1342308352
Control20=IDC_BOMBS,static,1342308352
Control21=IDC_PLANEC,static,1342308352
Control22=IDC_PLANES,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_TROOPL,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_SKILL,static,1342308352
Control27=IDC_STATIC,static,1342308352

[CLS:CPTHiScores]
Type=0
HeaderFile=PTHiScores.h
ImplementationFile=PTHiScores.cpp
BaseClass=CDialog
Filter=D
LastObject=CPTHiScores
VirtualFilter=dWC

[CLS:CPTMainBegin]
Type=0
HeaderFile=PTMainBegin.h
ImplementationFile=PTMainBegin.cpp
BaseClass=CDialog
Filter=D
LastObject=CPTMainBegin

[CLS:CPTStartDlg]
Type=0
HeaderFile=PTStartDlg.h
ImplementationFile=PTStartDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPTStartDlg

[DLG:IDD_OPTIONS (Dutch (Belgium))]
Type=1
Class=CPTOptions
ControlCount=10
Control1=IDC_Points,edit,1216413824
Control2=IDC_Lifes,edit,1216413824
Control3=IDC_Level,edit,1216413824
Control4=IDC_STATIC,static,1342177294
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_Name,edit,1350631552
Control8=IDC_DIFF,edit,1350639746
Control9=IDC_DIFFSPIN,msctls_updown32,1342177334
Control10=IDC_SOUND,button,1208025091

[DLG:IDD_HISCORES (Dutch (Belgium))]
Type=1
Class=CPTHiScores
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

