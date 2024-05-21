cmake_minimum_required(VERSION 3.10)

set(PACKAGE_NAME "PatternsFreeman")

project(${PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MY_VARIABLE test_value)

add_subdirectory(cpputest)

add_subdirectory(1_Strategy)
add_subdirectory(2_Observer)
add_subdirectory(3_Decorator)
add_subdirectory(4_Factory)
add_subdirectory(5_Singleton)
add_subdirectory(6_Command)
add_subdirectory(7_Adapter)
add_subdirectory(8_Facade)
add_subdirectory(9_TemplateMethod)
add_subdirectory(10_Iterator)

configure_file(patterns.h.in patterns.h)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")
endif()

install(FILES "${PROJECT_BINARY_DIR}/patterns.h" DESTINATION include)
