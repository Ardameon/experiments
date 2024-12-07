#include <stdio.h>
#include <stdint.h>

#include "a2e.h"

#define BUF_SIZE 1024
#define TO 500

void log_fun(const char *str, int len)
{
    printf("[CUSTOM_LOG_A2E] %.*s\n", len, str);
}

int main(void)
{
    //a2e_t *client;
    a2e_t *server;
    uint8_t *buf_rx;
    int reqests_cnt = 3;
    //uint8_t buf_tx[BUF_SIZE];
    a2e_cfg_t cfg;
    a2e_status_e status;
    int i;

    uint32_t size;

    a2e_dbg_on();
    a2e_dbg_set_func(&log_fun);

    a2e_cfg_set_default(&cfg);

    //a2e_init_client(&client, &cfg);
    //a2e_request_rx(client, &buf_rx, &size, TO);
    //a2e_request_tx(client, buf_tx, BUF_SIZE, TO);
    //a2e_request_complete(client);
    //a2e_response_rx(client, &buf_rx, &size, TO);
    //a2e_response_tx(client, buf_tx, size, TO);
    //a2e_progress_tx(client, TO);
    //a2e_close(client);

    /* for tests use netcat client
     * $ nc -U /tmp/a2e.sock
     */

    a2e_init_server(&server, &cfg);

    for(i = 0; i < reqests_cnt; i++)
    {
        do
        {
            status = a2e_request_rx(server, &buf_rx, &size, TO);
            printf("AAAAA. RX status: %s\n", a2e_perror(status));

            if (status == eA2E_SC_OK)
                break;

        } while (status == eA2E_SC_TIMEOUT || status == eA2E_SC_CONTINUE);

        printf("RX[%d]: %.*s\n", size, size, buf_rx);

        if (status == eA2E_SC_OK)
            a2e_progress_tx(server, TO);

        a2e_request_complete(server);
    }


    //a2e_request_tx(server, buf_tx, BUF_SIZE, TO);
    //a2e_request_complete(server);
    //a2e_response_rx(server, &buf_rx, &size, TO);
    //a2e_response_tx(server, buf_tx, size, TO);
    //a2e_progress_tx(server, TO);
    a2e_close(server);

    return 0;
}
