cmake_minimum_required(VERSION 3.17)
project(LibraryLinker)

set(CMAKE_CXX_STANDARD 20)

add_executable(LibraryLinker main.cpp)
target_link_libraries(LibraryLinker ${CMAKE_SOURCE_DIR}/../StringReplacer/cmake-build-debug/libstring_replacer.a)