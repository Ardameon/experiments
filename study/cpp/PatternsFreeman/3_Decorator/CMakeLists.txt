add_executable(3_decorator main.c decorator.h)

target_include_directories(3_decorator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 3_decorator DESTINATION bin)
