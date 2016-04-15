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
del /S *.bak
del /S *.opt

rem Exclude library file and project option file.
rem del /S *.lib
rmdir /S /Q .\Lib\T1ATM_Debug
rmdir /S /Q .\Lib\T1ATM_Release
rmdir /S /Q .\Lib\T2ATM_Debug
rmdir /S /Q .\Lib\T2ATM_Release
del /S *.dll
del /S *.ocx
del /S *.vcb
rem del /S *.opt

rem Delete ActiveX temporary file.
del /S *.map
del /S *.oca
del /S *.trg
del /S *.tlb
