list(APPEND test_src
    ${PROJECT_SOURCE_DIR}/src/a2e_msg.c
    ${PROJECT_SOURCE_DIR}/src/a2e_dbg.c
    )

list(APPEND test_src
    tests_main.cpp
    a2e_msg_test.cpp
    a2e_dbg_test.cpp
    )

file(GLOB a2e_hdrs RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SOURCE_DIR}/include/*.h)

#foreach(filename ${a2e_hdrs})
#    message(${filename})
#endforeach()

set_source_files_properties(${test_src} PROPERTIES LANGUAGE CXX)

add_subdirectory(cpputest)

add_executable(tests EXCLUDE_FROM_ALL ${test_src} ${a2e_hdrs})

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
