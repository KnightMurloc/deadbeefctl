cmake_minimum_required(VERSION 3.16)
project(deadbeefctl C)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})

set(CMAKE_C_STANDARD 11)

add_library(deadbeefctl SHARED library.c)
add_executable(deadbeef_ctl main.c)

