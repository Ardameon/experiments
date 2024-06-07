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

TEST(array_base, array_strategies2)
{
    array_base.size_strategy = size_array;
    array_base.size_strategy.size(&array_base);
    array_base.fill_strategy = fill_array;
    array_base.fill_strategy.fill(&array_base);
    array_base.sort_strategy = sort_array;
    array_base.sort_strategy.sort(&array_base);
    array_base.find_strategy = find_array;
    array_base.find_strategy.find(&array_base, 1);
    array_base.show_strategy = show_array;
    array_base.show_strategy.show(&array_base);
}

TEST(array_base, array_strategies)
{
    array_base.size_strategy;
    array_base.size_strategy.size;
    array_base.fill_strategy;
    array_base.fill_strategy.fill;
    array_base.show_strategy;
    array_base.show_strategy.show;
    array_base.sort_strategy;
    array_base.sort_strategy.sort;
    array_base.find_strategy;
    array_base.find_strategy.find;
}

TEST(array_base, array_t)
{
    array_size(&array_base);
    array_fill(&array_base);
    array_show(&array_base);
    array_sort(&array_base);
    array_find(&array_base, 5);
}

