# Microsoft Developer Studio Project File - Name="TranCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TranCtrl - Win32 T3ATM Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TranCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TranCtrl.mak" CFG="TranCtrl - Win32 T3ATM Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TranCtrl - Win32 T3ATM Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "TranCtrl___Win32_T3ATM_Release"
# PROP BASE Intermediate_Dir "TranCtrl___Win32_T3ATM_Release"
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
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../Bin/T3ATM_Release/TranCtrl.dll"
# ADD LINK32 ..\..\Lib\PsApi.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../Bin/T3ATM_Release/TranCtrl.dll"
# Begin Target

# Name "TranCtrl - Win32 T3ATM Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Builder Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\..\H\Builder\TranBeginProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranCommonProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranDepProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranEndProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranInqProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranNoMediaProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranPbProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTicketProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTranProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTransProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranWithProc.cpp
# End Source File
# End Group
# Begin Group "SubProc Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TranCmnLibSubProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCommonSubProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranNoMediaSubProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranTicketSubProc.cpp
# End Source File
# End Group
# Begin Group "ECashDataFieldProc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TranEcashDataFieldProc.Cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\cpu_info.cpp
# End Source File
# Begin Source File

SOURCE=.\EzCaDes.cpp
# End Source File
# Begin Source File

SOURCE=.\INIEncFile.cpp
# End Source File
# Begin Source File

SOURCE=.\perfmon.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\SeedDWS.cpp
# End Source File
# Begin Source File

SOURCE=.\Seedx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TranCmn.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.def
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\TranDevProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranEnCryptProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranHostProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranInputProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranKCashProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranLibProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranMainProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranMenuProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranNmsProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranOutputProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranRecvProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranSendProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranTermProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranTxRxCommProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranTxRxHeaderProc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Builder Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\H\Builder\TranCtrlProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranCtrlProc_STA.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc_STA.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranRetCode.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTranProc.h
# End Source File
# End Group
# Begin Group "SubProc Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\H\Tran\TranMainSubProc.h
# End Source File
# End Group
# Begin Group "NiceDataFieldProc Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\H\Tran\TranECashDataFieldProc.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\H\Common\CmnLib.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ConstDef.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\Define.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\MacroDef.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ScrDefine.h
# End Source File
# Begin Source File

SOURCE=.\Seedx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmn.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnEtc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnHost.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnUser.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranDevProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranHostProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranInputProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranKCashProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranMainProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranMenuProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranNmsProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranOutputProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranTermProc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TranCtrl.rc2
# End Source File
# End Group
# Begin Group "ImPortLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Lib\WooriSeed.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\xmclient.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\Encrypt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\Hana.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\HanaSeed.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\lseed.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\nh_isatm.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\PSBPEM.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\sc_wrap_isatm.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\KB_XMCLNT.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\tnATMCSTK.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\crypt_cds.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\fnHDEncLib.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\WREncClt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\DBEncClt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\DSEncClt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\DYEncClt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\HWEncClt.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\xc220c.lib
# End Source File
# End Group
# End Target
# End Project
