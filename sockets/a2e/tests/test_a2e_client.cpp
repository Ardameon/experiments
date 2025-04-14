#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

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

TEST_GROUP(a2e_client_test)
{
    a2e_cfg_t cfg;
    a2e_status_e status;
    a2e_client_t client;
    struct pollfd fds;

    void setup()
    {
        memset(&client, 0, sizeof(client));
        memset(&fds, 0, sizeof(fds));
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(a2e_client_test, client_conn_read_start_poll_timeout)
{
    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(0);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(status, eA2E_SC_TIMEOUT);
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
    CHECK_EQUAL(status, eA2E_SC_ERROR);
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
    CHECK_EQUAL(status, eA2E_SC_ERROR);
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
    CHECK_EQUAL(status, eA2E_SC_ERROR);
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
    CHECK_EQUAL(status, eA2E_SC_ERROR);
}
