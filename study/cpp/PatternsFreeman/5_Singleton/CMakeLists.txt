add_executable(5_singleton main.cpp singleton.h)

target_include_directories(5_singleton PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 5_singleton DESTINATION bin)
