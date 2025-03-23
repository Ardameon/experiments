cmake_minimum_required(VERSION 3.10)

set(PACKAGE_NAME "A2E Library")

project(${PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB a2e_hdr include/*.h)
file(GLOB a2e_src src/*.c)

set(compile_options "-Wall" "-O2" "-g")

add_library(a2e_static STATIC ${a2e_src} ${a2e_hdr})
target_include_directories(a2e_static PUBLIC include)
set_target_properties(a2e_static PROPERTIES OUTPUT_NAME a2e)

add_library(a2e_shared SHARED ${a2e_src} ${a2e_hdr})
target_include_directories(a2e_shared PUBLIC include)
set_target_properties(a2e_shared PROPERTIES OUTPUT_NAME a2e)

add_subdirectory(tests)

add_executable(server server.c)
add_executable(client client.c)

target_link_libraries(server PRIVATE a2e_static)
target_link_libraries(client PRIVATE a2e_static)

install(TARGETS server client
		DESTINATION bin)

install(TARGETS a2e_static
		ARCHIVE DESTINATION lib
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)

install(TARGETS a2e_shared
		ARCHIVE DESTINATION lib
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")
endif()

