rem Delete C++ temporary file.
del /S *.ncb
del /S *.obj
del /S *.ilk
del /S *.pdb
del /S *.plg
del /S *.aps
del /S *.bsc
del /S *.tmp
del /S *.trc
del /S *.pch
del /S *.idb
del /S *.exp
del /S *.sbr
del /S *.res
del /S *.tlh
del /S *.tli

rem Exclude library file and project option file.
rem del /S *.lib
rmdir /S /Q .\Lib\T1ATM_Debug
rmdir /S /Q .\Lib\T1ATM_Release
rmdir /S /Q .\Lib\T2ATM_Debug
rmdir /S /Q .\Lib\T2ATM_Release
rem del /S *.dll
rem del /S *.ocx
del /S *.vcb
rem del /S *.opt

rem Delete ActiveX temporary file.
del /S *.map
del /S *.oca
del /S *.trg
del /S *.tlb

cls
rem PKZIP25 Excute ..
rem VER INFO [INPUT:ECASH_VXXXXXX_XXXX]
set  /P  VER=
ECHO VER INFO [INPUT:ECASH_VXXXXXX_XXXX]=%VER%

MKDIR C:\XX_SRC_BACKUP_Dir

PKZIP25 -ADD -DIR=CURRENT C:\XX_SRC_BACKUP_Dir\ECASH_SRC_%VER%.ZIP

rem Src Backup Excute ..
CD ..

MKDIR DEV_ECASH_BU_Zip

CD DEV_ECASH_BU_Zip

MD ECASH_V%DATE%

CD ECASH_V%DATE%

XCOPY /s /i C:\XX_SRC_BACKUP_Dir\ECASH_SRC_%VER%.ZIP

rem Complete!! 