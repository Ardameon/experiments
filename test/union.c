#include <stdio.h>
#include <stdint.h>

enum msg_e {
	E_MSG_REQUEST,
	E_SMG_RESPONSE
};

typedef struct {
	int a;
	int data[0];
} __attribute__((packed)) data;

struct msg
{
	uint8_t msg_type;
	union
	{
		struct {
			uint8_t  request_type;
			uint32_t ip_addr;
			uint32_t tr_id;
			uint8_t  op_code;
			uint8_t  param[4];
		} msg_request;

		struct {
			uint8_t  response_code;
			uint32_t tr_id;
		} msg_response;
	};
};

int main(int argc, char const *argv[])
{
	struct msg msg1, msg2;
	data m1;
	int b = 5;

	msg1.msg_type = E_MSG_REQUEST;
	msg1.msg_request.request_type = 2;
	msg1.msg_request.tr_id = 111111;
	msg1.msg_request.op_code = 20;
	msg1.msg_request.param[0] = 0;
	msg1.msg_request.param[1] = 1;
	msg1.msg_request.param[2] = 2;
	msg1.msg_request.param[3] = 3;

	m1.a = 0x11111111;

	msg2.msg_type = E_SMG_RESPONSE;
	msg2.msg_response.response_code = 200;
	msg2.msg_response.tr_id = 222222;

	printf("sizeof msg1:%lu msg2:%lu\n", sizeof(msg1), sizeof(msg2));
	printf("msg1: %d %d %d %d %d %d %d\n", msg1.msg_request.request_type, msg1.msg_request.tr_id, msg1.msg_request.op_code,
		msg1.msg_request.param[0], msg1.msg_request.param[1], msg1.msg_request.param[2], msg1.msg_request.param[3]);

	msg1.msg_type = E_SMG_RESPONSE;
	msg1.msg_response.response_code = 150;
	msg1.msg_response.tr_id = 333333;

	printf("msg2: %d %d\n", msg2.msg_response.response_code, msg2.msg_response.tr_id);
	printf("msg1: %d %d\n", msg1.msg_response.response_code, msg1.msg_response.tr_id);

	printf("msg1: %d %d %d %d %d %d %d\n", msg1.msg_request.request_type, msg1.msg_request.tr_id, msg1.msg_request.op_code,
		msg1.msg_request.param[0], msg1.msg_request.param[1], msg1.msg_request.param[2], msg1.msg_request.param[3]);


	printf("data: %#x size: %lu\n", m1.data[0], sizeof(data));

	return 0;
}
