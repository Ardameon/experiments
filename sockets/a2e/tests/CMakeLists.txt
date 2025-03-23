list(APPEND test_src
    ${CMAKE_CURRENT_LIST_DIR}/../src/a2e_msg.c
    ${CMAKE_CURRENT_LIST_DIR}/../src/a2e_dbg.c
    )

list(APPEND test_src
    tests_main.cpp
    a2e_msg_test.cpp
    a2e_dbg_test.cpp
    )

file(GLOB a2e_hdrs ${CMAKE_CURRENT_LIST_DIR}/../include/*.h)

set_source_files_properties(${test_src} PROPERTIES LANGUAGE CXX)

add_subdirectory(cpputest)

add_executable(tests EXCLUDE_FROM_ALL ${test_src})

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(tests PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
