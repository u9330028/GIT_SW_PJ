# Microsoft Developer Studio Generated NMAKE File, Based on TranCtrl.dsp
!IF "$(CFG)" == ""
CFG=TranCtrl - Win32 T2 Debug
!MESSAGE No configuration specified. Defaulting to TranCtrl - Win32 T2 Debug.
!ENDIF 

!IF "$(CFG)" != "TranCtrl - Win32 Release" && "$(CFG)" != "TranCtrl - Win32 Debug" && "$(CFG)" != "TranCtrl - Win32 T2 Release" && "$(CFG)" != "TranCtrl - Win32 T2 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TranCtrl.mak" CFG="TranCtrl - Win32 T2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TranCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TranCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TranCtrl - Win32 T2 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TranCtrl - Win32 T2 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TranCtrl - Win32 Release"

OUTDIR=.\../../Lib/T1ATM_Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Release\TranCtrl.dll"

!ELSE 

ALL : "NetCtrl - Win32 Release" "ProCtrl - Win32 Release" "DevCtrl - Win32 Release" "..\..\Bin\T1ATM_Release\TranCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 ReleaseCLEAN" "ProCtrl - Win32 ReleaseCLEAN" "NetCtrl - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TranAjmsProc.obj"
	-@erase "$(INTDIR)\TranBeginProc.obj"
	-@erase "$(INTDIR)\TranClerkProc.obj"
	-@erase "$(INTDIR)\TranCmn.obj"
	-@erase "$(INTDIR)\TranCtrl.obj"
	-@erase "$(INTDIR)\TranCtrl.pch"
	-@erase "$(INTDIR)\TranCtrl.res"
	-@erase "$(INTDIR)\TranDepProc.obj"
	-@erase "$(INTDIR)\TranDevProc.obj"
	-@erase "$(INTDIR)\TranEndProc.obj"
	-@erase "$(INTDIR)\TranFuncProc.obj"
	-@erase "$(INTDIR)\TranHostProc.obj"
	-@erase "$(INTDIR)\TranICProc.obj"
	-@erase "$(INTDIR)\TranInputProc.obj"
	-@erase "$(INTDIR)\TranInqProc.obj"
	-@erase "$(INTDIR)\TranKCashProc.obj"
	-@erase "$(INTDIR)\TranMainProc.obj"
	-@erase "$(INTDIR)\TranMenuProc.obj"
	-@erase "$(INTDIR)\TranNmsProc.obj"
	-@erase "$(INTDIR)\TranOutputProc.obj"
	-@erase "$(INTDIR)\TranPbProc.obj"
	-@erase "$(INTDIR)\TranRFProc.obj"
	-@erase "$(INTDIR)\TranTermProc.obj"
	-@erase "$(INTDIR)\TranTranProc.obj"
	-@erase "$(INTDIR)\TranTransProc.obj"
	-@erase "$(INTDIR)\TranWithProc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TranCtrl.exp"
	-@erase "$(OUTDIR)\TranCtrl.lib"
	-@erase "..\..\Bin\T1ATM_Release\TranCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\TranCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TranCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\TranCtrl.pdb" /machine:I386 /def:".\TranCtrl.def" /out:"../../Bin/T1ATM_Release/TranCtrl.dll" /implib:"$(OUTDIR)\TranCtrl.lib" 
DEF_FILE= \
	".\TranCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\TranBeginProc.obj" \
	"$(INTDIR)\TranClerkProc.obj" \
	"$(INTDIR)\TranDepProc.obj" \
	"$(INTDIR)\TranEndProc.obj" \
	"$(INTDIR)\TranFuncProc.obj" \
	"$(INTDIR)\TranICProc.obj" \
	"$(INTDIR)\TranInqProc.obj" \
	"$(INTDIR)\TranPbProc.obj" \
	"$(INTDIR)\TranRFProc.obj" \
	"$(INTDIR)\TranTranProc.obj" \
	"$(INTDIR)\TranTransProc.obj" \
	"$(INTDIR)\TranWithProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TranAjmsProc.obj" \
	"$(INTDIR)\TranCmn.obj" \
	"$(INTDIR)\TranCtrl.obj" \
	"$(INTDIR)\TranDevProc.obj" \
	"$(INTDIR)\TranHostProc.obj" \
	"$(INTDIR)\TranInputProc.obj" \
	"$(INTDIR)\TranKCashProc.obj" \
	"$(INTDIR)\TranMainProc.obj" \
	"$(INTDIR)\TranMenuProc.obj" \
	"$(INTDIR)\TranNmsProc.obj" \
	"$(INTDIR)\TranOutputProc.obj" \
	"$(INTDIR)\TranTermProc.obj" \
	"$(INTDIR)\TranCtrl.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\NetCtrl.lib"

"..\..\Bin\T1ATM_Release\TranCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

OUTDIR=.\../../Lib/T1ATM_Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Debug\TranCtrl.dll"

!ELSE 

ALL : "NetCtrl - Win32 Debug" "ProCtrl - Win32 Debug" "DevCtrl - Win32 Debug" "..\..\Bin\T1ATM_Debug\TranCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 DebugCLEAN" "ProCtrl - Win32 DebugCLEAN" "NetCtrl - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TranAjmsProc.obj"
	-@erase "$(INTDIR)\TranBeginProc.obj"
	-@erase "$(INTDIR)\TranClerkProc.obj"
	-@erase "$(INTDIR)\TranCmn.obj"
	-@erase "$(INTDIR)\TranCtrl.obj"
	-@erase "$(INTDIR)\TranCtrl.pch"
	-@erase "$(INTDIR)\TranCtrl.res"
	-@erase "$(INTDIR)\TranDepProc.obj"
	-@erase "$(INTDIR)\TranDevProc.obj"
	-@erase "$(INTDIR)\TranEndProc.obj"
	-@erase "$(INTDIR)\TranFuncProc.obj"
	-@erase "$(INTDIR)\TranHostProc.obj"
	-@erase "$(INTDIR)\TranICProc.obj"
	-@erase "$(INTDIR)\TranInputProc.obj"
	-@erase "$(INTDIR)\TranInqProc.obj"
	-@erase "$(INTDIR)\TranKCashProc.obj"
	-@erase "$(INTDIR)\TranMainProc.obj"
	-@erase "$(INTDIR)\TranMenuProc.obj"
	-@erase "$(INTDIR)\TranNmsProc.obj"
	-@erase "$(INTDIR)\TranOutputProc.obj"
	-@erase "$(INTDIR)\TranPbProc.obj"
	-@erase "$(INTDIR)\TranRFProc.obj"
	-@erase "$(INTDIR)\TranTermProc.obj"
	-@erase "$(INTDIR)\TranTranProc.obj"
	-@erase "$(INTDIR)\TranTransProc.obj"
	-@erase "$(INTDIR)\TranWithProc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TranCtrl.exp"
	-@erase "$(OUTDIR)\TranCtrl.lib"
	-@erase "$(OUTDIR)\TranCtrl.pdb"
	-@erase "..\..\Bin\T1ATM_Debug\TranCtrl.dll"
	-@erase "..\..\Bin\T1ATM_Debug\TranCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\TranCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TranCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\TranCtrl.pdb" /debug /machine:I386 /def:".\TranCtrl.def" /out:"../../Bin/T1ATM_Debug/TranCtrl.dll" /implib:"$(OUTDIR)\TranCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\TranCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\TranBeginProc.obj" \
	"$(INTDIR)\TranClerkProc.obj" \
	"$(INTDIR)\TranDepProc.obj" \
	"$(INTDIR)\TranEndProc.obj" \
	"$(INTDIR)\TranFuncProc.obj" \
	"$(INTDIR)\TranICProc.obj" \
	"$(INTDIR)\TranInqProc.obj" \
	"$(INTDIR)\TranPbProc.obj" \
	"$(INTDIR)\TranRFProc.obj" \
	"$(INTDIR)\TranTranProc.obj" \
	"$(INTDIR)\TranTransProc.obj" \
	"$(INTDIR)\TranWithProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TranAjmsProc.obj" \
	"$(INTDIR)\TranCmn.obj" \
	"$(INTDIR)\TranCtrl.obj" \
	"$(INTDIR)\TranDevProc.obj" \
	"$(INTDIR)\TranHostProc.obj" \
	"$(INTDIR)\TranInputProc.obj" \
	"$(INTDIR)\TranKCashProc.obj" \
	"$(INTDIR)\TranMainProc.obj" \
	"$(INTDIR)\TranMenuProc.obj" \
	"$(INTDIR)\TranNmsProc.obj" \
	"$(INTDIR)\TranOutputProc.obj" \
	"$(INTDIR)\TranTermProc.obj" \
	"$(INTDIR)\TranCtrl.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\NetCtrl.lib"

"..\..\Bin\T1ATM_Debug\TranCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Release"

OUTDIR=.\../../Lib/T2ATM_Release
INTDIR=.\T2_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Release\TranCtrl.dll"

!ELSE 

ALL : "NetCtrl - Win32 T2 Release" "ProCtrl - Win32 T2 Release" "DevCtrl - Win32 T2 Release" "..\..\Bin\T2ATM_Release\TranCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 T2 ReleaseCLEAN" "ProCtrl - Win32 T2 ReleaseCLEAN" "NetCtrl - Win32 T2 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TranAjmsProc.obj"
	-@erase "$(INTDIR)\TranBeginProc.obj"
	-@erase "$(INTDIR)\TranClerkProc.obj"
	-@erase "$(INTDIR)\TranCmn.obj"
	-@erase "$(INTDIR)\TranCtrl.obj"
	-@erase "$(INTDIR)\TranCtrl.pch"
	-@erase "$(INTDIR)\TranCtrl.res"
	-@erase "$(INTDIR)\TranDepProc.obj"
	-@erase "$(INTDIR)\TranDevProc.obj"
	-@erase "$(INTDIR)\TranEndProc.obj"
	-@erase "$(INTDIR)\TranFuncProc.obj"
	-@erase "$(INTDIR)\TranHostProc.obj"
	-@erase "$(INTDIR)\TranICProc.obj"
	-@erase "$(INTDIR)\TranInputProc.obj"
	-@erase "$(INTDIR)\TranInqProc.obj"
	-@erase "$(INTDIR)\TranKCashProc.obj"
	-@erase "$(INTDIR)\TranMainProc.obj"
	-@erase "$(INTDIR)\TranMenuProc.obj"
	-@erase "$(INTDIR)\TranNmsProc.obj"
	-@erase "$(INTDIR)\TranOutputProc.obj"
	-@erase "$(INTDIR)\TranPbProc.obj"
	-@erase "$(INTDIR)\TranRFProc.obj"
	-@erase "$(INTDIR)\TranTermProc.obj"
	-@erase "$(INTDIR)\TranTranProc.obj"
	-@erase "$(INTDIR)\TranTransProc.obj"
	-@erase "$(INTDIR)\TranWithProc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TranCtrl.exp"
	-@erase "$(OUTDIR)\TranCtrl.lib"
	-@erase "..\..\Bin\T2ATM_Release\TranCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\TranCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TranCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\TranCtrl.pdb" /machine:I386 /def:".\TranCtrl.def" /out:"../../Bin/T2ATM_Release/TranCtrl.dll" /implib:"$(OUTDIR)\TranCtrl.lib" 
DEF_FILE= \
	".\TranCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\TranBeginProc.obj" \
	"$(INTDIR)\TranClerkProc.obj" \
	"$(INTDIR)\TranDepProc.obj" \
	"$(INTDIR)\TranEndProc.obj" \
	"$(INTDIR)\TranFuncProc.obj" \
	"$(INTDIR)\TranICProc.obj" \
	"$(INTDIR)\TranInqProc.obj" \
	"$(INTDIR)\TranPbProc.obj" \
	"$(INTDIR)\TranRFProc.obj" \
	"$(INTDIR)\TranTranProc.obj" \
	"$(INTDIR)\TranTransProc.obj" \
	"$(INTDIR)\TranWithProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TranAjmsProc.obj" \
	"$(INTDIR)\TranCmn.obj" \
	"$(INTDIR)\TranCtrl.obj" \
	"$(INTDIR)\TranDevProc.obj" \
	"$(INTDIR)\TranHostProc.obj" \
	"$(INTDIR)\TranInputProc.obj" \
	"$(INTDIR)\TranKCashProc.obj" \
	"$(INTDIR)\TranMainProc.obj" \
	"$(INTDIR)\TranMenuProc.obj" \
	"$(INTDIR)\TranNmsProc.obj" \
	"$(INTDIR)\TranOutputProc.obj" \
	"$(INTDIR)\TranTermProc.obj" \
	"$(INTDIR)\TranCtrl.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\NetCtrl.lib"

"..\..\Bin\T2ATM_Release\TranCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Debug"

OUTDIR=.\../../Lib/T2ATM_Debug
INTDIR=.\T2_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Debug\TranCtrl.dll"

!ELSE 

ALL : "NetCtrl - Win32 T2 Debug" "ProCtrl - Win32 T2 Debug" "DevCtrl - Win32 T2 Debug" "..\..\Bin\T2ATM_Debug\TranCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 T2 DebugCLEAN" "ProCtrl - Win32 T2 DebugCLEAN" "NetCtrl - Win32 T2 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TranAjmsProc.obj"
	-@erase "$(INTDIR)\TranBeginProc.obj"
	-@erase "$(INTDIR)\TranClerkProc.obj"
	-@erase "$(INTDIR)\TranCmn.obj"
	-@erase "$(INTDIR)\TranCtrl.obj"
	-@erase "$(INTDIR)\TranCtrl.pch"
	-@erase "$(INTDIR)\TranCtrl.res"
	-@erase "$(INTDIR)\TranDepProc.obj"
	-@erase "$(INTDIR)\TranDevProc.obj"
	-@erase "$(INTDIR)\TranEndProc.obj"
	-@erase "$(INTDIR)\TranFuncProc.obj"
	-@erase "$(INTDIR)\TranHostProc.obj"
	-@erase "$(INTDIR)\TranICProc.obj"
	-@erase "$(INTDIR)\TranInputProc.obj"
	-@erase "$(INTDIR)\TranInqProc.obj"
	-@erase "$(INTDIR)\TranKCashProc.obj"
	-@erase "$(INTDIR)\TranMainProc.obj"
	-@erase "$(INTDIR)\TranMenuProc.obj"
	-@erase "$(INTDIR)\TranNmsProc.obj"
	-@erase "$(INTDIR)\TranOutputProc.obj"
	-@erase "$(INTDIR)\TranPbProc.obj"
	-@erase "$(INTDIR)\TranRFProc.obj"
	-@erase "$(INTDIR)\TranTermProc.obj"
	-@erase "$(INTDIR)\TranTranProc.obj"
	-@erase "$(INTDIR)\TranTransProc.obj"
	-@erase "$(INTDIR)\TranWithProc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TranCtrl.exp"
	-@erase "$(OUTDIR)\TranCtrl.lib"
	-@erase "$(OUTDIR)\TranCtrl.pdb"
	-@erase "..\..\Bin\T2ATM_Debug\TranCtrl.dll"
	-@erase "..\..\Bin\T2ATM_Debug\TranCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\TranCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TranCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\TranCtrl.pdb" /debug /machine:I386 /def:".\TranCtrl.def" /out:"../../Bin/T2ATM_Debug/TranCtrl.dll" /implib:"$(OUTDIR)\TranCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\TranCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\TranBeginProc.obj" \
	"$(INTDIR)\TranClerkProc.obj" \
	"$(INTDIR)\TranDepProc.obj" \
	"$(INTDIR)\TranEndProc.obj" \
	"$(INTDIR)\TranFuncProc.obj" \
	"$(INTDIR)\TranICProc.obj" \
	"$(INTDIR)\TranInqProc.obj" \
	"$(INTDIR)\TranPbProc.obj" \
	"$(INTDIR)\TranRFProc.obj" \
	"$(INTDIR)\TranTranProc.obj" \
	"$(INTDIR)\TranTransProc.obj" \
	"$(INTDIR)\TranWithProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TranAjmsProc.obj" \
	"$(INTDIR)\TranCmn.obj" \
	"$(INTDIR)\TranCtrl.obj" \
	"$(INTDIR)\TranDevProc.obj" \
	"$(INTDIR)\TranHostProc.obj" \
	"$(INTDIR)\TranInputProc.obj" \
	"$(INTDIR)\TranKCashProc.obj" \
	"$(INTDIR)\TranMainProc.obj" \
	"$(INTDIR)\TranMenuProc.obj" \
	"$(INTDIR)\TranNmsProc.obj" \
	"$(INTDIR)\TranOutputProc.obj" \
	"$(INTDIR)\TranTermProc.obj" \
	"$(INTDIR)\TranCtrl.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\NetCtrl.lib"

"..\..\Bin\T2ATM_Debug\TranCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TranCtrl.dep")
!INCLUDE "TranCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "TranCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TranCtrl - Win32 Release" || "$(CFG)" == "TranCtrl - Win32 Debug" || "$(CFG)" == "TranCtrl - Win32 T2 Release" || "$(CFG)" == "TranCtrl - Win32 T2 Debug"
SOURCE=..\..\H\Builder\TranBeginProc.cpp

"$(INTDIR)\TranBeginProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranClerkProc.cpp

"$(INTDIR)\TranClerkProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranDepProc.cpp

"$(INTDIR)\TranDepProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranEndProc.cpp

"$(INTDIR)\TranEndProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranFuncProc.cpp

"$(INTDIR)\TranFuncProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranICProc.cpp

"$(INTDIR)\TranICProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranInqProc.cpp

"$(INTDIR)\TranInqProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranPbProc.cpp

"$(INTDIR)\TranPbProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranRFProc.cpp

"$(INTDIR)\TranRFProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranTranProc.cpp

"$(INTDIR)\TranTranProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranTransProc.cpp

"$(INTDIR)\TranTransProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\H\Builder\TranWithProc.cpp

"$(INTDIR)\TranWithProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TranCtrl - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TranCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TranCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TranCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\TranCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TranCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TranAjmsProc.cpp

"$(INTDIR)\TranAjmsProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranCmn.cpp

"$(INTDIR)\TranCmn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranCtrl.cpp

"$(INTDIR)\TranCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranCtrl.rc

"$(INTDIR)\TranCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TranDevProc.cpp

"$(INTDIR)\TranDevProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranHostProc.cpp

"$(INTDIR)\TranHostProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranInputProc.cpp

"$(INTDIR)\TranInputProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranKCashProc.cpp

"$(INTDIR)\TranKCashProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranMainProc.cpp

"$(INTDIR)\TranMainProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranMenuProc.cpp

"$(INTDIR)\TranMenuProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranNmsProc.cpp

"$(INTDIR)\TranNmsProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranOutputProc.cpp

"$(INTDIR)\TranOutputProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


SOURCE=.\TranTermProc.cpp

"$(INTDIR)\TranTermProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TranCtrl.pch"


!IF  "$(CFG)" == "TranCtrl - Win32 Release"

"DevCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Release" 
   cd "..\TranCtrl"

"DevCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

"DevCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Debug" 
   cd "..\TranCtrl"

"DevCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Release"

"DevCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Release" 
   cd "..\TranCtrl"

"DevCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Debug"

"DevCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Debug" 
   cd "..\TranCtrl"

"DevCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ENDIF 

!IF  "$(CFG)" == "TranCtrl - Win32 Release"

"ProCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" 
   cd "..\TranCtrl"

"ProCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

"ProCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" 
   cd "..\TranCtrl"

"ProCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Release"

"ProCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" 
   cd "..\TranCtrl"

"ProCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Debug"

"ProCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" 
   cd "..\TranCtrl"

"ProCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ENDIF 

!IF  "$(CFG)" == "TranCtrl - Win32 Release"

"NetCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Release" 
   cd "..\TranCtrl"

"NetCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

"NetCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Debug" 
   cd "..\TranCtrl"

"NetCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Release"

"NetCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Release" 
   cd "..\TranCtrl"

"NetCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 T2 Debug"

"NetCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Debug" 
   cd "..\TranCtrl"

"NetCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\TranCtrl"

!ENDIF 


!ENDIF 

