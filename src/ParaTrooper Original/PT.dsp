# Microsoft Developer Studio Project File - Name="PT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PT.mak" CFG="PT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PT - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PT - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PT - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x813 /d "NDEBUG"
# ADD RSC /l 0x813 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "PT - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x813 /d "_DEBUG"
# ADD RSC /l 0x813 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "PT - Win32 Release"
# Name "PT - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PT.cpp
# End Source File
# Begin Source File

SOURCE=.\PT.rc
# End Source File
# Begin Source File

SOURCE=.\PTDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PTGameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PTGameInfoStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\PTGameObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PTGameStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\PTHiScores.cpp
# End Source File
# Begin Source File

SOURCE=.\PTInGameObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PTOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\PTView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PT.h
# End Source File
# Begin Source File

SOURCE=.\PTDoc.h
# End Source File
# Begin Source File

SOURCE=.\PTGameInfo.h
# End Source File
# Begin Source File

SOURCE=.\PTGameInfoStatic.h
# End Source File
# Begin Source File

SOURCE=.\PTGameObject.h
# End Source File
# Begin Source File

SOURCE=.\PTGameStatistics.h
# End Source File
# Begin Source File

SOURCE=.\PTHiScores.h
# End Source File
# Begin Source File

SOURCE=.\PTInGameObject.h
# End Source File
# Begin Source File

SOURCE=.\PTOptions.h
# End Source File
# Begin Source File

SOURCE=.\PTView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\background1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bkgr2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bkgr3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bomber2g.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\chute.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cloudone.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\comicfont.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\explosio.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\gun90degr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gunbody.bmp
# End Source File
# Begin Source File

SOURCE=.\res\heli1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\helichinook.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\heligroen.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\helipaars.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\helitole.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\helitori.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\level2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\life.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\noknop.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\noknopfoc.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\okknop.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\okknopfoc.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\options.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\optionscover.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\optionsskin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\planeexp.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\planetol.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\planetor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PT.ico
# End Source File
# Begin Source File

SOURCE=.\res\PT.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PTDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\splat.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\stats.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\titel.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\troopchut.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\trooper.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
