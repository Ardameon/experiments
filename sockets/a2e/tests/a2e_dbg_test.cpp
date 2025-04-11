#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "a2e_dbg.h"
#include <stdio.h>

/* Maximum log string length */
#define LOG_STR_LEN_MAX 1024

/* Helper function to mock the custom log function */
static void mock_log_func(const char *log_str, int len)
{
    mock().actualCall("mock_log_func")
          .withStringParameter("log_str", log_str)
          .withIntParameter("len", len);
}

/* Test group for a2e_dbg functions */
TEST_GROUP(a2e_dbg_test_group)
{
    void setup()
    {
        /* Initialization before each test */
        a2e_set_dbg(1);  /* Enable debug mode */
        a2e_set_dbg_log_func(mock_log_func);  /* Set the custom log function */
    }

    void teardown()
    {
        /* Cleanup after each test */
        mock().clear();  /* Clear mocks */
    }
};

/* Test case 1: Check logging when debug mode is enabled */
TEST(a2e_dbg_test_group, log_when_debug_is_enabled)
{
    const char *test_message = "Test debug message";

    /* Mock printf to check the output */
    mock().expectOneCall("mock_log_func")
          .withStringParameter("log_str", test_message)
          .withIntParameter("len", strlen(test_message));

    /* Call the logging function */
    a2e_log(test_message);

    /* Verify that printf was called with the correct parameters */
    mock().checkExpectations();
}

/* Test case 2: Check logging when debug mode is disabled */
TEST(a2e_dbg_test_group, log_when_debug_is_disabled)
{
    const char *test_message = "This should not be logged";

    /* Disable debug mode */
    a2e_set_dbg(0);

    mock().expectNoCall("mock_log_func");

    /* Call the logging function */
    a2e_log(test_message);

    /* Verify that printf was not called */
    mock().checkExpectations();
}

/* Test case 3: Check custom log function */
TEST(a2e_dbg_test_group, log_with_custom_log_function)
{
    const char *test_message = "Custom log function test";
    int expected_len = strlen(test_message);

    /* Set the custom log function */
    a2e_set_dbg_log_func(mock_log_func);

    /* Expect the custom log function to be called with the correct parameters */
    mock().expectOneCall("mock_log_func")
          .withParameter("log_str", test_message)
          .withParameter("len", expected_len);

    /* Call the logging function */
    a2e_log(test_message);

    /* Verify that the mock was called */
    mock().checkExpectations();
}

/* Test case 4: Check logging with an empty format */
TEST(a2e_dbg_test_group, log_with_empty_format)
{
    /* Mock printf to check the output */
    mock().expectOneCall("mock_log_func")
          .withStringParameter("log_str", "")
          .withIntParameter("len", 0);

    /* Call the logging function with an empty format */
    a2e_log("");

    /* Verify that printf was called with an empty message */
    mock().checkExpectations();
}

/* Test case 5: Check logging with a long string */
TEST(a2e_dbg_test_group, log_with_long_string)
{
    char long_message[LOG_STR_LEN_MAX + 100];
    char long_message_modified[LOG_STR_LEN_MAX + 1];
    memset(long_message, 'A', LOG_STR_LEN_MAX + 99);
    long_message[LOG_STR_LEN_MAX + 99] = '\0';
    memcpy(long_message_modified, long_message, LOG_STR_LEN_MAX);
    long_message_modified[LOG_STR_LEN_MAX - 1] = '\0';

    /* Mock printf to check the output */
    mock().expectOneCall("mock_log_func")
          .withStringParameter("log_str", long_message_modified)
          .withIntParameter("len", strlen(long_message_modified));

    /* Call the logging function with a long string */
    a2e_log(long_message);

    /* Verify that printf was called with the correct parameters */
    mock().checkExpectations();
}
