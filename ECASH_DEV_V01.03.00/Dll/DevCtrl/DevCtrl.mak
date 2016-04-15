# Microsoft Developer Studio Generated NMAKE File, Based on DevCtrl.dsp
!IF "$(CFG)" == ""
CFG=DevCtrl - Win32 T2 Debug
!MESSAGE No configuration specified. Defaulting to DevCtrl - Win32 T2 Debug.
!ENDIF 

!IF "$(CFG)" != "DevCtrl - Win32 Release" && "$(CFG)" != "DevCtrl - Win32 Debug" && "$(CFG)" != "DevCtrl - Win32 T2 Release" && "$(CFG)" != "DevCtrl - Win32 T2 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DevCtrl.mak" CFG="DevCtrl - Win32 T2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DevCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DevCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DevCtrl - Win32 T2 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DevCtrl - Win32 T2 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

OUTDIR=.\../../Lib/T1ATM_Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Release\DevCtrl.dll"

!ELSE 

ALL : "VolCtrl - Win32 Release" "ProCtrl - Win32 Release" "NTLCPDLL - Win32 Release" "NetCtrl - Win32 Release" "LcpOpen - Win32 Release" "..\..\Bin\T1ATM_Release\DevCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 ReleaseCLEAN" "NetCtrl - Win32 ReleaseCLEAN" "NTLCPDLL - Win32 ReleaseCLEAN" "ProCtrl - Win32 ReleaseCLEAN" "VolCtrl - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DevApl.obj"
	-@erase "$(INTDIR)\DevCmn.obj"
	-@erase "$(INTDIR)\DevCsh.obj"
	-@erase "$(INTDIR)\DevCtrl.obj"
	-@erase "$(INTDIR)\DevCtrl.pch"
	-@erase "$(INTDIR)\DevCtrl.res"
	-@erase "$(INTDIR)\DevEMV.obj"
	-@erase "$(INTDIR)\DevEventQue.obj"
	-@erase "$(INTDIR)\DevFinanceIC.obj"
	-@erase "$(INTDIR)\DevKCash.obj"
	-@erase "$(INTDIR)\DevNet.obj"
	-@erase "$(INTDIR)\DevRDF.obj"
	-@erase "$(INTDIR)\DevScr.obj"
	-@erase "$(INTDIR)\DevSeed.obj"
	-@erase "$(INTDIR)\DevSnd.obj"
	-@erase "$(INTDIR)\DevThread.obj"
	-@erase "$(INTDIR)\DevUtil.obj"
	-@erase "$(INTDIR)\EventQue.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DevCtrl.exp"
	-@erase "$(OUTDIR)\DevCtrl.lib"
	-@erase "..\..\Bin\T1ATM_Release\DevCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\DevCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DevCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\DevCtrl.pdb" /machine:I386 /def:".\DevCtrl.def" /out:"../../Bin/T1ATM_Release/DevCtrl.dll" /implib:"$(OUTDIR)\DevCtrl.lib" 
DEF_FILE= \
	".\DevCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DevApl.obj" \
	"$(INTDIR)\DevCmn.obj" \
	"$(INTDIR)\DevCsh.obj" \
	"$(INTDIR)\DevCtrl.obj" \
	"$(INTDIR)\DevEMV.obj" \
	"$(INTDIR)\DevEventQue.obj" \
	"$(INTDIR)\DevFinanceIC.obj" \
	"$(INTDIR)\DevKCash.obj" \
	"$(INTDIR)\DevNet.obj" \
	"$(INTDIR)\DevRDF.obj" \
	"$(INTDIR)\DevScr.obj" \
	"$(INTDIR)\DevSeed.obj" \
	"$(INTDIR)\DevSnd.obj" \
	"$(INTDIR)\DevThread.obj" \
	"$(INTDIR)\DevUtil.obj" \
	"$(INTDIR)\EventQue.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DevCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib" \
	"$(OUTDIR)\NetCtrl.lib" \
	"$(OUTDIR)\NTLcpDLL.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\VolCtrl.lib"

"..\..\Bin\T1ATM_Release\DevCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

OUTDIR=.\../../Lib/T1ATM_Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Debug\DevCtrl.dll"

!ELSE 

ALL : "VolCtrl - Win32 Debug" "ProCtrl - Win32 Debug" "NTLCPDLL - Win32 Debug" "NetCtrl - Win32 Debug" "LcpOpen - Win32 Debug" "..\..\Bin\T1ATM_Debug\DevCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 DebugCLEAN" "NetCtrl - Win32 DebugCLEAN" "NTLCPDLL - Win32 DebugCLEAN" "ProCtrl - Win32 DebugCLEAN" "VolCtrl - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DevApl.obj"
	-@erase "$(INTDIR)\DevCmn.obj"
	-@erase "$(INTDIR)\DevCsh.obj"
	-@erase "$(INTDIR)\DevCtrl.obj"
	-@erase "$(INTDIR)\DevCtrl.pch"
	-@erase "$(INTDIR)\DevCtrl.res"
	-@erase "$(INTDIR)\DevEMV.obj"
	-@erase "$(INTDIR)\DevEventQue.obj"
	-@erase "$(INTDIR)\DevFinanceIC.obj"
	-@erase "$(INTDIR)\DevKCash.obj"
	-@erase "$(INTDIR)\DevNet.obj"
	-@erase "$(INTDIR)\DevRDF.obj"
	-@erase "$(INTDIR)\DevScr.obj"
	-@erase "$(INTDIR)\DevSeed.obj"
	-@erase "$(INTDIR)\DevSnd.obj"
	-@erase "$(INTDIR)\DevThread.obj"
	-@erase "$(INTDIR)\DevUtil.obj"
	-@erase "$(INTDIR)\EventQue.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DevCtrl.exp"
	-@erase "$(OUTDIR)\DevCtrl.lib"
	-@erase "$(OUTDIR)\DevCtrl.pdb"
	-@erase "..\..\Bin\T1ATM_Debug\DevCtrl.dll"
	-@erase "..\..\Bin\T1ATM_Debug\DevCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\DevCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DevCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\DevCtrl.pdb" /debug /machine:I386 /def:".\DevCtrl.def" /out:"../../Bin/T1ATM_Debug/DevCtrl.dll" /implib:"$(OUTDIR)\DevCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\DevCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DevApl.obj" \
	"$(INTDIR)\DevCmn.obj" \
	"$(INTDIR)\DevCsh.obj" \
	"$(INTDIR)\DevCtrl.obj" \
	"$(INTDIR)\DevEMV.obj" \
	"$(INTDIR)\DevEventQue.obj" \
	"$(INTDIR)\DevFinanceIC.obj" \
	"$(INTDIR)\DevKCash.obj" \
	"$(INTDIR)\DevNet.obj" \
	"$(INTDIR)\DevRDF.obj" \
	"$(INTDIR)\DevScr.obj" \
	"$(INTDIR)\DevSeed.obj" \
	"$(INTDIR)\DevSnd.obj" \
	"$(INTDIR)\DevThread.obj" \
	"$(INTDIR)\DevUtil.obj" \
	"$(INTDIR)\EventQue.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DevCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib" \
	"$(OUTDIR)\NetCtrl.lib" \
	"$(OUTDIR)\NTLcpDLL.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\VolCtrl.lib"

"..\..\Bin\T1ATM_Debug\DevCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

OUTDIR=.\../../Lib/T2ATM_Release
INTDIR=.\T2_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Release\DevCtrl.dll"

!ELSE 

ALL : "VolCtrl - Win32 T2 Release" "ProCtrl - Win32 T2 Release" "NTLCPDLL - Win32 T2 Release" "NetCtrl - Win32 T2 Release" "LcpOpen - Win32 T2 Release" "..\..\Bin\T2ATM_Release\DevCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 T2 ReleaseCLEAN" "NetCtrl - Win32 T2 ReleaseCLEAN" "NTLCPDLL - Win32 T2 ReleaseCLEAN" "ProCtrl - Win32 T2 ReleaseCLEAN" "VolCtrl - Win32 T2 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DevApl.obj"
	-@erase "$(INTDIR)\DevCmn.obj"
	-@erase "$(INTDIR)\DevCsh.obj"
	-@erase "$(INTDIR)\DevCtrl.obj"
	-@erase "$(INTDIR)\DevCtrl.pch"
	-@erase "$(INTDIR)\DevCtrl.res"
	-@erase "$(INTDIR)\DevEMV.obj"
	-@erase "$(INTDIR)\DevEventQue.obj"
	-@erase "$(INTDIR)\DevFinanceIC.obj"
	-@erase "$(INTDIR)\DevKCash.obj"
	-@erase "$(INTDIR)\DevNet.obj"
	-@erase "$(INTDIR)\DevRDF.obj"
	-@erase "$(INTDIR)\DevScr.obj"
	-@erase "$(INTDIR)\DevSeed.obj"
	-@erase "$(INTDIR)\DevSnd.obj"
	-@erase "$(INTDIR)\DevThread.obj"
	-@erase "$(INTDIR)\DevUtil.obj"
	-@erase "$(INTDIR)\EventQue.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DevCtrl.exp"
	-@erase "$(OUTDIR)\DevCtrl.lib"
	-@erase "..\..\Bin\T2ATM_Release\DevCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\DevCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DevCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\DevCtrl.pdb" /machine:I386 /def:".\DevCtrl.def" /out:"../../Bin/T2ATM_Release/DevCtrl.dll" /implib:"$(OUTDIR)\DevCtrl.lib" 
DEF_FILE= \
	".\DevCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DevApl.obj" \
	"$(INTDIR)\DevCmn.obj" \
	"$(INTDIR)\DevCsh.obj" \
	"$(INTDIR)\DevCtrl.obj" \
	"$(INTDIR)\DevEMV.obj" \
	"$(INTDIR)\DevEventQue.obj" \
	"$(INTDIR)\DevFinanceIC.obj" \
	"$(INTDIR)\DevKCash.obj" \
	"$(INTDIR)\DevNet.obj" \
	"$(INTDIR)\DevRDF.obj" \
	"$(INTDIR)\DevScr.obj" \
	"$(INTDIR)\DevSeed.obj" \
	"$(INTDIR)\DevSnd.obj" \
	"$(INTDIR)\DevThread.obj" \
	"$(INTDIR)\DevUtil.obj" \
	"$(INTDIR)\EventQue.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DevCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib" \
	"$(OUTDIR)\NetCtrl.lib" \
	"$(OUTDIR)\NTLcpDLL.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\VolCtrl.lib"

"..\..\Bin\T2ATM_Release\DevCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

OUTDIR=.\../../Lib/T2ATM_Debug
INTDIR=.\T2_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Debug\DevCtrl.dll"

!ELSE 

ALL : "VolCtrl - Win32 T2 Debug" "ProCtrl - Win32 T2 Debug" "NTLCPDLL - Win32 T2 Debug" "NetCtrl - Win32 T2 Debug" "LcpOpen - Win32 T2 Debug" "..\..\Bin\T2ATM_Debug\DevCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 T2 DebugCLEAN" "NetCtrl - Win32 T2 DebugCLEAN" "NTLCPDLL - Win32 T2 DebugCLEAN" "ProCtrl - Win32 T2 DebugCLEAN" "VolCtrl - Win32 T2 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DevApl.obj"
	-@erase "$(INTDIR)\DevCmn.obj"
	-@erase "$(INTDIR)\DevCsh.obj"
	-@erase "$(INTDIR)\DevCtrl.obj"
	-@erase "$(INTDIR)\DevCtrl.pch"
	-@erase "$(INTDIR)\DevCtrl.res"
	-@erase "$(INTDIR)\DevEMV.obj"
	-@erase "$(INTDIR)\DevEventQue.obj"
	-@erase "$(INTDIR)\DevFinanceIC.obj"
	-@erase "$(INTDIR)\DevKCash.obj"
	-@erase "$(INTDIR)\DevNet.obj"
	-@erase "$(INTDIR)\DevRDF.obj"
	-@erase "$(INTDIR)\DevScr.obj"
	-@erase "$(INTDIR)\DevSeed.obj"
	-@erase "$(INTDIR)\DevSnd.obj"
	-@erase "$(INTDIR)\DevThread.obj"
	-@erase "$(INTDIR)\DevUtil.obj"
	-@erase "$(INTDIR)\EventQue.obj"
	-@erase "$(INTDIR)\nhmwi.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DevCtrl.exp"
	-@erase "$(OUTDIR)\DevCtrl.lib"
	-@erase "$(OUTDIR)\DevCtrl.pdb"
	-@erase "..\..\Bin\T2ATM_Debug\DevCtrl.dll"
	-@erase "..\..\Bin\T2ATM_Debug\DevCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\DevCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DevCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\DevCtrl.pdb" /debug /machine:I386 /def:".\DevCtrl.def" /out:"../../Bin/T2ATM_Debug/DevCtrl.dll" /implib:"$(OUTDIR)\DevCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\DevCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DevApl.obj" \
	"$(INTDIR)\DevCmn.obj" \
	"$(INTDIR)\DevCsh.obj" \
	"$(INTDIR)\DevCtrl.obj" \
	"$(INTDIR)\DevEMV.obj" \
	"$(INTDIR)\DevEventQue.obj" \
	"$(INTDIR)\DevFinanceIC.obj" \
	"$(INTDIR)\DevKCash.obj" \
	"$(INTDIR)\DevNet.obj" \
	"$(INTDIR)\DevRDF.obj" \
	"$(INTDIR)\DevScr.obj" \
	"$(INTDIR)\DevSeed.obj" \
	"$(INTDIR)\DevSnd.obj" \
	"$(INTDIR)\DevThread.obj" \
	"$(INTDIR)\DevUtil.obj" \
	"$(INTDIR)\EventQue.obj" \
	"$(INTDIR)\nhmwi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DevCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib" \
	"$(OUTDIR)\NetCtrl.lib" \
	"$(OUTDIR)\NTLcpDLL.lib" \
	"$(OUTDIR)\ProCtrl.lib" \
	"$(OUTDIR)\VolCtrl.lib"

"..\..\Bin\T2ATM_Debug\DevCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("DevCtrl.dep")
!INCLUDE "DevCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "DevCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "DevCtrl - Win32 Release" || "$(CFG)" == "DevCtrl - Win32 Debug" || "$(CFG)" == "DevCtrl - Win32 T2 Release" || "$(CFG)" == "DevCtrl - Win32 T2 Debug"
SOURCE=.\DevApl.cpp

"$(INTDIR)\DevApl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevCmn.cpp

"$(INTDIR)\DevCmn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevCsh.cpp

"$(INTDIR)\DevCsh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevCtrl.cpp

"$(INTDIR)\DevCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevCtrl.rc

"$(INTDIR)\DevCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DevEMV.cpp

"$(INTDIR)\DevEMV.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevEventQue.cpp

"$(INTDIR)\DevEventQue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevFinanceIC.cpp

"$(INTDIR)\DevFinanceIC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevKCash.cpp

"$(INTDIR)\DevKCash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevNet.cpp

"$(INTDIR)\DevNet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevRDF.cpp

"$(INTDIR)\DevRDF.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevScr.cpp

"$(INTDIR)\DevScr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevSeed.cpp

"$(INTDIR)\DevSeed.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevSnd.cpp

"$(INTDIR)\DevSnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevThread.cpp

"$(INTDIR)\DevThread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\DevUtil.cpp

"$(INTDIR)\DevUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=.\EventQue.cpp

"$(INTDIR)\EventQue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"


SOURCE=..\..\WinAtm\nhmwi.cpp

"$(INTDIR)\nhmwi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DevCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\DevCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\DevCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\DevCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\DevCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\DevCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

"LcpOpen - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Release" 
   cd "..\..\DevCtrl"

"LcpOpen - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

"LcpOpen - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Debug" 
   cd "..\..\DevCtrl"

"LcpOpen - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

"LcpOpen - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Release" 
   cd "..\..\DevCtrl"

"LcpOpen - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

"LcpOpen - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Debug" 
   cd "..\..\DevCtrl"

"LcpOpen - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

"NetCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Release" 
   cd "..\DevCtrl"

"NetCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

"NetCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Debug" 
   cd "..\DevCtrl"

"NetCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

"NetCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Release" 
   cd "..\DevCtrl"

"NetCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

"NetCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Debug" 
   cd "..\DevCtrl"

"NetCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\NetCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\NetCtrl.mak CFG="NetCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

"NTLCPDLL - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 Release" 
   cd "..\..\DevCtrl"

"NTLCPDLL - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

"NTLCPDLL - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 Debug" 
   cd "..\..\DevCtrl"

"NTLCPDLL - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

"NTLCPDLL - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 T2 Release" 
   cd "..\..\DevCtrl"

"NTLCPDLL - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

"NTLCPDLL - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 T2 Debug" 
   cd "..\..\DevCtrl"

"NTLCPDLL - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpDll"
   $(MAKE) /$(MAKEFLAGS) /F .\NtLcpDll.mak CFG="NTLCPDLL - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\DevCtrl"

!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

"ProCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" 
   cd "..\DevCtrl"

"ProCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

"ProCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" 
   cd "..\DevCtrl"

"ProCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

"ProCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" 
   cd "..\DevCtrl"

"ProCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

"ProCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" 
   cd "..\DevCtrl"

"ProCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\ProCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\ProCtrl.mak CFG="ProCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

!ENDIF 

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

"VolCtrl - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 Release" 
   cd "..\DevCtrl"

"VolCtrl - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

"VolCtrl - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 Debug" 
   cd "..\DevCtrl"

"VolCtrl - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Release"

"VolCtrl - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 T2 Release" 
   cd "..\DevCtrl"

"VolCtrl - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 T2 Debug"

"VolCtrl - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 T2 Debug" 
   cd "..\DevCtrl"

"VolCtrl - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\VolCtrl"
   $(MAKE) /$(MAKEFLAGS) /F .\VolCtrl.mak CFG="VolCtrl - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\DevCtrl"

!ENDIF 


!ENDIF 

