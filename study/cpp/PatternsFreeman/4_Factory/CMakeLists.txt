add_executable(4_factory main.cpp salad.h factory.h salad_shop.h components.h)

target_include_directories(4_factory PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 4_factory DESTINATION bin)
