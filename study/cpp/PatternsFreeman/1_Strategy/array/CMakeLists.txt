file(GLOB array_src   ./*.c)
file(GLOB array_hdr   ./*.h)

set_source_files_properties(${array_src} PROPERTIES LANGUAGE C)

add_subdirectory(tests)

add_executable(1_array
    ${array_src}
    ${array_hdr}
    )

target_include_directories(1_array PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 1_array DESTINATION bin)
