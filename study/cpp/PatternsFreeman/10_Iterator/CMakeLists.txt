add_executable(10_iterator main.cpp iterator.h simplelist.cpp simplelist.h)

target_include_directories(10_iterator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 10_iterator DESTINATION bin)
