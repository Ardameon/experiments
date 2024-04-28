add_executable(1_strategy_c
    main.c
    duck.c
    mellor_duck.c
    rocket_duck.c
    bath_duck.c
    fly_behavior.c
    quak_behavior.c
    duck.h
    mellor_duck.h
    rocket_duck.h
    bath_duck.h
    fly_behavior.h
    quak_behavior.h
    )

target_include_directories(1_strategy_c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS 1_strategy_c DESTINATION bin)
