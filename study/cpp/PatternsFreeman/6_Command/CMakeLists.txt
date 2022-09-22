add_executable(6_command main.cpp command.h)

target_include_directories(6_command PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 6_command DESTINATION bin)
