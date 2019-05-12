cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(tools LANGUAGES C)
message("start to compile audio parse tools")

set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB COMPILE_FILES "${SRC_ROOT_PATH}/*.c")

set(SOURCE_FILES ${COMPILE_FILES})
include_directories(${SRC_ROOT_PATH})
add_executable(tools ${SOURCE_FILES})
