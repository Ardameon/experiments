#ifndef __A2E_MESSAGE_H__
#define __A2E_MESSAGE_H__

#include <stdint.h>
#include <limits.h>

#define A2E_MSG_LEN_MAX UINT_MAX
#define A2E_MSG_MAGIC 0xBADA77BC

typedef enum
{
    eA2E_MSG_REQEUST,
    eA2E_MSG_RESPONSE
} a2e_msg_type_e;

typedef struct a2e_msg_t
{
    a2e_msg_type_e type;
    uint32_t magic;
    uint32_t len;
}
/** @cond */
__attribute__((__packed__))
/** @endcond */
a2e_msg_t;

void a2e_msg_fill_req(a2e_msg_t *msg, uint32_t len);
void a2e_msg_fill_rsp(a2e_msg_t *msg, uint32_t len);


#endif /* __A2E_MESSAGE_H__ */
