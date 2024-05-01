file(GLOB strategy_src   ./*.c)
file(GLOB strategy_hdr   ./*.h)

set_source_files_properties(${strategy_src} PROPERTIES LANGUAGE C)

add_executable(1_strategy_c
    ${strategy_src}
    ${strategy_hdr}
    )

target_include_directories(1_strategy_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(1_strategy_c_test
    test_main.cpp
    test.cpp
    )

target_link_libraries(1_strategy_c_test PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)

install(TARGETS 1_strategy_c DESTINATION bin)
