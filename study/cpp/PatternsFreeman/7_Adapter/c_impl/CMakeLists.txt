add_executable(7_adapter_c main.c cadapter.c cadapter.h)

target_include_directories(7_adapter_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 7_adapter_c DESTINATION bin)
