add_executable(1_strategy main.cpp duck.h)

target_include_directories(1_strategy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 1_strategy DESTINATION bin)
