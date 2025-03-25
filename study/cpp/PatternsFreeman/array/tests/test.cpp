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

TEST(array_base, fill_strategy)
{
    array_fill_random(&array_base);
    array_fill_inc_pos(&array_base);
    array_fill_inc_neg(&array_base);
    array_fill_inc_mix(&array_base);
    array_fill_dec_pos(&array_base);
    array_fill_dec_neg(&array_base);
    array_fill_dec_mix(&array_base);

    array_base.fill_strategy = fill_array_random;
    array_base.fill_strategy.fill(&array_base);
    array_base.fill_strategy = fill_array_inc_pos;
    array_base.fill_strategy = fill_array_inc_neg;
    array_base.fill_strategy = fill_array_inc_mix;
    array_base.fill_strategy = fill_array_dec_pos;
    array_base.fill_strategy = fill_array_dec_neg;
    array_base.fill_strategy = fill_array_dec_mix;
}

TEST(array_base, size_strategy)
{
    array_base.size_strategy = size_array;
    array_base.size_strategy.size(&array_base);
}

TEST(array_base, sort_strategy)
{
    array_base.sort_strategy = sort_array;
    array_base.sort_strategy.sort(&array_base);
}

TEST(array_base, find_strategy)
{
    array_base.find_strategy = find_array;
    array_base.find_strategy.find(&array_base, 1);
}

TEST(array_base, show_strategy)
{
    array_base.show_strategy = show_array;
    array_base.show_strategy.show(&array_base);
}

TEST(array_base, array_strategies)
{
    (void)array_base.size_strategy;
    (void)array_base.size_strategy.size;
    (void)array_base.fill_strategy;
    (void)array_base.fill_strategy.fill;
    (void)array_base.show_strategy;
    (void)array_base.show_strategy.show;
    (void)array_base.sort_strategy;
    (void)array_base.sort_strategy.sort;
    (void)array_base.find_strategy;
    (void)array_base.find_strategy.find;
}

TEST(array_base, array_t)
{
    array_size(&array_base);
    array_fill(&array_base);
    array_show(&array_base);
    array_sort(&array_base);
    array_find(&array_base, 5);

    (void)array_base.size;
    (void)array_base.item;
}

