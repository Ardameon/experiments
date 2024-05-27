add_executable(2_observer main.c observer.h)

target_include_directories(2_observer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(2_observer PUBLIC ${compile_options})

install(TARGETS 2_observer DESTINATION bin)
