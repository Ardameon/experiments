cmake_minimum_required(VERSION 3.10)

project(RunningString VERSION 1.0.0 LANGUAGES C)

message(STATUS "This is TEST warning message")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(BUILD_RELEASE no)

message(STATUS "Build RELEASE: ${BUILD_RELEASE}")

if(BUILD_RELEASE)
    message(STATUS "Build type: Release")
    #set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE MinSizeRel)
else()
    message(STATUS "Build type: Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(running_string main.c letters.h running_str.c running_str.h)
target_include_directories(running_string PRIVATE ${RunningString_BINARY_DIR})

if (BUILD_RELEASE)
    add_custom_command(TARGET running_string POST_BUILD COMMAND strip running_string)
endif()

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

message(STATUS "Before INSTALL")

configure_file(running_str_config.h.in running_str_config.h)

install(TARGETS running_string DESTINATION bin)

message(STATUS "After INSTALL")
message(STATUS "INSTALL prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "STRIP: ${CMAKE_STRIP}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
