cmake_minimum_required(VERSION 3.12)
project(KTPuild_real)

set(CMAKE_CXX_STANDARD 14)

add_executable(KTPuild_real Buildfile.cpp CppFile.cpp main.cpp)