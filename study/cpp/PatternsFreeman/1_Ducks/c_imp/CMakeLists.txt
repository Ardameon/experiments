add_executable(1_strategy_c main.c duck.h)

target_include_directories(1_strategy_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 1_strategy_c DESTINATION bin)
