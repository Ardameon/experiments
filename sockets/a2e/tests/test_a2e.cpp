#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_client.h"
#include "a2e_server.h"
#include "a2e_dbg.h"

typedef struct a2e_ext_t
{
    a2e_t base;
    a2e_strategy_i iface;
} a2e_ext_t;

/* Stub for function from a2e_dbg.c used in a2e_common.c just for test build */
void a2e_log(const char *format, ...)
{
    (void)format;
}

/* Stub functio for checking a2e_set_dbg_log_func */
void test_log_func(const char *log_str, int len)
{
}

int a2e_init_mock(a2e_t **a2e, const a2e_cfg_t *cfg)
{
    return mock().actualCall("a2e_init_mock").returnIntValueOrDefault(0);
}

int a2e_close_mock(a2e_t *a2e)
{
    return mock().actualCall("a2e_close_mock").returnIntValueOrDefault(0);
}

int a2e_req_rx_mock(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    return mock().actualCall("a2e_req_rx_mock").returnIntValueOrDefault(0);
}

int a2e_req_tx_mock(a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms)
{
    return mock().actualCall("a2e_req_tx_mock").returnIntValueOrDefault(0);
}

int a2e_req_compl_mock(a2e_t *a2e)
{
    return mock().actualCall("a2e_req_compl_mock").returnIntValueOrDefault(0);
}

int a2e_req_compl_wait_mock(a2e_t *a2e, uint16_t to_ms)
{
    return mock().actualCall("a2e_req_compl_wait_mock").returnIntValueOrDefault(0);
}

int a2e_resp_rx_mock(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    return mock().actualCall("a2e_resp_rx_mock").returnIntValueOrDefault(0);
}

int a2e_resp_tx_mock(a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms)
{
    return mock().actualCall("a2e_resp_tx_mock").returnIntValueOrDefault(0);
}

int a2e_prog_tx_mock(a2e_t *a2e, uint16_t to_ms)
{
    return mock().actualCall("a2e_prog_tx_mock").returnIntValueOrDefault(0);
}

void a2e_set_dbg(uint8_t on)
{
    mock().actualCall("a2e_set_dbg").withBoolParameter("on", on);
}

void a2e_set_dbg_log_func(a2e_log_func log_func)
{
    mock().actualCall("a2e_set_dbg_log_func")
          .withPointerParameter("log_func", (void *)log_func);
}

/* Mock for a2e_strategy_i interface */
static a2e_strategy_i mock_strategy;

/* Mock for a2e_client_iface_get */
a2e_strategy_i a2e_client_iface_get(void)
{
    return mock_strategy;
}

/* Mock for a2e_server_iface_get */
a2e_strategy_i a2e_server_iface_get(void)
{
    return mock_strategy;
}

TEST_GROUP(a2e_tests)
{
    a2e_t *a2e;
    a2e_cfg_t cfg;

    void setup()
    {
        a2e = (a2e_t *)malloc(sizeof(a2e_ext_t));
        memset(a2e, 0, sizeof(a2e_ext_t));
        memset(&cfg, 0, sizeof(cfg));

        /* Initialize default config */
        strncpy(cfg.name, "test", A2E_NAME_LEN_MAX);
        strncpy(cfg.sock_dir, "/tmp", A2E_SOCK_DIR_LEN_MAX);
        cfg.rw_chunk_size = 1024;

        mock_strategy =
        {
            .init = a2e_init_mock,
            .close = a2e_close_mock,
            .req_rx = a2e_req_rx_mock,
            .req_tx = a2e_req_tx_mock,
            .req_cmplt = a2e_req_compl_mock,
            .req_cmplt_wait = a2e_req_compl_wait_mock,
            .resp_rx = a2e_resp_rx_mock,
            .resp_tx = a2e_resp_tx_mock,
            .prog_tx = a2e_prog_tx_mock
        };
    }

    void teardown()
    {
        free(a2e);
        mock().clear();
    }
};

/* Test a2e_dbg_on */
TEST(a2e_tests, a2e_dbg_on)
{
    mock().expectOneCall("a2e_set_dbg").withBoolParameter("on", 1);

    a2e_dbg_on();

    mock().checkExpectations();
}

/* Test a2e_dbg_off */
TEST(a2e_tests, a2e_dbg_off)
{
    mock().expectOneCall("a2e_set_dbg").withBoolParameter("on", 0);

    a2e_dbg_off();

    mock().checkExpectations();
}

/* Test a2e_dbg_set_func */
TEST(a2e_tests, a2e_dbg_set_func)
{
    mock().expectOneCall("a2e_set_dbg_log_func").withPointerParameter("log_func", (void *)test_log_func);

    a2e_dbg_set_func(test_log_func);

    mock().checkExpectations();
}

/* Test a2e_init_client */
TEST(a2e_tests, a2e_init_client_success)
{
    mock().expectOneCall("a2e_init_mock").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_init_client(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

/* Test a2e_init_client */
TEST(a2e_tests, a2e_init_client_fail_not_implemented)
{
    mock().expectNoCall("a2e_init_mock");

    mock_strategy.init = NULL;

    a2e_status_e status = a2e_init_client(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_NOT_IMPLEMENTED, status);
    mock().checkExpectations();
}

TEST(a2e_tests, a2e_init_client_failure)
{
    mock().expectOneCall("a2e_init_mock").andReturnValue(eA2E_SC_ERROR);

    a2e_status_e status = a2e_init_client(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_ERROR, status);
    mock().checkExpectations();
}

/* Test a2e_init_server */
TEST(a2e_tests, a2e_init_server_success)
{
    mock().expectOneCall("a2e_init_mock").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_init_server(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

/* Test a2e_init_server */
TEST(a2e_tests, a2e_init_server_fail_not_implemented)
{
    mock().expectNoCall("a2e_init_mock");

    mock_strategy.init = NULL;

    a2e_status_e status = a2e_init_server(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_NOT_IMPLEMENTED, status);
    mock().checkExpectations();
}

TEST(a2e_tests, a2e_init_server_failure)
{
    mock().expectOneCall("a2e_init_mock").andReturnValue(eA2E_SC_ERROR);

    a2e_status_e status = a2e_init_server(&a2e, &cfg);

    LONGS_EQUAL(eA2E_SC_ERROR, status);
    mock().checkExpectations();
}

#if 0
/* Test a2e_close */
TEST(a2e_tests, a2e_close_success)
{
    mock().expectOneCall("close").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_close(a2e);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

TEST(a2e_tests, a2e_close_null_ptr)
{
    a2e_status_e status = a2e_close(NULL);
    LONGS_EQUAL(eA2E_SC_INVALID_ARGS, status);
}

/* Test a2e_request_rx */
TEST(a2e_tests, a2e_request_rx_success)
{
    uint8_t *buffer;
    uint32_t size;

    mock().expectOneCall("req_rx").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_request_rx(a2e, &buffer, &size, 100);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

TEST(a2e_tests, a2e_request_rx_null_args)
{
    a2e_status_e status = a2e_request_rx(NULL, NULL, NULL, 100);
    LONGS_EQUAL(eA2E_SC_INVALID_ARGS, status);
}

/* Test a2e_request_tx */
TEST(a2e_tests, a2e_request_tx_success)
{
    uint8_t buffer[10];

    mock().expectOneCall("req_tx").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_request_tx(a2e, buffer, sizeof(buffer), 100);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

/* Test a2e_request_complete */
TEST(a2e_tests, a2e_request_complete_success)
{
    mock().expectOneCall("req_cmplt").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_request_complete(a2e);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

/* Test a2e_response_rx */
TEST(a2e_tests, a2e_response_rx_success)
{
    uint8_t *buffer;
    uint32_t size;

    mock().expectOneCall("resp_rx").andReturnValue(eA2E_SC_OK);

    a2e_status_e status = a2e_response_rx(a2e, &buffer, &size, 100);

    LONGS_EQUAL(eA2E_SC_OK, status);
    mock().checkExpectations();
}

/* Test a2e_perror */
TEST(a2e_tests, a2e_perror_returns_correct_strings)
{
    STRCMP_EQUAL("OK", a2e_perror(eA2E_SC_OK));
    STRCMP_EQUAL("TIMEOUT", a2e_perror(eA2E_SC_TIMEOUT));
    STRCMP_EQUAL("INVALID_ARGS", a2e_perror(eA2E_SC_INVALID_ARGS));
    STRCMP_EQUAL("UNDEF_STATUS", a2e_perror((a2e_status_e)999));
}

/* Test a2e_cfg_set_default */
TEST(a2e_tests, a2e_cfg_set_default_sets_correct_values)
{
    a2e_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    a2e_cfg_set_default(&cfg);

    STRCMP_EQUAL(DEF_A2E_SOCK_DIR, cfg.sock_dir);
    STRCMP_EQUAL(DEF_A2E_NAME, cfg.name);
    LONGS_EQUAL(DEF_A2E_RW_CHUNK_SIZE, cfg.rw_chunk_size);
    LONGS_EQUAL(DEF_A2E_REQ_CMPLT_WAIT_TRIES, cfg.req_cmplt_wait_tries);
}

TEST(a2e_tests, a2e_cfg_set_default_null_ptr)
{
    a2e_cfg_set_default(NULL); /* Should not crash */
}
#endif
