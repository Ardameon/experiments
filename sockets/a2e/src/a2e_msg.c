#include "a2e_msg.h"

/**
 * @copybrief a2e_msg_fill_req
 *
 * @param[in]  msg  Message structure to fill
 * @param[in]  len  Message length
 */
void a2e_msg_fill_req(a2e_msg_t *msg, uint32_t len)
{
    if (msg)
    {
        msg->type = eA2E_MSG_REQEUST;
        msg->magic = A2E_MSG_MAGIC;
        msg->len = len;
    }
}

/**
 * @copybrief a2e_msg_fill_rsp
 *
 * @param[in]  msg  Message structure to fill
 * @param[in]  len  Message length
 */
void a2e_msg_fill_rsp(a2e_msg_t *msg, uint32_t len)
{
    if (msg)
    {
        msg->type = eA2E_MSG_RESPONSE;
        msg->magic = A2E_MSG_MAGIC;
        msg->len = len;
    }
}

/**
 * @copybrief a2e_msg_fill_prg
 *
 * @param[in]  msg  Message structure to fill
 */
void a2e_msg_fill_prg(a2e_msg_t *msg)
{
    if (msg)
    {
        msg->type = eA2E_MSG_PROGRESS;
        msg->magic = A2E_MSG_MAGIC;
        msg->len = 0;
    }
}
