#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "a2e.h"

#define BUF_SIZE 1024
#define TO 500

void log_fun(const char *str, int len)
{
    printf("[A2E_TEST_CLIENT] %.*s\n", len, str);
}

int main(void)
{
    a2e_t *client;
    uint8_t *buf_rx;
    int reqests_cnt = 5;
    uint8_t buf_tx[BUF_SIZE];
    a2e_cfg_t cfg;
    uint32_t size;
    a2e_status_e status;
    int i;

    a2e_dbg_on();
    a2e_dbg_set_func(&log_fun);

    a2e_cfg_set_default(&cfg);

    a2e_init_client(&client, &cfg);

    for(i = 0; i < reqests_cnt; i++)
    {
        size = 233;
        memset(buf_tx, i + '0', size);

        do
        {
            status = a2e_request_tx(client, buf_tx, size, TO);
            printf("AAAAA. TX status: %s\n", a2e_perror(status));

            if (status == eA2E_SC_OK)
                break;

        } while (status == eA2E_SC_TIMEOUT || status == eA2E_SC_CONTINUE || status == eA2E_SC_CONTINUE_TIMEOUT);

        if (status == eA2E_SC_OK)
            printf("TX[%d]: %.*s\n", size, size, buf_tx);
        else
            printf("TX Failed: %s\n", a2e_perror(status));

        if (status == eA2E_SC_OK)
        {
            do
            {
                status = a2e_response_rx(client, &buf_rx, &size, TO);
                printf("AAAAA. RX status: %s\n", a2e_perror(status));

                if (status == eA2E_SC_OK)
                    break;

            } while (status == eA2E_SC_TIMEOUT || status == eA2E_SC_CONTINUE || status == eA2E_SC_CONTINUE_TIMEOUT);

            if (status == eA2E_SC_OK)
                printf("RX[%d]: %.*s\n", size, size, buf_rx);
            else
                printf("RX Failed: %s\n", a2e_perror(status));
        }

        a2e_request_complete(client);
    }

    a2e_close(client);

    return 0;
}
