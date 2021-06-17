#include <stdio.h>
#include <stdint.h>

struct st
{
	uint16_t ID;
	uint8_t a:1,
	        b:2,
	        c:1,
	        d:1,
	        e:1,
	        :0;

	uint8_t rem;
};

int main()
{
	struct st tmp = {
		17,
		1,
		2,
		0,
		3,
		1,
		255
	};


	printf("ID:%d a:%d b:%d c:%d d:%d e:%d rem:%d size:%d\n",
			tmp.ID, tmp.a, tmp.b, tmp.c, tmp.d, tmp.e, tmp.rem, sizeof(tmp));

	return 0;
}