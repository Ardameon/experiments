#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "lib.h"
}

int mod(int number, int delim)
{
    mock().actualCall(__func__).withIntParameter("number", number).withIntParameter("delim", delim);
    return mock().intReturnValue();
}


