cmake_minimum_required(VERSION 3.17)
project(StringReplacer)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#add_library(string_replacer STATIC replacer.h replacer.cpp)
add_library(string_replacer SHARED replacer.h replacer.cpp)