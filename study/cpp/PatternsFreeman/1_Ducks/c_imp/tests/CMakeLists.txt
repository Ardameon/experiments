list(APPEND test_src
    ../bath_duck.c
    ../duck.c
    ../fly_behavior.c
    ../mellor_duck.c
    ../quak_behavior.c
    ../rocket_duck.c
    )

file(GLOB strategy_hdr ../*.h)

list(REMOVE_ITEM test_src ../main.c)
list(APPEND test_src test_main.cpp)

list(REMOVE_ITEM test_src ../mellor_duck.c)
list(APPEND test_src test_mellor_duck.cpp)

list(APPEND test_src test.cpp)

set_source_files_properties(${test_src} PROPERTIES LANGUAGE CXX)

add_executable(tests #EXCLUDE_FROM_ALL
    ${test_src}
    ${strategy_hdr}
    )

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/.. )

#set(test_compile_options "-O0" "-g")
#target_compile_options(tests PUBLIC ${test_compile_options})

target_link_libraries(tests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
