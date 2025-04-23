#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "a2e_def.h"
#include "a2e_client.h"
#include "a2e_msg.h"

void *calloc_mock(size_t nmemb, size_t size);
void *malloc_mock(size_t size);
void free_mock(void *ptr);

#include "../src/a2e_client.c"

int socket(int domain, int type, int protocol)
{
    return mock().actualCall("socket")
                 .withIntParameter("domain", domain)
                 .withIntParameter("type", type)
                 .withIntParameter("protocol", protocol)
                 .returnIntValue();
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return mock().actualCall("connect")
                 .withIntParameter("sockfd", sockfd)
                 .withPointerParameter("addr", (void *)addr)
                 .withIntParameter("addrlen", addrlen)
                 .returnIntValue();
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
    return mock().actualCall("recv")
                 .withIntParameter("sockfd", sockfd)
                 .withOutputParameter("buf", buf)
                 .withUnsignedIntParameter("len", len)
                 .withIntParameter("flags", flags)
                 .returnIntValue();
}

int shutdown(int sockfd, int how)
{
    return mock().actualCall("shutdown")
                 .withIntParameter("sockfd", sockfd)
                 .withIntParameter("how", how)
                 .returnIntValue();
}

int close(int fd)
{
    return mock().actualCall("close")
                 .withIntParameter("fd", fd)
                 .returnIntValue();
}

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

/* Need to use non-standart name because "calloc" macro is redefined by CppUTest */
void *malloc_mock(size_t size)
{
    return mock().actualCall("malloc")
                 .withUnsignedIntParameter("size", size)
                 .returnPointerValue();
}

/* Need to use non-standart name because "free" macro is redefined by CppUTest */
void free_mock(void *ptr)
{
    mock().actualCall("free")
          .withPointerParameter("ptr", ptr);
}

int stat(const char *pathname, struct stat *statbuf)
{
    return mock().actualCall("stat")
                 .withStringParameter("pathname", pathname)
                 .withOutputParameter("statbuf", statbuf)
                 .returnIntValue();
}

#ifdef A2E_UTEST_MEM_MOCK
static a2e_status_e client_alloc(a2e_client_t **client)
{
    return (a2e_status_e)mock().actualCall("client_alloc")
                               .withOutputParameter("client", client)
                               .returnIntValue();
}

static void client_free(a2e_client_t *client)
{
    mock().actualCall("client_free")
          .withPointerParameter("client", client)
          .returnIntValue();
}
#endif

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
    a2e_msg_t msg;
    uint8_t *rx_buf;
    uint32_t rx_buf_size;
    struct pollfd fds;

    void setup()
    {
        memset(&client, 0, sizeof(client));
        memset(&fds, 0, sizeof(fds));
        memset(&msg, 0, sizeof(msg));
        strcpy(cfg.sock_dir, "/testdir");
        msg.magic = A2E_MSG_MAGIC;
        rx_buf = NULL;
        rx_buf_size = 0;
        p_client = &client;
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(a2e_client_test, client_conn_read_recv_full_with_three_attempts)
{
    const char *recv_buf = "111112222233333";
    fds.revents = POLLIN;
    client.fd = 999;
    client.base.cfg.rw_chunk_size = 5;
    client.rsp_size_exp = 15;
    client.rsp_size_recv = 0;
    uint8_t buf[50] = {0};
    client.rsp = buf;
    int recv_len_wanted = A2E_MIN(client.base.cfg.rw_chunk_size, client.rsp_size_exp - client.rsp_size_recv);
    int recv_len = recv_len_wanted;

    memset(buf, 'a', sizeof(buf));

    mock().expectNCalls(3, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectNCalls(3, "recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", recv_buf + client.rsp_size_recv, recv_len_wanted)
          .withUnsignedIntParameter("len", recv_len_wanted)
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)A2E_MIN(recv_len_wanted, strlen(recv_buf) + 1));

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 100);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(client.rsp_size_exp, client.rsp_size_recv);
    STRNCMP_EQUAL("111111111111111", (char *)client.rsp, client.rsp_size_recv);
}

TEST(a2e_client_test, client_conn_read_recv_full_in_second_chunk)
{
    const char *recv_buf = "111112222233333";
    fds.revents = POLLIN;
    client.fd = 999;
    client.base.cfg.rw_chunk_size = strlen(recv_buf) + 1;
    client.rsp_size_exp = 10;
    client.rsp_size_recv = 5;
    uint8_t buf[50] = {0};
    client.rsp = buf;
    int recv_len_wanted = A2E_MIN(client.base.cfg.rw_chunk_size, client.rsp_size_exp - client.rsp_size_recv);
    int recv_len = recv_len_wanted;

    memset(buf, 'a', sizeof(buf));

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", recv_buf + client.rsp_size_recv, recv_len_wanted)
          .withUnsignedIntParameter("len", recv_len_wanted)
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)A2E_MIN(recv_len_wanted, strlen(recv_buf) + 1));

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(client.rsp_size_exp, client.rsp_size_recv);
    STRNCMP_EQUAL("aaaaa22222", (char *)client.rsp, client.rsp_size_recv);
}

TEST(a2e_client_test, client_conn_read_recv_full_in_one_chunk)
{
    const char *recv_buf = "111112222233333";
    fds.revents = POLLIN;
    client.fd = 999;
    client.base.cfg.rw_chunk_size = strlen(recv_buf) + 1;
    client.rsp_size_exp = strlen(recv_buf) + 1;
    client.rsp_size_recv = 0;
    uint8_t buf[50] = {0};
    client.rsp = buf;
    int recv_len_wanted = A2E_MIN(client.base.cfg.rw_chunk_size, client.rsp_size_exp - client.rsp_size_recv);
    int recv_len = recv_len_wanted;

    memset(buf, 'a', sizeof(buf));

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", recv_buf + client.rsp_size_recv, recv_len_wanted)
          .withUnsignedIntParameter("len", recv_len_wanted)
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)A2E_MIN(recv_len_wanted, strlen(recv_buf) + 1));

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(client.base.cfg.rw_chunk_size, client.rsp_size_recv);
    CHECK_EQUAL(strlen(recv_buf) + 1, client.rsp_size_recv);
    STRNCMP_EQUAL(recv_buf, (char *)client.rsp, client.rsp_size_recv);
}

TEST(a2e_client_test, client_conn_read_recv_part_and_timeout)
{
    const char *recv_buf = "111112222233333";
    fds.revents = POLLIN;
    client.fd = 999;
    client.base.cfg.rw_chunk_size = 5;
    client.rsp_size_exp = strlen(recv_buf) + 1;;
    client.rsp_size_recv = 0;
    uint8_t buf[50] = {0};
    client.rsp = buf;
    int recv_len_wanted = A2E_MIN(client.base.cfg.rw_chunk_size, client.rsp_size_exp - client.rsp_size_recv);
    int recv_len = recv_len_wanted;

    memset(buf, 'a', sizeof(buf));

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", recv_buf + client.rsp_size_recv, recv_len_wanted)
          .withUnsignedIntParameter("len", recv_len_wanted)
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)A2E_MIN(recv_len_wanted, strlen(recv_buf) + 1));

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_CONTINUE_TIMEOUT, status);
    CHECK_EQUAL(5, client.rsp_size_recv);
    CHECK_EQUAL(client.base.cfg.rw_chunk_size, client.rsp_size_recv);
    STRNCMP_EQUAL(recv_buf, (char *)client.rsp, client.rsp_size_recv);
}

TEST(a2e_client_test, client_conn_read_recv_fail)
{
    fds.revents = POLLIN;
    client.fd = 999;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .ignoreOtherParameters()
          .andReturnValue(-1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail_timeout)
{
    errno = EINTR;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(-1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_TIMEOUT, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail)
{
    errno = !EINTR;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(-1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail_no_POLLIN)
{
    fds.revents = 0;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail_POLLHUP)
{
    fds.revents = POLLHUP;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail_POLLNVAL)
{
    fds.revents = POLLNVAL;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_fail_POLLERR)
{
    fds.revents = POLLERR;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_poll_timeout)
{
    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(0);

    status = client_conn_read(&client, &rx_buf, &rx_buf_size, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_TIMEOUT, status);
}

TEST(a2e_client_test, client_conn_read_wrong_param)
{
    status = client_conn_read(NULL, NULL, NULL, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_connect_success)
{
    client.fd = -1;

    mock().expectOneCall("socket")
          .ignoreOtherParameters()
          .andReturnValue(999);

    mock().expectOneCall("connect")
          .withIntParameter("sockfd", 999)
          .ignoreOtherParameters()
          .andReturnValue(0);

    status = client_conn_connect(&client);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_CONTINUE, status);
    CHECK_EQUAL(eA2E_STATE_REQ_TX_START, client.base.state);
}

TEST(a2e_client_test, client_conn_connect_connect_fail)
{
    client.fd = -1;

    mock().expectOneCall("socket")
          .ignoreOtherParameters()
          .andReturnValue(999);

    mock().expectOneCall("connect")
          .withIntParameter("sockfd", 999)
          .ignoreOtherParameters()
          .andReturnValue(-1);

    status = client_conn_connect(&client);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_connect_socket_fail)
{
    client.fd = -1;

    mock().expectOneCall("socket")
          .ignoreOtherParameters()
          .andReturnValue(-2);

    status = client_conn_connect(&client);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
    CHECK_EQUAL(-2, client.fd);
}

TEST(a2e_client_test, client_conn_connect_wrong_param)
{
    client.fd = -1;

    status = client_conn_connect(NULL);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_close_func_with_inactive_socket)
{
    client.fd = -1;

    mock().expectOneCall("client_free")
          .withPointerParameter("client", &client);

    status = client_close_func(&client);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(eA2E_STATE_NULL, client.base.state);
}

TEST(a2e_client_test, client_close_func_with_active_socket)
{
    client.fd = 999;

    mock().expectOneCall("shutdown")
          .withIntParameter("sockfd", client.fd)
          .withIntParameter("how", SHUT_RDWR)
          .andReturnValue(0);

    mock().expectOneCall("close")
          .withIntParameter("fd", client.fd)
          .andReturnValue(0);

    mock().expectOneCall("client_free")
          .withPointerParameter("client", &client);

    status = client_close_func(&client);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(eA2E_STATE_NULL, client.base.state);
}

TEST(a2e_client_test, client_init_func_success)
{
    struct stat st;

    st.st_mode = S_IFDIR;

    mock().expectOneCall("client_alloc")
          .withOutputParameterReturning("client", &p_client, sizeof(p_client))
          .andReturnValue(eA2E_SC_OK);

    mock().expectOneCall("stat")
          .withOutputParameterReturning("statbuf", &st, sizeof(st))
          .withStringParameter("pathname", cfg.sock_dir)
          .andReturnValue(0);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_OK, status);
    CHECK_EQUAL(eA2E_STATE_IDLE, tmp_client->base.state);
}

TEST(a2e_client_test, client_init_func_wrong_param)
{
    status = client_init_func(NULL, NULL);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_init_func_path_is_not_dir)
{
    client.base.state = eA2E_STATE_REQ_RX;
    struct stat st;

    st.st_mode = S_IFBLK;

    mock().expectOneCall("client_alloc")
          .withOutputParameterReturning("client", &p_client, sizeof(p_client))
          .andReturnValue(eA2E_SC_OK);

    mock().expectOneCall("stat")
          .withOutputParameterReturning("statbuf", &st, sizeof(st))
          .withStringParameter("pathname", cfg.sock_dir)
          .andReturnValue(0);

    status = client_init_func(&tmp_client, &cfg);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_init_func_wrong_path)
{
    client.base.state = eA2E_STATE_REQ_RX;
    struct stat st;

    st.st_mode = S_IFDIR;

    mock().expectOneCall("client_alloc")
          .withOutputParameterReturning("client", &p_client, sizeof(p_client))
          .andReturnValue(eA2E_SC_OK);

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
    mock().expectOneCall("client_alloc")
          .ignoreOtherParameters()
          .andReturnValue(eA2E_SC_NO_MEM);

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

TEST(a2e_client_test, client_conn_read_start_poll_fail_timeout)
{
    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(-1);

    errno = EINTR;

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_TIMEOUT, status);
}

TEST(a2e_client_test, client_conn_read_start_wrong_param)
{
    status = client_conn_read_start(NULL, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_poll_fail)
{
    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(-1);

    errno = !EINTR;

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
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

TEST(a2e_client_test, client_conn_read_start_recv_fail)
{
    fds.revents = POLLIN;
    client.fd = 999;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue(-1);

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_recv_len_less_then_msg)
{
    fds.revents = POLLIN;
    client.fd = 999;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)(sizeof(msg) - 1));

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_recv_wrong_magic)
{
    fds.revents = POLLIN;
    client.fd = 999;
    msg.magic = 999;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)(sizeof(msg)));

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_ERROR, status);
}

TEST(a2e_client_test, client_conn_read_start_recv_progress)
{
    fds.revents = POLLIN;
    client.fd = 999;
    msg.type = eA2E_MSG_PROGRESS;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)(sizeof(msg)));

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_IN_PROGRESS, status);
    CHECK_EQUAL(eA2E_STATE_REQ_PROGRESS, client.base.state);
}

TEST(a2e_client_test, client_conn_read_start_rsp_malloc_fail)
{
    fds.revents = POLLIN;
    client.fd = 999;
    msg.len = 5;
    p_client = &client;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)(sizeof(msg)));

    client.rsp = NULL;

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_NO_MEM, status);
}

TEST(a2e_client_test, client_conn_read_start_success)
{
    fds.revents = POLLIN;
    client.fd = 999;
    msg.len = 5;
    uint8_t buf[5];

    client.rsp = buf;

    mock().expectNCalls(1, "poll")
          .withOutputParameterReturning("fds", &fds, sizeof(fds))
          .ignoreOtherParameters()
          .andReturnValue(1);

    mock().expectOneCall("recv")
          .withIntParameter("sockfd", client.fd)
          .withOutputParameterReturning("buf", &msg, sizeof(msg))
          .withUnsignedIntParameter("len", sizeof(msg))
          .withUnsignedIntParameter("flags", 0)
          .andReturnValue((int)(sizeof(msg)));

    status = client_conn_read_start(&client, 1);

    mock().checkExpectations();
    CHECK_EQUAL(eA2E_SC_CONTINUE, status);
    CHECK_EQUAL(eA2E_STATE_RSP_RX, client.base.state);
    CHECK_EQUAL(buf, client.rsp);
    CHECK_EQUAL(msg.len, client.rsp_size_exp);
}
