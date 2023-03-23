del /s /q build
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -DCMAKE_GENERATOR_PLATFORM=WIN32 
cmake --build .
cd ..
