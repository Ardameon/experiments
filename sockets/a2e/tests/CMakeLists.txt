list(APPEND app_src_common
    ${PROJECT_SOURCE_DIR}/src/a2e_msg.c
    ${PROJECT_SOURCE_DIR}/src/a2e_common.c
    ${PROJECT_SOURCE_DIR}/src/a2e_dbg.c
    )

list(APPEND test_src_common
    test_main.cpp
    test_a2e_msg.cpp
    test_a2e_dbg.cpp
    test_a2e_common.cpp
    )

list(APPEND app_src_a2e
    ${PROJECT_SOURCE_DIR}/src/a2e_msg.c
    ${PROJECT_SOURCE_DIR}/src/a2e_common.c
    ${PROJECT_SOURCE_DIR}/src/a2e.c
    )

list(APPEND test_src_a2e
    test_main.cpp
	test_a2e.cpp
    )

list(APPEND app_src_a2e_client
    ${PROJECT_SOURCE_DIR}/src/a2e_msg.c
    ${PROJECT_SOURCE_DIR}/src/a2e_common.c
    ${PROJECT_SOURCE_DIR}/src/a2e_dbg.c
    )

list(APPEND test_src_a2e_client
    test_main.cpp
	test_a2e_client.cpp
    )

set(CMAKE_BUILD_TYPE Debug)

file(GLOB a2e_hdrs RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SOURCE_DIR}/include/*.h)

#foreach(filename ${a2e_hdrs})
#    message(${filename})
#endforeach()

set_source_files_properties(
    ${app_src_common}
    ${app_src_a2e}
    ${test_src_common}
    ${test_src_a2e}
    ${test_src_a2e_client}
    PROPERTIES LANGUAGE CXX)

add_subdirectory(cpputest)

add_executable(test_common EXCLUDE_FROM_ALL ${app_src_common} ${test_src_common} ${a2e_hdrs})
add_executable(test_a2e EXCLUDE_FROM_ALL ${app_src_a2e} ${test_src_a2e} ${a2e_hdrs})
add_executable(test_a2e_client EXCLUDE_FROM_ALL ${app_src_a2e_client} ${test_src_a2e_client} ${a2e_hdrs})

foreach(TEST_TARGET test_common test_a2e test_a2e_client)
    target_include_directories(${TEST_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${TEST_TARGET} PRIVATE CppUTest::CppUTest CppUTest::CppUTestExt)
    target_compile_options(${TEST_TARGET} PRIVATE -DA2E_UTEST -DA2E_UTEST_MEM_MOCK --coverage)
    target_link_options(${TEST_TARGET} PRIVATE -DA2E_UTEST --coverage)
endforeach()

# Custom target to build all tests targets
add_custom_target(tests DEPENDS test_common test_a2e test_a2e_client COMMENT "Build all tests")

# to build all tests:
#
# make tests

# Enable CTest driver support
enable_testing()

set(TEST_RUN_PARAM -c -v)

add_test(NAME A2E_test_common COMMAND test_common ${TEST_RUN_PARAM})
add_test(NAME A2E_test_a2e COMMAND test_a2e ${TEST_RUN_PARAM})
add_test(NAME A2E_test_a2e_client COMMAND test_a2e_client ${TEST_RUN_PARAM})

# To start tests invoke ctest after building in build-dir:
#
# ctest [--verbose] [--output-on-failure]

# To create coverage report invoke ctest -T Coverage after tests run
#
# ctest -T Coverage


