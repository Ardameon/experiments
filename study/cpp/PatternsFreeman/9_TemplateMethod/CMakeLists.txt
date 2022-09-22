add_executable(9_template_method main.cpp template_method.h)

target_include_directories(9_template_method PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 9_template_method DESTINATION bin)
