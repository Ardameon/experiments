add_executable(2_observer main.c observer.h)

target_include_directories(2_observer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 2_observer DESTINATION bin)
