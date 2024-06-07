list(APPEND test_src
        ../array.c
        ../array_size_strategy.c
        ../array_fill_strategy.c
        ../array_sort_strategy.c
        ../array_find_strategy.c
        ../array_show_strategy.c
    )

file(GLOB array_hdr ../*.h)

list(APPEND test_src test_main.cpp)

#list(REMOVE_ITEM test_src ../mellor_duck.c)
#list(APPEND test_src test_mellor_duck.cpp)

list(APPEND test_src test.cpp)

set_source_files_properties(${test_src} PROPERTIES LANGUAGE CXX)

add_executable(array_tests #EXCLUDE_FROM_ALL
    ${test_src}
    ${array_hdr}
    )

target_include_directories(array_tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/.. )

#set(test_compile_options "-O0" "-g")
#target_compile_options(array_tests PUBLIC ${test_compile_options})

target_link_libraries(array_tests PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
