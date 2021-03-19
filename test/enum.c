#include <stdio.h>
#include <stdint.h>

typedef enum {
	E_A = 0,
	E_B,
	E_C
} e_num;

typedef struct call_options {
    uint32_t
    dtmf_detect: 1,
    tel_event_pt: 8,
    vad: 1,
    cng_pt: 8,
    echo_cancel: 1,
    rtp_timeout: 11,
    rtcp_timeout: 11,
    ssrc: 32,
    test: 3,
    : 0;
} __attribute__((packed)) call_options_t;

int main()
{
	e_num e = 10;
	e_num e1 = -1;
	call_options_t opt;
	int res;

	opt.test = 16;
	opt.echo_cancel = 16;
	opt.vad = 1;

	res = opt.vad;

	printf("sizeof call_options_t: %d\n", sizeof(call_options_t));
	printf("enum value: %d\n", e);
	printf("enum1 value: %d\n", e1);
	printf("test value: %d\n", opt.test);
	printf("echo_cancel value: %d\n", opt.echo_cancel);
	printf("res value: %d\n", res);

	return 0;
}
