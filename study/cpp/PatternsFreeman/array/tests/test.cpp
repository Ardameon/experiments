#include <cstdio>
#include "CppUTest/TestHarness.h"
#include "array.h"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
    #include "CppUTest/TestHarness_c.h"
}

TEST_GROUP(array_base)
{
    array_t array_base;

    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(array_base, array_strategies)
{
    array_base.size_strategy;
    //array_base.fill_strategy;
}

TEST(array_base, array_t)
{
    array_size(&array_base);
    array_fill(&array_base);
    array_show(&array_base);
    array_sort(&array_base);
    array_find(&array_base, 5);
}

