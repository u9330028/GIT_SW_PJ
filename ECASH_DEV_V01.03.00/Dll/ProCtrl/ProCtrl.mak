# Microsoft Developer Studio Generated NMAKE File, Based on ProCtrl.dsp
!IF "$(CFG)" == ""
CFG=ProCtrl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ProCtrl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ProCtrl - Win32 Release" && "$(CFG)" != "ProCtrl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProCtrl.mak" CFG="ProCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ProCtrl - Win32 Release"

OUTDIR=\Chb\LibRelease
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\..\Chb\ExeRelease\ProCtrl.dll"

!ELSE 

ALL : "Common - Win32 Release" "..\..\..\..\..\..\Chb\ExeRelease\ProCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Common - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ErrStack.obj"
	-@erase "$(INTDIR)\ErrSum.obj"
	-@erase "$(INTDIR)\ProCtrl.obj"
	-@erase "$(INTDIR)\ProCtrl.pch"
	-@erase "$(INTDIR)\ProCtrl.res"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Total.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ProCtrl.exp"
	-@erase "$(OUTDIR)\ProCtrl.lib"
	-@erase "..\..\..\..\..\..\Chb\ExeRelease\ProCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ProCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ProCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ProCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Common.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ProCtrl.pdb" /machine:I386 /def:".\ProCtrl.def" /out:"\Chb\ExeRelease/ProCtrl.dll" /implib:"$(OUTDIR)\ProCtrl.lib" 
DEF_FILE= \
	".\ProCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\ErrStack.obj" \
	"$(INTDIR)\ErrSum.obj" \
	"$(INTDIR)\ProCtrl.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Total.obj" \
	"$(INTDIR)\ProCtrl.res" \
	"$(OUTDIR)\Common.lib"

"..\..\..\..\..\..\Chb\ExeRelease\ProCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ProCtrl - Win32 Debug"

OUTDIR=\Chb\LibDebug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\..\Chb\ExeDebug\ProCtrl.dll"

!ELSE 

ALL : "Common - Win32 Debug" "..\..\..\..\..\..\Chb\ExeDebug\ProCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Common - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ErrStack.obj"
	-@erase "$(INTDIR)\ErrSum.obj"
	-@erase "$(INTDIR)\ProCtrl.obj"
	-@erase "$(INTDIR)\ProCtrl.pch"
	-@erase "$(INTDIR)\ProCtrl.res"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Total.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ProCtrl.exp"
	-@erase "$(OUTDIR)\ProCtrl.lib"
	-@erase "$(OUTDIR)\ProCtrl.pdb"
	-@erase "..\..\..\..\..\..\Chb\ExeDebug\ProCtrl.dll"
	-@erase "..\..\..\..\..\..\Chb\ExeDebug\ProCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ProCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ProCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ProCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Common.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ProCtrl.pdb" /debug /machine:I386 /def:".\ProCtrl.def" /out:"\Chb\ExeDebug/ProCtrl.dll" /implib:"$(OUTDIR)\ProCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\ProCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\ErrStack.obj" \
	"$(INTDIR)\ErrSum.obj" \
	"$(INTDIR)\ProCtrl.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Total.obj" \
	"$(INTDIR)\ProCtrl.res" \
	"$(OUTDIR)\Common.lib"

"..\..\..\..\..\..\Chb\ExeDebug\ProCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ProCtrl.dep")
!INCLUDE "ProCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "ProCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ProCtrl - Win32 Release" || "$(CFG)" == "ProCtrl - Win32 Debug"
SOURCE=.\ErrStack.cpp

"$(INTDIR)\ErrStack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProCtrl.pch"


SOURCE=.\ErrSum.cpp

"$(INTDIR)\ErrSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProCtrl.pch"


SOURCE=.\ProCtrl.cpp

"$(INTDIR)\ProCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProCtrl.pch"


SOURCE=.\ProCtrl.rc

"$(INTDIR)\ProCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Profile.cpp

"$(INTDIR)\Profile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProCtrl.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ProCtrl - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ProCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ProCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ProCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ProCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ProCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Total.cpp

"$(INTDIR)\Total.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProCtrl.pch"


!IF  "$(CFG)" == "ProCtrl - Win32 Release"

"Common - Win32 Release" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Release" 
   cd "..\ProCtrl"

"Common - Win32 ReleaseCLEAN" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ProCtrl"

!ELSEIF  "$(CFG)" == "ProCtrl - Win32 Debug"

"Common - Win32 Debug" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Debug" 
   cd "..\ProCtrl"

"Common - Win32 DebugCLEAN" : 
   cd "\User\Windows\Chb\Src\DLL\Common"
   $(MAKE) /$(MAKEFLAGS) /F .\Common.mak CFG="Common - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ProCtrl"

!ENDIF 


!ENDIF 

