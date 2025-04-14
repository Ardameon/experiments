#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
    #include "CppUTest/TestHarness_c.h"
}

TEST_GROUP(group_name)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(group_name, test_name)
{
}
