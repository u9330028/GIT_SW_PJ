# Microsoft Developer Studio Generated NMAKE File, Based on NetCtrl.dsp
!IF "$(CFG)" == ""
CFG=NetCtrl - Win32 T2 Debug
!MESSAGE No configuration specified. Defaulting to NetCtrl - Win32 T2 Debug.
!ENDIF 

!IF "$(CFG)" != "NetCtrl - Win32 Release" && "$(CFG)" != "NetCtrl - Win32 Debug" && "$(CFG)" != "NetCtrl - Win32 T2 Release" && "$(CFG)" != "NetCtrl - Win32 T2 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetCtrl.mak" CFG="NetCtrl - Win32 T2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NetCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NetCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NetCtrl - Win32 T2 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NetCtrl - Win32 T2 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "NetCtrl - Win32 Release"

OUTDIR=.\../../Lib/T1ATM_Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Release\NetCtrl.dll"

!ELSE 

ALL : "LcpOpen - Win32 Release" "..\..\Bin\T1ATM_Release\NetCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DlcCntl.obj"
	-@erase "$(INTDIR)\Hdlc.obj"
	-@erase "$(INTDIR)\Ioc.obj"
	-@erase "$(INTDIR)\NetCtrl.obj"
	-@erase "$(INTDIR)\NetCtrl.pch"
	-@erase "$(INTDIR)\NetCtrl.res"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\SocketCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Swp.obj"
	-@erase "$(INTDIR)\Tcpip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\NetCtrl.exp"
	-@erase "$(OUTDIR)\NetCtrl.lib"
	-@erase "..\..\Bin\T1ATM_Release\NetCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /I "../seed" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\NetCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NetCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\NetCtrl.pdb" /machine:I386 /def:".\NetCtrl.def" /out:"../../Bin/T1ATM_Release/NetCtrl.dll" /implib:"$(OUTDIR)\NetCtrl.lib" 
DEF_FILE= \
	".\NetCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DlcCntl.obj" \
	"$(INTDIR)\Hdlc.obj" \
	"$(INTDIR)\Ioc.obj" \
	"$(INTDIR)\NetCtrl.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\SocketCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Swp.obj" \
	"$(INTDIR)\Tcpip.obj" \
	"$(INTDIR)\NetCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib"

"..\..\Bin\T1ATM_Release\NetCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 Debug"

OUTDIR=.\../../Lib/T1ATM_Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T1ATM_Debug\NetCtrl.dll"

!ELSE 

ALL : "LcpOpen - Win32 Debug" "..\..\Bin\T1ATM_Debug\NetCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DlcCntl.obj"
	-@erase "$(INTDIR)\Hdlc.obj"
	-@erase "$(INTDIR)\Ioc.obj"
	-@erase "$(INTDIR)\NetCtrl.obj"
	-@erase "$(INTDIR)\NetCtrl.pch"
	-@erase "$(INTDIR)\NetCtrl.res"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\SocketCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Swp.obj"
	-@erase "$(INTDIR)\Tcpip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\NetCtrl.exp"
	-@erase "$(OUTDIR)\NetCtrl.lib"
	-@erase "$(OUTDIR)\NetCtrl.pdb"
	-@erase "..\..\Bin\T1ATM_Debug\NetCtrl.dll"
	-@erase "..\..\Bin\T1ATM_Debug\NetCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I "../seed" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\NetCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NetCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\NetCtrl.pdb" /debug /machine:I386 /def:".\NetCtrl.def" /out:"../../Bin/T1ATM_Debug/NetCtrl.dll" /implib:"$(OUTDIR)\NetCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\NetCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DlcCntl.obj" \
	"$(INTDIR)\Hdlc.obj" \
	"$(INTDIR)\Ioc.obj" \
	"$(INTDIR)\NetCtrl.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\SocketCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Swp.obj" \
	"$(INTDIR)\Tcpip.obj" \
	"$(INTDIR)\NetCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib"

"..\..\Bin\T1ATM_Debug\NetCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Release"

OUTDIR=.\../../Lib/T2ATM_Release
INTDIR=.\T2_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Release\NetCtrl.dll"

!ELSE 

ALL : "LcpOpen - Win32 T2 Release" "..\..\Bin\T2ATM_Release\NetCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 T2 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DlcCntl.obj"
	-@erase "$(INTDIR)\Hdlc.obj"
	-@erase "$(INTDIR)\Ioc.obj"
	-@erase "$(INTDIR)\NetCtrl.obj"
	-@erase "$(INTDIR)\NetCtrl.pch"
	-@erase "$(INTDIR)\NetCtrl.res"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\SocketCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Swp.obj"
	-@erase "$(INTDIR)\Tcpip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\NetCtrl.exp"
	-@erase "$(OUTDIR)\NetCtrl.lib"
	-@erase "..\..\Bin\T2ATM_Release\NetCtrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /O2 /I "../seed" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\NetCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NetCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\NetCtrl.pdb" /machine:I386 /def:".\NetCtrl.def" /out:"../../Bin/T2ATM_Release/NetCtrl.dll" /implib:"$(OUTDIR)\NetCtrl.lib" 
DEF_FILE= \
	".\NetCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DlcCntl.obj" \
	"$(INTDIR)\Hdlc.obj" \
	"$(INTDIR)\Ioc.obj" \
	"$(INTDIR)\NetCtrl.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\SocketCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Swp.obj" \
	"$(INTDIR)\Tcpip.obj" \
	"$(INTDIR)\NetCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib"

"..\..\Bin\T2ATM_Release\NetCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Debug"

OUTDIR=.\../../Lib/T2ATM_Debug
INTDIR=.\T2_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\T2ATM_Debug\NetCtrl.dll"

!ELSE 

ALL : "LcpOpen - Win32 T2 Debug" "..\..\Bin\T2ATM_Debug\NetCtrl.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"LcpOpen - Win32 T2 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DlcCntl.obj"
	-@erase "$(INTDIR)\Hdlc.obj"
	-@erase "$(INTDIR)\Ioc.obj"
	-@erase "$(INTDIR)\NetCtrl.obj"
	-@erase "$(INTDIR)\NetCtrl.pch"
	-@erase "$(INTDIR)\NetCtrl.res"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\SocketCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Swp.obj"
	-@erase "$(INTDIR)\Tcpip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\NetCtrl.exp"
	-@erase "$(OUTDIR)\NetCtrl.lib"
	-@erase "$(OUTDIR)\NetCtrl.pdb"
	-@erase "..\..\Bin\T2ATM_Debug\NetCtrl.dll"
	-@erase "..\..\Bin\T2ATM_Debug\NetCtrl.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I "../seed" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\NetCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NetCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\NetCtrl.pdb" /debug /machine:I386 /def:".\NetCtrl.def" /out:"../../Bin/T2ATM_Debug/NetCtrl.dll" /implib:"$(OUTDIR)\NetCtrl.lib" /pdbtype:sept 
DEF_FILE= \
	".\NetCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\DlcCntl.obj" \
	"$(INTDIR)\Hdlc.obj" \
	"$(INTDIR)\Ioc.obj" \
	"$(INTDIR)\NetCtrl.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\SocketCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Swp.obj" \
	"$(INTDIR)\Tcpip.obj" \
	"$(INTDIR)\NetCtrl.res" \
	"$(OUTDIR)\LcpOpen.lib"

"..\..\Bin\T2ATM_Debug\NetCtrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("NetCtrl.dep")
!INCLUDE "NetCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "NetCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "NetCtrl - Win32 Release" || "$(CFG)" == "NetCtrl - Win32 Debug" || "$(CFG)" == "NetCtrl - Win32 T2 Release" || "$(CFG)" == "NetCtrl - Win32 T2 Debug"
SOURCE=.\DlcCntl.cpp

"$(INTDIR)\DlcCntl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\Hdlc.cpp

"$(INTDIR)\Hdlc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\Ioc.cpp

"$(INTDIR)\Ioc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\NetCtrl.cpp

"$(INTDIR)\NetCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\NetCtrl.rc

"$(INTDIR)\NetCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\NetWork.cpp

"$(INTDIR)\NetWork.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\SocketCtrl.cpp

"$(INTDIR)\SocketCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "NetCtrl - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /I "../seed" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\NetCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "NetCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I "../seed" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T1ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\NetCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Release"

CPP_SWITCHES=/nologo /Zp1 /MD /W3 /GX /O2 /I "../seed" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\NetCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Debug"

CPP_SWITCHES=/nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I "../seed" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "T2ATM_MODE" /Fp"$(INTDIR)\NetCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\NetCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Swp.cpp

"$(INTDIR)\Swp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


SOURCE=.\Tcpip.cpp

"$(INTDIR)\Tcpip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\NetCtrl.pch"


!IF  "$(CFG)" == "NetCtrl - Win32 Release"

"LcpOpen - Win32 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Release" 
   cd "..\..\NetCtrl"

"LcpOpen - Win32 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\NetCtrl"

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 Debug"

"LcpOpen - Win32 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Debug" 
   cd "..\..\NetCtrl"

"LcpOpen - Win32 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\NetCtrl"

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Release"

"LcpOpen - Win32 T2 Release" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Release" 
   cd "..\..\NetCtrl"

"LcpOpen - Win32 T2 ReleaseCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Release" RECURSE=1 CLEAN 
   cd "..\..\NetCtrl"

!ELSEIF  "$(CFG)" == "NetCtrl - Win32 T2 Debug"

"LcpOpen - Win32 T2 Debug" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Debug" 
   cd "..\..\NetCtrl"

"LcpOpen - Win32 T2 DebugCLEAN" : 
   cd "\T1ATM\SRC\JBB\Dll\DriverDll\LcpOpen"
   $(MAKE) /$(MAKEFLAGS) /F .\LcpOpen.mak CFG="LcpOpen - Win32 T2 Debug" RECURSE=1 CLEAN 
   cd "..\..\NetCtrl"

!ENDIF 


!ENDIF 

