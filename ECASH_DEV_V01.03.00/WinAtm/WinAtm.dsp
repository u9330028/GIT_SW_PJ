# Microsoft Developer Studio Project File - Name="WinAtm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WinAtm - Win32 T3ATM Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinAtm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinAtm.mak" CFG="WinAtm - Win32 T3ATM Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinAtm - Win32 T3ATM Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinAtm___Win32_T3ATM_Release"
# PROP BASE Intermediate_Dir "WinAtm___Win32_T3ATM_Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib/T3ATM_Release"
# PROP Intermediate_Dir "T3ATM Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T3ATM_MODE" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Gz /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T3ATM_MODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/T3ATM_Release/WinAtm.ocx"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/T3ATM_Release/WinAtm.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\../Lib/T3ATM_Release
TargetPath=\01.ATM_SRC\18.EASY_src\ECASH_SRC\01.WINATM_SRC\DEV_V01.00.00\Bin\T3ATM_Release\WinAtm.ocx
InputPath=\01.ATM_SRC\18.EASY_src\ECASH_SRC\01.WINATM_SRC\DEV_V01.00.00\Bin\T3ATM_Release\WinAtm.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build
# Begin Target

# Name "WinAtm - Win32 T3ATM Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AtmClerkCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AtmHostCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AtmModeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AtmTranCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\handleocr.cpp
# End Source File
# Begin Source File

SOURCE=.\Helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\nhalert4spackx.cpp
# End Source File
# Begin Source File

SOURCE=.\nhguros.cpp
# End Source File
# Begin Source File

SOURCE=.\nhiccm.cpp
# End Source File
# Begin Source File

SOURCE=.\nhmwi.cpp
# End Source File
# Begin Source File

SOURCE=.\nhmwiemv.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinAtm.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAtm.def
# End Source File
# Begin Source File

SOURCE=.\WinAtm.odl
# End Source File
# Begin Source File

SOURCE=.\WinAtm.rc
# End Source File
# Begin Source File

SOURCE=.\WinAtmCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAtmPpg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\H\Common\Class.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\ClassInclude.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\ClassPointer.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\ClassSetOwner.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\CmnLib.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\CmnLibIn.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\ConstDef.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\Define.h
# End Source File
# Begin Source File

SOURCE=.\handleocr.h
# End Source File
# Begin Source File

SOURCE=.\Helpers.h
# End Source File
# Begin Source File

SOURCE=.\nhalert4spackx.h
# End Source File
# Begin Source File

SOURCE=.\nhguros.h
# End Source File
# Begin Source File

SOURCE=.\nhmwi.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\H\Common\ScrDefine.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WinAtm.h
# End Source File
# Begin Source File

SOURCE=.\WinAtmCtl.h
# End Source File
# Begin Source File

SOURCE=.\WinAtmPpg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\WinAtm.ico
# End Source File
# Begin Source File

SOURCE=.\WinAtmCtl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\HistoryList.txt
# End Source File
# End Target
# End Project
# Section WinAtm : {984454BC-621A-41BE-AC67-636C00801AFC}
# 	2:21:DefaultSinkHeaderFile:nhmwi.h
# 	2:16:DefaultSinkClass:CNHMWI
# End Section
# Section WinAtm : {9EDFDAFE-3023-4F3F-BAD8-DCA1DD3717D9}
# 	2:5:Class:CNHMWI
# 	2:10:HeaderFile:nhmwi.h
# 	2:8:ImplFile:nhmwi.cpp
# End Section
