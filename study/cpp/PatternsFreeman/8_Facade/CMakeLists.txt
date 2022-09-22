add_executable(8_facade main.cpp facade.h subsystem.h)

target_include_directories(8_facade PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 8_facade DESTINATION bin)
