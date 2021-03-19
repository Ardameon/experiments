#include <stdio.h>
#include <stdint.h>

struct st
{
	uint8_t a:1,
			:0;
} __attribute__((packed));

int main()
{
	struct st tmp[32];

	printf("size:%lu\n", sizeof(tmp));

	return 0;
}
