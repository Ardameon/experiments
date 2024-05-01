#include "CppUTest/TestHarness.h"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
	#include <stdio.h>
    #include "CppUTest/TestHarness_c.h"
}

TEST_GROUP(FirstTestGroup)
{
    void setup()
    {
    	printf("\nTestSetup!");
    }

    void teardown()
    {
    	printf("\nTestTearDown!");
    }
};

//TEST(FirstTestGroup, ThirdTest)
//{
//    STRCMP_EQUAL("hello", "world");
//}
//
//TEST(FirstTestGroup, SecondTest)
//{
//   FAIL("Fail me!");
//}

TEST(FirstTestGroup, FirstTest)
{
}

