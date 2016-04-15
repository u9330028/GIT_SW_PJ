# Microsoft Developer Studio Generated NMAKE File, Based on WinAtm.dsp
!IF "$(CFG)" == ""
CFG=WinAtm - Win32 T2 Debug
!MESSAGE No configuration specified. Defaulting to WinAtm - Win32 T2 Debug.
!ENDIF 

!IF "$(CFG)" != "WinAtm - Win32 Release" && "$(CFG)" != "WinAtm - Win32 Debug" && "$(CFG)" != "WinAtm - Win32 T2 Release" && "$(CFG)" != "WinAtm - Win32 T2 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinAtm.mak" CFG="WinAtm - Win32 T2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinAtm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WinAtm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WinAtm - Win32 T2 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WinAtm - Win32 T2 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "WinAtm - Win32 Release"

OUTDIR=.\../Lib/T1ATM_Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../Lib/T1ATM_Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\T1ATM_Release\WinAtm.ocx" "$(OUTDIR)\WinAtm.tlb" "$(OUTDIR)\WinAtm.bsc" ".\../Lib/T1ATM_Release\regsvr32.trg"

!ELSE 

ALL : "TranCtrl - Win32 Release" "ProCtrl - Win32 Release" "DevCtrl - Win32 Release" "..\Bin\T1ATM_Release\WinAtm.ocx" "$(OUTDIR)\WinAtm.tlb" "$(OUTDIR)\WinAtm.bsc" ".\../Lib/T1ATM_Release\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 ReleaseCLEAN" "ProCtrl - Win32 ReleaseCLEAN" "TranCtrl - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AtmClerkCtrl.obj"
	-@erase "$(INTDIR)\AtmClerkCtrl.sbr"
	-@erase "$(INTDIR)\AtmHostCtrl.obj"
	-@erase "$(INTDIR)\AtmHostCtrl.sbr"
	-@erase "$(INTDIR)\AtmModeCtrl.obj"
	-@erase "$(INTDIR)\AtmModeCtrl.sbr"
	-@erase "$(INTDIR)\AtmTranCtrl.obj"
	-@erase "$(INTDIR)\AtmTranCtrl.sbr"
	-@erase "$(INTDIR)\CdClerkCtrl.obj"
	-@erase "$(INTDIR)\CdClerkCtrl.sbr"
	-@erase "$(INTDIR)\handleocr.obj"
	-@erase "$(INTDIR)\handleocr.sbr"
	-@erase "$(INTDIR)\Helpers.obj"
	-@erase "$(INTDIR)\Helpers.sbr"
	-@erase "$(INTDIR)\nhguros.obj"
	-@erase "$(INTDIR)\nhguros.sbr"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\nhmwi.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinAtm.obj"
	-@erase "$(INTDIR)\WinAtm.pch"
	-@erase "$(INTDIR)\WinAtm.res"
	-@erase "$(INTDIR)\WinAtm.sbr"
	-@erase "$(INTDIR)\WinAtmCtl.obj"
	-@erase "$(INTDIR)\WinAtmCtl.sbr"
	-@erase "$(INTDIR)\WinAtmPpg.obj"
	-@erase "$(INTDIR)\WinAtmPpg.sbr"
	-@erase "$(OUTDIR)\WinAtm.bsc"
	-@erase "$(OUTDIR)\WinAtm.exp"
	-@erase "$(OUTDIR)\WinAtm.lib"
	-@erase "..\Bin\T1ATM_Release\WinAtm.ocx"
	-@erase "..\Lib\T1ATM_Release\WinAtm.tlb"
	-@erase ".\../Lib/T1ATM_Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T1ATM_MODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WinAtm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\WinAtm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WinAtm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AtmClerkCtrl.sbr" \
	"$(INTDIR)\AtmHostCtrl.sbr" \
	"$(INTDIR)\AtmModeCtrl.sbr" \
	"$(INTDIR)\AtmTranCtrl.sbr" \
	"$(INTDIR)\CdClerkCtrl.sbr" \
	"$(INTDIR)\handleocr.sbr" \
	"$(INTDIR)\Helpers.sbr" \
	"$(INTDIR)\nhguros.sbr" \
	"$(INTDIR)\nhmwi.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WinAtm.sbr" \
	"$(INTDIR)\WinAtmCtl.sbr" \
	"$(INTDIR)\WinAtmPpg.sbr"

"$(OUTDIR)\WinAtm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\WinAtm.pdb" /machine:I386 /def:".\WinAtm.def" /out:"../Bin/T1ATM_Release/WinAtm.ocx" /implib:"$(OUTDIR)\WinAtm.lib" 
DEF_FILE= \
	".\WinAtm.def"
LINK32_OBJS= \
	"$(INTDIR)\AtmClerkCtrl.obj" \
	"$(INTDIR)\AtmHostCtrl.obj" \
	"$(INTDIR)\AtmModeCtrl.obj" \
	"$(INTDIR)\AtmTranCtrl.obj" \
	"$(INTDIR)\CdClerkCtrl.obj" \
	"$(INTDIR)\handleocr.obj" \
	"$(INTDIR)\Helpers.obj" \
	"$(INTDIR)\nhguros.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinAtm.obj" \
	"$(INTDIR)\WinAtmCtl.obj" \
	"$(INTDIR)\WinAtmPpg.obj" \
	"$(INTDIR)\WinAtm.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\TranCtrl.lib"

"..\Bin\T1ATM_Release\WinAtm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\../Lib/T1ATM_Release
TargetPath=\T1ATM\SRC\JBB\Bin\T1ATM_Release\WinAtm.ocx
InputPath=\T1ATM\SRC\JBB\Bin\T1ATM_Release\WinAtm.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

OUTDIR=.\../Lib/T1ATM_Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\T1ATM_Debug\WinAtm.ocx" ".\../Lib/T1ATM_Debug\regsvr32.trg"

!ELSE 

ALL : "TranCtrl - Win32 Debug" "ProCtrl - Win32 Debug" "DevCtrl - Win32 Debug" "..\Bin\T1ATM_Debug\WinAtm.ocx" ".\../Lib/T1ATM_Debug\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 DebugCLEAN" "ProCtrl - Win32 DebugCLEAN" "TranCtrl - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AtmClerkCtrl.obj"
	-@erase "$(INTDIR)\AtmHostCtrl.obj"
	-@erase "$(INTDIR)\AtmModeCtrl.obj"
	-@erase "$(INTDIR)\AtmTranCtrl.obj"
	-@erase "$(INTDIR)\CdClerkCtrl.obj"
	-@erase "$(INTDIR)\handleocr.obj"
	-@erase "$(INTDIR)\Helpers.obj"
	-@erase "$(INTDIR)\nhguros.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinAtm.obj"
	-@erase "$(INTDIR)\WinAtm.pch"
	-@erase "$(INTDIR)\WinAtm.res"
	-@erase "$(INTDIR)\WinAtmCtl.obj"
	-@erase "$(INTDIR)\WinAtmPpg.obj"
	-@erase "$(OUTDIR)\WinAtm.exp"
	-@erase "$(OUTDIR)\WinAtm.lib"
	-@erase "$(OUTDIR)\WinAtm.pdb"
	-@erase "..\Bin\T1ATM_Debug\WinAtm.ilk"
	-@erase "..\Bin\T1ATM_Debug\WinAtm.ocx"
	-@erase "..\Lib\T1ATM_Debug\WinAtm.tlb"
	-@erase ".\../Lib/T1ATM_Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T1ATM_MODE" /Fp"$(INTDIR)\WinAtm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\WinAtm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WinAtm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\WinAtm.pdb" /debug /machine:I386 /def:".\WinAtm.def" /out:"../Bin/T1ATM_Debug/WinAtm.ocx" /implib:"$(OUTDIR)\WinAtm.lib" /pdbtype:sept 
DEF_FILE= \
	".\WinAtm.def"
LINK32_OBJS= \
	"$(INTDIR)\AtmClerkCtrl.obj" \
	"$(INTDIR)\AtmHostCtrl.obj" \
	"$(INTDIR)\AtmModeCtrl.obj" \
	"$(INTDIR)\AtmTranCtrl.obj" \
	"$(INTDIR)\CdClerkCtrl.obj" \
	"$(INTDIR)\handleocr.obj" \
	"$(INTDIR)\Helpers.obj" \
	"$(INTDIR)\nhguros.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinAtm.obj" \
	"$(INTDIR)\WinAtmCtl.obj" \
	"$(INTDIR)\WinAtmPpg.obj" \
	"$(INTDIR)\WinAtm.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\TranCtrl.lib"

"..\Bin\T1ATM_Debug\WinAtm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\../Lib/T1ATM_Debug
TargetPath=\T1ATM\SRC\JBB\Bin\T1ATM_Debug\WinAtm.ocx
InputPath=\T1ATM\SRC\JBB\Bin\T1ATM_Debug\WinAtm.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

OUTDIR=.\../Lib/T2ATM_Release
INTDIR=.\T2_Release
# Begin Custom Macros
OutDir=.\../Lib/T2ATM_Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\T2ATM_Release\WinAtm.ocx" "$(OUTDIR)\WinAtm.bsc" ".\../Lib/T2ATM_Release\regsvr32.trg"

!ELSE 

ALL : "TranCtrl - Win32 T2 Release" "ProCtrl - Win32 T2 Release" "DevCtrl - Win32 T2 Release" "..\Bin\T2ATM_Release\WinAtm.ocx" "$(OUTDIR)\WinAtm.bsc" ".\../Lib/T2ATM_Release\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 T2 ReleaseCLEAN" "ProCtrl - Win32 T2 ReleaseCLEAN" "TranCtrl - Win32 T2 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AtmClerkCtrl.obj"
	-@erase "$(INTDIR)\AtmClerkCtrl.sbr"
	-@erase "$(INTDIR)\AtmHostCtrl.obj"
	-@erase "$(INTDIR)\AtmHostCtrl.sbr"
	-@erase "$(INTDIR)\AtmModeCtrl.obj"
	-@erase "$(INTDIR)\AtmModeCtrl.sbr"
	-@erase "$(INTDIR)\AtmTranCtrl.obj"
	-@erase "$(INTDIR)\AtmTranCtrl.sbr"
	-@erase "$(INTDIR)\CdClerkCtrl.obj"
	-@erase "$(INTDIR)\CdClerkCtrl.sbr"
	-@erase "$(INTDIR)\handleocr.obj"
	-@erase "$(INTDIR)\handleocr.sbr"
	-@erase "$(INTDIR)\Helpers.obj"
	-@erase "$(INTDIR)\Helpers.sbr"
	-@erase "$(INTDIR)\nhguros.obj"
	-@erase "$(INTDIR)\nhguros.sbr"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\nhmwi.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinAtm.obj"
	-@erase "$(INTDIR)\WinAtm.pch"
	-@erase "$(INTDIR)\WinAtm.res"
	-@erase "$(INTDIR)\WinAtm.sbr"
	-@erase "$(INTDIR)\WinAtmCtl.obj"
	-@erase "$(INTDIR)\WinAtmCtl.sbr"
	-@erase "$(INTDIR)\WinAtmPpg.obj"
	-@erase "$(INTDIR)\WinAtmPpg.sbr"
	-@erase "$(OUTDIR)\WinAtm.bsc"
	-@erase "$(OUTDIR)\WinAtm.exp"
	-@erase "$(OUTDIR)\WinAtm.lib"
	-@erase "..\Bin\T2ATM_Release\WinAtm.ocx"
	-@erase "..\Lib\T2ATM_Release\WinAtm.tlb"
	-@erase ".\../Lib/T2ATM_Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T2ATM_MODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WinAtm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\WinAtm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WinAtm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AtmClerkCtrl.sbr" \
	"$(INTDIR)\AtmHostCtrl.sbr" \
	"$(INTDIR)\AtmModeCtrl.sbr" \
	"$(INTDIR)\AtmTranCtrl.sbr" \
	"$(INTDIR)\CdClerkCtrl.sbr" \
	"$(INTDIR)\handleocr.sbr" \
	"$(INTDIR)\Helpers.sbr" \
	"$(INTDIR)\nhguros.sbr" \
	"$(INTDIR)\nhmwi.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WinAtm.sbr" \
	"$(INTDIR)\WinAtmCtl.sbr" \
	"$(INTDIR)\WinAtmPpg.sbr"

"$(OUTDIR)\WinAtm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\WinAtm.pdb" /machine:I386 /def:".\WinAtm.def" /out:"../Bin/T2ATM_Release/WinAtm.ocx" /implib:"$(OUTDIR)\WinAtm.lib" 
DEF_FILE= \
	".\WinAtm.def"
LINK32_OBJS= \
	"$(INTDIR)\AtmClerkCtrl.obj" \
	"$(INTDIR)\AtmHostCtrl.obj" \
	"$(INTDIR)\AtmModeCtrl.obj" \
	"$(INTDIR)\AtmTranCtrl.obj" \
	"$(INTDIR)\CdClerkCtrl.obj" \
	"$(INTDIR)\handleocr.obj" \
	"$(INTDIR)\Helpers.obj" \
	"$(INTDIR)\nhguros.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinAtm.obj" \
	"$(INTDIR)\WinAtmCtl.obj" \
	"$(INTDIR)\WinAtmPpg.obj" \
	"$(INTDIR)\WinAtm.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\TranCtrl.lib"

"..\Bin\T2ATM_Release\WinAtm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\../Lib/T2ATM_Release
TargetPath=\T1ATM\SRC\JBB\Bin\T2ATM_Release\WinAtm.ocx
InputPath=\T1ATM\SRC\JBB\Bin\T2ATM_Release\WinAtm.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

OUTDIR=.\../Lib/T2ATM_Debug
INTDIR=.\T2_Debug
# Begin Custom Macros
OutDir=.\../Lib/T2ATM_Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\T2ATM_Debug\WinAtm.ocx" "$(OUTDIR)\WinAtm.tlb" ".\../Lib/T2ATM_Debug\regsvr32.trg"

!ELSE 

ALL : "TranCtrl - Win32 T2 Debug" "ProCtrl - Win32 T2 Debug" "DevCtrl - Win32 T2 Debug" "..\Bin\T2ATM_Debug\WinAtm.ocx" "$(OUTDIR)\WinAtm.tlb" ".\../Lib/T2ATM_Debug\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"DevCtrl - Win32 T2 DebugCLEAN" "ProCtrl - Win32 T2 DebugCLEAN" "TranCtrl - Win32 T2 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AtmClerkCtrl.obj"
	-@erase "$(INTDIR)\AtmHostCtrl.obj"
	-@erase "$(INTDIR)\AtmModeCtrl.obj"
	-@erase "$(INTDIR)\AtmTranCtrl.obj"
	-@erase "$(INTDIR)\CdClerkCtrl.obj"
	-@erase "$(INTDIR)\handleocr.obj"
	-@erase "$(INTDIR)\Helpers.obj"
	-@erase "$(INTDIR)\nhguros.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinAtm.obj"
	-@erase "$(INTDIR)\WinAtm.pch"
	-@erase "$(INTDIR)\WinAtm.res"
	-@erase "$(INTDIR)\WinAtmCtl.obj"
	-@erase "$(INTDIR)\WinAtmPpg.obj"
	-@erase "$(OUTDIR)\WinAtm.exp"
	-@erase "$(OUTDIR)\WinAtm.lib"
	-@erase "$(OUTDIR)\WinAtm.pdb"
	-@erase "..\Bin\T2ATM_Debug\WinAtm.ilk"
	-@erase "..\Bin\T2ATM_Debug\WinAtm.ocx"
	-@erase "..\Lib\T2ATM_Debug\WinAtm.tlb"
	-@erase ".\../Lib/T2ATM_Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T2ATM_MODE" /Fp"$(INTDIR)\WinAtm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\WinAtm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WinAtm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\WinAtm.pdb" /debug /machine:I386 /def:".\WinAtm.def" /out:"../Bin/T2ATM_Debug/WinAtm.ocx" /implib:"$(OUTDIR)\WinAtm.lib" /pdbtype:sept 
DEF_FILE= \
	".\WinAtm.def"
LINK32_OBJS= \
	"$(INTDIR)\AtmClerkCtrl.obj" \
	"$(INTDIR)\AtmHostCtrl.obj" \
	"$(INTDIR)\AtmModeCtrl.obj" \
	"$(INTDIR)\AtmTranCtrl.obj" \
	"$(INTDIR)\CdClerkCtrl.obj" \
	"$(INTDIR)\handleocr.obj" \
	"$(INTDIR)\Helpers.obj" \
	"$(INTDIR)\nhguros.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinAtm.obj" \
	"$(INTDIR)\WinAtmCtl.obj" \
	"$(INTDIR)\WinAtmPpg.obj" \
	"$(INTDIR)\WinAtm.res" \
	"$(OUTDIR)\DevCtrl.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\TranCtrl.lib"

"..\Bin\T2ATM_Debug\WinAtm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\../Lib/T2ATM_Debug
TargetPath=\T1ATM\SRC\JBB\Bin\T2ATM_Debug\WinAtm.ocx
InputPath=\T1ATM\SRC\JBB\Bin\T2ATM_Debug\WinAtm.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WinAtm.dep")
!INCLUDE "WinAtm.dep"
!ELSE 
!MESSAGE Warning: cannot find "WinAtm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WinAtm - Win32 Release" || "$(CFG)" == "WinAtm - Win32 Debug" || "$(CFG)" == "WinAtm - Win32 T2 Release" || "$(CFG)" == "WinAtm - Win32 T2 Debug"
SOURCE=.\AtmClerkCtrl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\AtmClerkCtrl.obj"	"$(INTDIR)\AtmClerkCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\AtmClerkCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\AtmClerkCtrl.obj"	"$(INTDIR)\AtmClerkCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\AtmClerkCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\AtmHostCtrl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\AtmHostCtrl.obj"	"$(INTDIR)\AtmHostCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\AtmHostCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\AtmHostCtrl.obj"	"$(INTDIR)\AtmHostCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\AtmHostCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\AtmModeCtrl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\AtmModeCtrl.obj"	"$(INTDIR)\AtmModeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\AtmModeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\AtmModeCtrl.obj"	"$(INTDIR)\AtmModeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\AtmModeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\AtmTranCtrl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\AtmTranCtrl.obj"	"$(INTDIR)\AtmTranCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\AtmTranCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\AtmTranCtrl.obj"	"$(INTDIR)\AtmTranCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\AtmTranCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\CdClerkCtrl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\CdClerkCtrl.obj"	"$(INTDIR)\CdClerkCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\CdClerkCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\CdClerkCtrl.obj"	"$(INTDIR)\CdClerkCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\CdClerkCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\handleocr.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\handleocr.obj"	"$(INTDIR)\handleocr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\handleocr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\handleocr.obj"	"$(INTDIR)\handleocr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\handleocr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\Helpers.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\Helpers.obj"	"$(INTDIR)\Helpers.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\Helpers.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\Helpers.obj"	"$(INTDIR)\Helpers.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\Helpers.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\nhguros.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\nhguros.obj"	"$(INTDIR)\nhguros.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\nhguros.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\nhguros.obj"	"$(INTDIR)\nhguros.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\nhguros.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\nhmwi.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\nhmwi.obj"	"$(INTDIR)\nhmwi.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\nhmwi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\nhmwi.obj"	"$(INTDIR)\nhmwi.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\nhmwi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T1ATM_MODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WinAtm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\WinAtm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T1ATM_MODE" /Fp"$(INTDIR)\WinAtm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\WinAtm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T2ATM_MODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WinAtm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\WinAtm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "T2ATM_MODE" /Fp"$(INTDIR)\WinAtm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\WinAtm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WinAtm.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\WinAtm.obj"	"$(INTDIR)\WinAtm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\WinAtm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\WinAtm.obj"	"$(INTDIR)\WinAtm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\WinAtm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\WinAtm.odl

!IF  "$(CFG)" == "WinAtm - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\WinAtm.tlb" /mktyplib203 /win32 

"$(OUTDIR)\WinAtm.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\WinAtm.tlb" /mktyplib203 /win32 

"$(OUTDIR)\WinAtm.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\WinAtm.tlb" /mktyplib203 /win32 

"$(OUTDIR)\WinAtm.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\WinAtm.tlb" /mktyplib203 /win32 

"$(OUTDIR)\WinAtm.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WinAtm.rc

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\WinAtm.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\WinAtm.res" /i "../Lib/T1ATM_Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\WinAtm.res" : $(SOURCE) "$(INTDIR)" "..\Lib\T1ATM_Debug\WinAtm.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\WinAtm.res" /i "../Lib/T1ATM_Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\WinAtm.res" : $(SOURCE) "$(INTDIR)" "..\Lib\T2ATM_Release\WinAtm.tlb"
	$(RSC) /l 0x412 /fo"$(INTDIR)\WinAtm.res" /i "../Lib/T2ATM_Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\WinAtm.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x412 /fo"$(INTDIR)\WinAtm.res" /i "../Lib/T2ATM_Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\WinAtmCtl.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\WinAtmCtl.obj"	"$(INTDIR)\WinAtmCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\WinAtmCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\WinAtmCtl.obj"	"$(INTDIR)\WinAtmCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\WinAtmCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

SOURCE=.\WinAtmPpg.cpp

!IF  "$(CFG)" == "WinAtm - Win32 Release"


"$(INTDIR)\WinAtmPpg.obj"	"$(INTDIR)\WinAtmPpg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"


"$(INTDIR)\WinAtmPpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"


"$(INTDIR)\WinAtmPpg.obj"	"$(INTDIR)\WinAtmPpg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"


"$(INTDIR)\WinAtmPpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WinAtm.pch"


!ENDIF 

!IF  "$(CFG)" == "WinAtm - Win32 Release"

"DevCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Release" 
   cd "..\..\WinAtm"

"DevCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

"DevCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Debug" 
   cd "..\..\WinAtm"

"DevCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

"DevCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Release" 
   cd "..\..\WinAtm"

"DevCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

"DevCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Debug" 
   cd "..\..\WinAtm"

"DevCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DevCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\DevCtrl.mak CFG="DevCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ENDIF 

!IF  "$(CFG)" == "WinAtm - Win32 Release"

"ProCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" 
   cd "..\..\WinAtm"

"ProCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

"ProCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" 
   cd "..\..\WinAtm"

"ProCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

"ProCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" 
   cd "..\..\WinAtm"

"ProCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

"ProCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" 
   cd "..\..\WinAtm"

"ProCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ENDIF 

!IF  "$(CFG)" == "WinAtm - Win32 Release"

"TranCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 Release" 
   cd "..\..\WinAtm"

"TranCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

"TranCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 Debug" 
   cd "..\..\WinAtm"

"TranCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

"TranCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 T2 Release" 
   cd "..\..\WinAtm"

"TranCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

"TranCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 T2 Debug" 
   cd "..\..\WinAtm"

"TranCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\TranCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\TranCtrl.mak CFG="TranCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\WinAtm"

!ENDIF 

!IF  "$(CFG)" == "WinAtm - Win32 Release"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 Debug"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Release"

!ELSEIF  "$(CFG)" == "WinAtm - Win32 T2 Debug"

!ENDIF 


!ENDIF 

