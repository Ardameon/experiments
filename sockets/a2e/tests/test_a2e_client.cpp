#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "a2e_def.h"
#include "a2e_client.h"

void *calloc_mock(size_t nmemb, size_t size);

#include "../src/a2e_client.c"

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    /* Need to sleep for timeout check */
    if (timeout > 0)
    {
        /* actual sleep is (timeout / 10) msec */
        usleep(timeout * 100);
    }

    return mock().actualCall("poll")
                 .withOutputParameter("fds", fds)
                 .withParameter("nfds", nfds)
                 .withIntParameter("timeout", timeout)
                 .returnIntValueOrDefault(0);
}

/* Need to use non-standart name because "calloc" macro is redefined by CppUTest */
void *calloc_mock(size_t nmemb, size_t size)
{
    return mock().actualCall("calloc")
                 .withUnsignedIntParameter("nmemb", nmemb)
                 .withUnsignedIntParameter("size", size)
                 .returnPointerValue();
}

int stat(const char *pathname, struct stat *statbuf)
{
    return mock().actualCall("stat")
                 .withStringParameter("pathname", pathname)
                 .withOutputParameter("statbuf", statbuf)
                 .returnIntValue();
}

static a2e_status_e client_alloc_test(a2e_client_t **client)
{
    return (a2e_status_e)mock().actualCall("client_alloc_test")
                               .withOutputParameter("client", client)
                               .returnIntValue();
}

static a2e_status_e client_start_test(a2e_client_t *client)
{
    return (a2e_status_e)mock().actualCall("client_start")
                               .withPointerParameter("client", client)
                               .returnIntValue();
}

TEST_GROUP(a2e_client_test)
{
    a2e_cfg_t cfg;
    a2e_status_e status;
    a2e_client_t client;
    a2e_client_t *p_client;
    a2e_client_t *tmp_client;
    struct pollfd fds;

    void setup()
    {
        memset(&client, 0, sizeof(client));
        memset(&fds, 0, sizeof(fds));
        strcpy(cfg.sock_dir, "/testdir");
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(a2e_client_test, client_init_func_success)
{
    p_client = &client;
    struct stat st;

    st.st_mode = S_IFDIR;

    mock().expectOneCall("calloc")
          .withUnsignedIntParameter("nmemb", 1)
          .withUnsignedIntParameter("size", sizeof(client))
          .andReturnValue(&client);

    mock().expectOneCall("stat")
          .withOutputParameterReturning("statbuf", &st, sizeof(st))
          .withStringParameter("pathname", cfg.sock_dir)
          .andReturnValue(0);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(eA2E_STATE_IDLE, tmp_client->base.state);
}


TEST(a2e_client_test, client_init_func_start_path_is_not_dir)
{
    client.base.state = eA2E_STATE_REQ_RX;
    p_client = &client;
    struct stat st;

    st.st_mode = S_IFBLK;

    mock().expectOneCall("calloc")
          .withUnsignedIntParameter("nmemb", 1)
          .withUnsignedIntParameter("size", sizeof(client))
          .andReturnValue(&client);

    mock().expectOneCall("stat")
          .withOutputParameterReturning("statbuf", &st, sizeof(st))
          .withStringParameter("pathname", cfg.sock_dir)
          .andReturnValue(0);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_init_func_start_wrong_path)
{
    client.base.state = eA2E_STATE_REQ_RX;
    p_client = &client;
    struct stat st;

    st.st_mode = S_IFDIR;

    mock().expectOneCall("calloc")
          .withUnsignedIntParameter("nmemb", 1)
          .withUnsignedIntParameter("size", sizeof(client))
          .andReturnValue(&client);

    mock().expectOneCall("stat")
          .withOutputParameterReturning("statbuf", &st, sizeof(st))
          .withStringParameter("pathname", cfg.sock_dir)
          .andReturnValue(-1);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_init_func_alloc_fail)
{
    p_client = &client;

    mock().expectOneCall("calloc")
          .withUnsignedIntParameter("nmemb", 1)
          .withUnsignedIntParameter("size", sizeof(client))
          .andReturnValue((void *)NULL);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_NO_MEM, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_timeout)
{
    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(0);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_TIMEOUT, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_error_POLLERR)
{
    fds.revents = POLLERR;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_error_POLLNVAL)
{
    fds.revents = POLLNVAL;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_error_POLLHUP)
{
    fds.revents = POLLHUP;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_no_POLLIN)
{
    fds.revents = 0; /* Successful call with no POLLIN */

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}
