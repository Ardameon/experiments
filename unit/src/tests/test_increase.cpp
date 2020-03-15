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

TEST(TestIncreaseGrp, _4_invoke_increase_check_return_is_6)
{
    int num = 1;
    int result;

    mock().expectOneCall("mod").ignoreOtherParameters().andReturnValue(0);
    
    result = increase(num);
    
    mock().checkExpectations();
    
    CHECK_EQUAL(6, result)
}

TEST(TestIncreaseGrp, _3_invoke_increase_check_return_is_11)
{
    int num = 1;
    int result;

    mock().expectOneCall("mod").ignoreOtherParameters().andReturnValue(1);
    
    result = increase(num);
    
    mock().checkExpectations();
    
    CHECK_EQUAL(11, result)
}

TEST(TestIncreaseGrp, _2_invoke_increase_check_mod_call_with_params)
{
    int num = 1;
    int result;

    mock().expectOneCall("mod").withIntParameter("number", 1).withIntParameter("delim", 2);
    
    result = increase(num);
    
    mock().checkExpectations();
}

TEST(TestIncreaseGrp, _1_invoke_increase_check_one_call_mod)
{
    int result;
    int num = 1;

    mock().expectOneCall("mod").ignoreOtherParameters();       
    
    result = increase(num);

    mock().checkExpectations();
}
