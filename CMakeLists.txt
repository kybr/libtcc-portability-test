cmake_minimum_required(VERSION 3.12)
project(TEST)
add_executable(test test.cpp)


if(UNIX)
target_include_directories(test PUBLIC tcc/include)
target_link_directories(test PUBLIC tcc/lib)
target_link_libraries(test tcc)
target_compile_features(test PRIVATE cxx_std_14)
endif()

if(MSVC)
target_include_directories(test PUBLIC tcc/libtcc)
target_link_directories(test PUBLIC tcc)
target_link_libraries(test libtcc)
target_compile_features(test PRIVATE cxx_std_14)
endif()
