#include "CppUTest/TestHarness.h"

#include "../mellor_duck.c"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
    #include "CppUTest/TestHarness_c.h"
}


TEST_GROUP(mellor_ducks)
{
    duck_t *duck;

    void setup()
    {
        mellor_duck_create(&duck);
    }

    void teardown()
    {
        mellor_duck_destroy((mellor_duck_t *)duck);
    }
};

TEST(mellor_ducks, static_check)
{
    mellor_duck_show((mellor_duck_t *)duck);
}
