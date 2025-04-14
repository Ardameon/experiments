#include "CppUTest/TestHarness.h"
#include "a2e_msg.h"

/* Test group for a2e_msg functions */
TEST_GROUP(a2e_message_test_group)
{
    a2e_msg_t msg;

    /* Setup function: Initialize the message structure before each test */
    void setup()
    {
        memset(&msg, 0, sizeof(a2e_msg_t));  /* Clear the message structure */
    }

    /* Teardown function: No cleanup needed in this case */
    void teardown()
    {
        /* Nothing to clean up */
    }
};

/* Test case 1: Test filling a request message */
TEST(a2e_message_test_group, fill_request_message_test)
{
    uint32_t expected_len = 100;

    /* Call the function to fill the request message */
    a2e_msg_fill_req(&msg, expected_len);

    /* Verify the message fields */
    CHECK_EQUAL(eA2E_MSG_REQEUST, msg.type);  /* Check the message type */
    CHECK_EQUAL(A2E_MSG_MAGIC, msg.magic);    /* Check the magic number */
    CHECK_EQUAL(expected_len, msg.len);       /* Check the length */
}

/* Test case 2: Test filling a response message */
TEST(a2e_message_test_group, fill_response_message_test)
{
    uint32_t expected_len = 200;

    /* Call the function to fill the response message */
    a2e_msg_fill_rsp(&msg, expected_len);

    /* Verify the message fields */
    CHECK_EQUAL(eA2E_MSG_RESPONSE, msg.type);  /* Check the message type */
    CHECK_EQUAL(A2E_MSG_MAGIC, msg.magic);     /* Check the magic number */
    CHECK_EQUAL(expected_len, msg.len);        /* Check the length */
}

/* Test case 3: Test filling a progress message */
TEST(a2e_message_test_group, fill_progress_message_test)
{
    /* Call the function to fill the progress message */
    a2e_msg_fill_prg(&msg);

    /* Verify the message fields */
    CHECK_EQUAL(eA2E_MSG_PROGRESS, msg.type);  /* Check the message type */
    CHECK_EQUAL(A2E_MSG_MAGIC, msg.magic);     /* Check the magic number */
    CHECK_EQUAL(0, msg.len);                   /* Check the length (should be 0 for progress) */
}

/* Test case 4: Test filling a message with a NULL pointer */
TEST(a2e_message_test_group, fill_message_with_null_pointer_test)
{
    /* Call the functions with a NULL pointer */
    a2e_msg_fill_req(NULL, 100);
    a2e_msg_fill_rsp(NULL, 200);
    a2e_msg_fill_prg(NULL);

    /* No assertions needed; the test passes if no crash occurs */
}
