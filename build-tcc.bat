rem need to check if we are in the right folder
rem
del /s /q tcc
cd tinycc\win32
call build-tcc.bat -clean
call build-tcc.bat -c cl -t 32 -i ..\..\tcc
cd ..\..
cd tcc
rem see tinycc\win32\tcc-win32.txt
lib /def:libtcc\libtcc.def /out:libtcc.lib
cd ..
