# How to build against libtcc

We want this to work on Windows, Linux, and macOS. How?

## Notes on Windows

built tinycc...

```
git clone https://repo.or.cz/tinycc.git
mkdir tcc
cd tinycc\win32
build-tcc.bat -c cl -i ..\..\tcc
cd ..\..
cd tcc
lib /def:libtcc\libtcc.def /out:libtcc.lib
```


This seems to build tcc for x86. It seems that we have to build everything for x86 as a result.


```
cd build
cmake .. -G "Visual Studio 16 2019" Win32
cmake --build .
```

Now, libtcc.dll needs to be in a place that our program can find it.

```
cd tcc
..\build\Debug\clavm.exe
```

```
cd build
cmake .. -G "Visual Studio 16 2019" -DCMAKE_GENERATOR_PLATFORM=WIN32 
cmake --build .
```
