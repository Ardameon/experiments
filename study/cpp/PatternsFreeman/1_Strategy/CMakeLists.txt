add_subdirectory(c_imp)
add_subdirectory(array)

add_executable(1_strategy main.cpp duck.hpp)

target_include_directories(1_strategy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 1_strategy DESTINATION bin)
