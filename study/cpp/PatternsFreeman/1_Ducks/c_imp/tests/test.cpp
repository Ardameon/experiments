#include <cstdio>
#include "CppUTest/TestHarness.h"
#include "duck.h"
#include "mellor_duck.h"
#include "rocket_duck.h"
#include "bath_duck.h"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
    #include "CppUTest/TestHarness_c.h"
}

TEST_GROUP(ducks_base)
{
    duck_t *duck;

    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(ducks_base, duck_quak_null_ptr)
{
    duck = NULL;
    duck_quak(duck);
}

TEST(ducks_base, duck_show_null_ptr)
{
    duck = NULL;
    duck_show(duck);
}

TEST(ducks_base, duck_fly_null_ptr)
{
    duck = NULL;
    duck_fly(duck);
}

TEST(ducks_base, duck_create_null_ptr)
{
    duck = NULL;
    duck_create(duck);
}

TEST(ducks_base, duck_destroy_null_ptr)
{
    duck = NULL;
    duck_destroy(duck);
}

TEST_GROUP(fly_behavior)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fly_behavior, fly_with_rocket_imp_correct)
{
    POINTERS_EQUAL(fly_with_rocket_imp.fly, &fly_with_rocket);
}

TEST(fly_behavior, fly_with_rocket_imp)
{
    fly_with_rocket_imp.fly();
}

TEST(fly_behavior, fly_with_rocket)
{
    fly_with_rocket();
}

TEST(fly_behavior, fly_no_way_imp_correct)
{
    POINTERS_EQUAL(fly_no_way_imp.fly, &fly_no_way);
}

TEST(fly_behavior, fly_no_way_imp)
{
    fly_no_way_imp.fly();
}

TEST(fly_behavior, fly_no_way)
{
    fly_no_way();
}

TEST(fly_behavior, fly_with_wings_imp_correct)
{
    POINTERS_EQUAL(fly_with_wings_imp.fly, &fly_with_wings);
}

TEST(fly_behavior, fly_with_wings_imp)
{
    fly_with_wings_imp.fly();
}

TEST(fly_behavior, fly_with_wings)
{
    fly_with_wings();
}

TEST_GROUP(quak_behavior)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(quak_behavior, silent_quak_imp_correct)
{
    POINTERS_EQUAL(silent_quak_imp.quak, &silent_quak);
}

TEST(quak_behavior, silent_quak_imp)
{
    silent_quak_imp.quak();
}

TEST(quak_behavior, silent_quak)
{
    silent_quak();
}

TEST(quak_behavior, squeek_imp_correct)
{
    POINTERS_EQUAL(squeek_imp.quak, &squeek);
}

TEST(quak_behavior, squeek_imp)
{
    squeek_imp.quak();
}

TEST(quak_behavior, squeek)
{
    squeek();
}

TEST(quak_behavior, quak_imp_correct)
{
    POINTERS_EQUAL(quak_imp.quak, &quak);
}

TEST(quak_behavior, quak_imp)
{
    quak_imp.quak();
}

TEST(quak_behavior, quak)
{
    quak();
}

