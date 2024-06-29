cmake_minimum_required (VERSION 3.14)

project (Menu01)
#set( CMAKE_CXX_COMPILER "E:/programy/Cygwin")
set( CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED On)
set (CMAKE_CXX_FLAGS "-std=gnu++20 -Wall -Wsign-conversion -O3")
#SET(CMAKE_CXX_FLAGS  "-Wall -Werror --version")

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests unit_tests/menu01_tests.cpp src/Menu01.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_features(tests PUBLIC cxx_std_20)
   
#   include(CTest)
#include(Catch)
#catch_discover_tests(tests)