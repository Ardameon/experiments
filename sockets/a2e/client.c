#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "a2e.h"

#define MSG_SIZE 2 * 1024 * 1024
#define TO 500

void log_fun(const char *str, int len)
{
    printf("[A2E_TEST_CLIENT] %.*s\n", len, str);
}

int main(void)
{
    a2e_t *client;
    uint8_t *buf_rx;
    int reqests_cnt = 1;
    a2e_cfg_t cfg;
    uint8_t *buf_tx;
    uint32_t size;
    a2e_status_e status;
    int i;

    a2e_dbg_on();
    a2e_dbg_set_func(&log_fun);

    a2e_cfg_set_default(&cfg);

    strcpy(cfg.name, "CLIENT_NAME");

    a2e_init_client(&client, &cfg);

    for(i = 0; i < reqests_cnt; i++)
    {
        size = MSG_SIZE;
        buf_tx = malloc(size);

        memset(buf_tx, i + '0', size);

        do
        {
            status = a2e_request_tx(client, buf_tx, size, TO);
            printf("AAAAA. TX status: %s\n", a2e_perror(status));

            if (status == eA2E_SC_OK)
                break;

        } while (status == eA2E_SC_TIMEOUT || status == eA2E_SC_CONTINUE || status == eA2E_SC_CONTINUE_TIMEOUT);

        //if (status == eA2E_SC_OK)
        //    printf("TX[%d]: %.*s\n", size, size, buf_tx);
        //else
        //    printf("TX Failed: %s\n", a2e_perror(status));

        if (status == eA2E_SC_OK)
        {
            do
            {
                status = a2e_response_rx(client, &buf_rx, &size, TO);
                printf("AAAAA. RX status: %s\n", a2e_perror(status));

                if (status == eA2E_SC_OK)
                    break;

                if (status == eA2E_SC_IN_PROGRESS)
                {
                    printf("AAAAA. Progress received: %s\n", a2e_perror(status));
                    status = eA2E_SC_CONTINUE;
                    continue;
                }

            } while (status == eA2E_SC_TIMEOUT || status == eA2E_SC_CONTINUE || status == eA2E_SC_CONTINUE_TIMEOUT);

            //if (status == eA2E_SC_OK)
            //    printf("RX[%d]: %.*s\n", size, size, buf_rx);
            //else
            //    printf("RX Failed: %s\n", a2e_perror(status));
        }

        a2e_request_complete(client);

        free(buf_tx);
    }

    a2e_close(client);

    return 0;
}
