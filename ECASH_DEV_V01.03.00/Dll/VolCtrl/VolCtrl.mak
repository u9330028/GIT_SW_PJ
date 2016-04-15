# Microsoft Developer Studio Generated NMAKE File, Based on VolCtrl.dsp
!IF "$(CFG)" == ""
CFG=VolCtrl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VolCtrl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VolCtrl - Win32 Release" && "$(CFG)" != "VolCtrl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VolCtrl.mak" CFG="VolCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VolCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VolCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VolCtrl - Win32 Release"

OUTDIR=\Chb\LibRelease
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\..\Chb\ExeRelease\VolCtrl.dll"

!ELSE 

ALL : "Common - Win32 Release" "..\..\..\..\..\..\Chb\ExeRelease\VolCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Common - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VolCtrl.obj"
	-@erase "$(INTDIR)\VolCtrl.pch"
	-@erase "$(INTDIR)\VolCtrl.res"
	-@erase "$(INTDIR)\Volume.obj"
	-@erase "$(OUTDIR)\VolCtrl.exp"
	-@erase "$(OUTDIR)\VolCtrl.lib"
	-@erase "..\..\..\..\..\..\Chb\ExeRelease\VolCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\VolCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\VolCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VolCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Common.lib Winmm.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\VolCtrl.pdb" /machine:I386 /def:".\VolCtrl.def" /out:"\Chb\ExeRelease/VolCtrl.dll" /implib:"$(OUTDIR)\VolCtrl.lib" 
DEF_FILE= \
	".\VolCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VolCtrl.obj" \
	"$(INTDIR)\Volume.obj" \
	"$(INTDIR)\VolCtrl.res" \
	"$(OUTDIR)\Common.lib"

"..\..\..\..\..\..\Chb\ExeRelease\VolCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VolCtrl - Win32 Debug"

OUTDIR=\Chb\LibDebug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\..\Chb\ExeDebug\VolCtrl.dll"

!ELSE 

ALL : "Common - Win32 Debug" "..\..\..\..\..\..\Chb\ExeDebug\VolCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Common - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VolCtrl.obj"
	-@erase "$(INTDIR)\VolCtrl.pch"
	-@erase "$(INTDIR)\VolCtrl.res"
	-@erase "$(INTDIR)\Volume.obj"
	-@erase "$(OUTDIR)\VolCtrl.exp"
	-@erase "$(OUTDIR)\VolCtrl.lib"
	-@erase "$(OUTDIR)\VolCtrl.pdb"
	-@erase "..\..\..\..\..\..\Chb\ExeDebug\VolCtrl.dll"
	-@erase "..\..\..\..\..\..\Chb\ExeDebug\VolCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\VolCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\VolCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VolCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Common.lib Winmm.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\VolCtrl.pdb" /debug /machine:I386 /def:".\VolCtrl.def" /out:"\Chb\ExeDebug/VolCtrl.dll" /implib:"$(OUTDIR)\VolCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\VolCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VolCtrl.obj" \
	"$(INTDIR)\Volume.obj" \
	"$(INTDIR)\VolCtrl.res" \
	"$(OUTDIR)\Common.lib"

"..\..\..\..\..\..\Chb\ExeDebug\VolCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VolCtrl.dep")
!INCLUDE "VolCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "VolCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VolCtrl - Win32 Release" || "$(CFG)" == "VolCtrl - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VolCtrl - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\VolCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VolCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VolCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\VolCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VolCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VolCtrl.cpp

"$(INTDIR)\VolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VolCtrl.pch"


SOURCE=.\VolCtrl.rc

"$(INTDIR)\VolCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Volume.cpp

"$(INTDIR)\Volume.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VolCtrl.pch"


!IF  "$(CFG)" == "VolCtrl - Win32 Release"

"Common - Win32 Release" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Release" 
   cd "..\VolCtrl"

"Common - Win32 ReleaseCLEAN" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Release" RECURSE=1 CLEAN 
   cd "..\VolCtrl"

!ELSEIF  "$(CFG)" == "VolCtrl - Win32 Debug"

"Common - Win32 Debug" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Debug" 
   cd "..\VolCtrl"

"Common - Win32 DebugCLEAN" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\VolCtrl"

!ENDIF 


!ENDIF 

