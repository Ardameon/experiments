add_executable(7_adapter main.cpp adapter.h)

add_subdirectory(c_impl)

target_include_directories(7_adapter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 7_adapter DESTINATION bin)
