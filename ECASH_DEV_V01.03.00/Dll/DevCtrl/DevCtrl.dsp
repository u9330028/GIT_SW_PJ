# Microsoft Developer Studio Project File - Name="DevCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DevCtrl - Win32 T3ATM Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DevCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DevCtrl.mak" CFG="DevCtrl - Win32 T3ATM Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DevCtrl - Win32 T3ATM Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DevCtrl___Win32_T3ATM_Release"
# PROP BASE Intermediate_Dir "DevCtrl___Win32_T3ATM_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Lib/T3ATM_Release"
# PROP Intermediate_Dir "T3ATM Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T3ATM_MODE" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T3ATM_MODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../Bin/T3ATM_Release/DevCtrl.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../Bin/T3ATM_Release/DevCtrl.dll"
# Begin Target

# Name "DevCtrl - Win32 T3ATM Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CFileVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\DevAlertSpack.cpp
# End Source File
# Begin Source File

SOURCE=.\DevApl.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCmn.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCsh.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.def
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\DevDes.CPP
# End Source File
# Begin Source File

SOURCE=.\DevEMV.cpp
# End Source File
# Begin Source File

SOURCE=.\DevFinanceIC.cpp
# End Source File
# Begin Source File

SOURCE=.\DevHiPass.cpp
# End Source File
# Begin Source File

SOURCE=.\DevKCash.cpp
# End Source File
# Begin Source File

SOURCE=.\DevNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DevRDF.cpp
# End Source File
# Begin Source File

SOURCE=.\DevRpcLockCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DevScr.cpp
# End Source File
# Begin Source File

SOURCE=.\DevSnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DevThread.cpp
# End Source File
# Begin Source File

SOURCE=.\DevUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\FILEMISC.CPP
# End Source File
# Begin Source File

SOURCE=..\..\WinAtm\nhalert4spackx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\WinAtm\nhiccm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\WinAtm\nhmwi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\WinAtm\nhmwiemv.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TTSwaveout.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\H\Dll\CFileVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\Class.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassInclude.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassPointer.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassSetOwner.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\CmnLib.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\CmnLibIn.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ConstDef.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\Define.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevCmn.h
# End Source File
# Begin Source File

SOURCE=.\DevDes.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevEtc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevICCard.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevRpcLockCtrlInc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\U8100ErrorTbl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DevCtrl.rc2
# End Source File
# End Group
# Begin Group "ImportLib"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=..\..\Lib\PowerTTS.lib
# End Source File
# End Group
# End Target
# End Project
