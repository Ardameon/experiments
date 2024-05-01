file(GLOB strategy_src
    ../bath_duck.c
    ../duck.c
    ../fly_behavior.c
    ../mellor_duck.c
    ../quak_behavior.c
    ../rocket_duck.c
    )
file(GLOB strategy_hdr
    bath_duck.h
    duck.h
    fly_behavior.h
    mellor_duck.h
    quak_behavior.h
    rocket_duck.h
    )

set_source_files_properties(${strategy_src} PROPERTIES LANGUAGE CXX)

add_executable(tests #EXCLUDE_FROM_ALL
    test_main.cpp
    test.cpp
    ${strategy_src}
    ${strategy_hdr}
    )

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/.. )

target_link_libraries(tests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
