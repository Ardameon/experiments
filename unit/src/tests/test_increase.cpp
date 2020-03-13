#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
    #include <stdio.h>
    #include "increase.h"
#include "lib.h"
}

TEST_GROUP(TestIncreaseGrp)
{
    void setup()
    {

    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(TestIncreaseGrp, ThirdTest)
{
}

TEST(TestIncreaseGrp, SecondTest)
{
    int num = 1;
    int result;

//    mock().expectOneCall("mod");
    mock().expectNCalls(1, "mod");

    result = increase(num);

    mock().checkExpectations();
}

int mod(int number, int delim)
{
    mock().actualCall("mod");
    return mock().returnIntValueOrDefault(0);
}

TEST(TestIncreaseGrp, invok_increase_check_result_is_true)
{
    int num = 1;
    int result;

    mock().disable();

    result = increase(num);

    mock().enable();

    CHECK(result);
}
