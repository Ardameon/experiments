#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "a2e_common.h"
#include "a2e_def.h"
#include "a2e_dbg.h"

/* Helper function to mock the custom log function */
static void mock_log_func(const char *log_str, int len)
{
    mock().actualCall("mock_log_func")
          .withStringParameter("log_str", log_str)
          .withIntParameter("len", len);
}

/* Test group for a2e_common module tests */
TEST_GROUP(a2e_common_tests)
{
    a2e_t a2e;
    a2e_cfg_t cfg;

    /* Setup before each test */
    void setup()
    {
        memset(&a2e, 0, sizeof(a2e));
        memset(&cfg, 0, sizeof(cfg));

        a2e_set_dbg(1);  /* Enable debug mode */
        a2e_set_dbg_log_func(mock_log_func);  /* Set the custom log function */
    }

    /* Cleanup after each test */
    void teardown()
    {
        mock().clear();
    }
};

/* Test that a2e_get_timestamp returns non-zero value */
TEST(a2e_common_tests, a2e_get_timestamp_returns_non_zero_value)
{
    unsigned long timestamp = a2e_get_timestamp();
    CHECK_TRUE(timestamp > 0);
}

/* Test that a2e_timeout_reached returns true when timeout is reached */
TEST(a2e_common_tests, a2e_timeout_reached_returns_true_when_timeout_reached)
{
    unsigned long start = a2e_get_timestamp();
    unsigned timeout_ms = 1; /* 1ms timeout */

    /* Wait for timeout to pass */
    while (!a2e_timeout_reached(start, timeout_ms)) {}

    CHECK_TRUE(a2e_timeout_reached(start, timeout_ms));
}

/* Test that a2e_timeout_reached returns false when timeout is not reached */
TEST(a2e_common_tests, a2e_timeout_reached_returns_false_when_timeout_not_reached)
{
    unsigned long start = a2e_get_timestamp();
    unsigned timeout_ms = 100; /* 100ms timeout */

    CHECK_FALSE(a2e_timeout_reached(start, timeout_ms));
}

/* Test that a2e_set_state changes state and logs when state differs */
TEST(a2e_common_tests, a2e_set_state_changes_state_and_logs_when_state_differs)
{
    a2e.state = eA2E_STATE_IDLE;
    mock().expectOneCall("mock_log_func").ignoreOtherParameters();

    a2e_set_state(&a2e, eA2E_STATE_REQ_RX);

    LONGS_EQUAL(eA2E_STATE_REQ_RX, a2e.state);
    mock().checkExpectations();
}

/* Test that a2e_set_state does nothing when new state is same as current */
TEST(a2e_common_tests, a2e_set_state_does_nothing_when_state_same)
{
    a2e.state = eA2E_STATE_IDLE;
    mock().expectNoCall("mock_log_func");

    a2e_set_state(&a2e, eA2E_STATE_IDLE);

    LONGS_EQUAL(eA2E_STATE_IDLE, a2e.state);
    mock().checkExpectations();
}

/* Test that a2e_state_str returns correct string for each state */
TEST(a2e_common_tests, a2e_state_str_returns_correct_string_for_each_state)
{
    STRCMP_EQUAL("NULL", a2e_state_str(eA2E_STATE_NULL));
    STRCMP_EQUAL("IDLE", a2e_state_str(eA2E_STATE_IDLE));
    STRCMP_EQUAL("REQ_RX_START", a2e_state_str(eA2E_STATE_REQ_RX_START));
    STRCMP_EQUAL("REQ_RX", a2e_state_str(eA2E_STATE_REQ_RX));
    STRCMP_EQUAL("REQ_RX_FINISH", a2e_state_str(eA2E_STATE_REQ_RX_FINISH));
    STRCMP_EQUAL("REQ_TX_START", a2e_state_str(eA2E_STATE_REQ_TX_START));
    STRCMP_EQUAL("REQ_TX", a2e_state_str(eA2E_STATE_REQ_TX));
    STRCMP_EQUAL("REQ_TX_FINISH", a2e_state_str(eA2E_STATE_REQ_TX_FINISH));
    STRCMP_EQUAL("REQ_SENT", a2e_state_str(eA2E_STATE_REQ_SENT));
    STRCMP_EQUAL("REQ_PROGRESS", a2e_state_str(eA2E_STATE_REQ_PROGRESS));
    STRCMP_EQUAL("REQ_COMPLT", a2e_state_str(eA2E_STATE_REQ_COMPLT));
    STRCMP_EQUAL("RSP_RX", a2e_state_str(eA2E_STATE_RSP_RX));
    STRCMP_EQUAL("RSP_RX_FINISH", a2e_state_str(eA2E_STATE_RSP_RX_FINISH));
    STRCMP_EQUAL("RSP_TX", a2e_state_str(eA2E_STATE_RSP_TX));
    STRCMP_EQUAL("RSP_TX_FINISH", a2e_state_str(eA2E_STATE_RSP_TX_FINISH));
    STRCMP_EQUAL("UNDEF_STATE", a2e_state_str((a2e_state_e)999)); /* Invalid state */
}

/* Test that a2e_name returns configured name when a2e is valid */
TEST(a2e_common_tests, a2e_name_returns_cfg_name_when_a2e_valid)
{
    strncpy(a2e.cfg.name, "test_name", A2E_NAME_LEN_MAX);
    STRCMP_EQUAL("test_name", a2e_name(&a2e));
}

/* Test that a2e_name returns "nullptr" when a2e is NULL */
TEST(a2e_common_tests, a2e_name_returns_nullptr_when_a2e_null)
{
    STRCMP_EQUAL("nullptr", a2e_name(NULL));
}

/* Test that a2e_cfg_apply copies config and sets default chunk size when zero */
TEST(a2e_common_tests, a2e_cfg_apply_copies_config_and_sets_default_chunk_size)
{
    strncpy(cfg.name, "test", A2E_NAME_LEN_MAX);
    strncpy(cfg.sock_dir, "/test_dir", A2E_SOCK_DIR_LEN_MAX);
    cfg.rw_chunk_size = 0; /* Should use default */

    a2e_cfg_apply(&a2e, &cfg);

    STRCMP_EQUAL("test", a2e.cfg.name);
    STRCMP_EQUAL("/test_dir", a2e.cfg.sock_dir);
    LONGS_EQUAL(DEF_A2E_RW_CHUNK_SIZE, a2e.cfg.rw_chunk_size);
}

/* Test that a2e_cfg_apply preserves custom chunk size when non-zero */
TEST(a2e_common_tests, a2e_cfg_apply_preserves_custom_chunk_size)
{
    cfg.rw_chunk_size = 12345;

    a2e_cfg_apply(&a2e, &cfg);

    LONGS_EQUAL(12345, a2e.cfg.rw_chunk_size);
}
