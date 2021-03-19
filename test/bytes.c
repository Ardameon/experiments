#include <stdio.h>
#include <stdint.h>
#include <string.h>


struct {
	short   a:1,
			b:7;
}st;

struct stt {
	char num[10];
};

int func(int i)
{
	printf("i = %d\n", i);
	return 0;
}

int main()
{
	uint8_t a[4];
	uint16_t *b[2];
	uint32_t *c;
	uint8_t *d;

	a[0] = 0x00;
	a[1] = 0x11;
	a[2] = 0x22;
	a[3] = 0x33;

	b[0] = (uint16_t *)a;
	b[1] = (uint16_t *)(a + 2);

	c = (uint32_t *)a;
	d = c;

	printf("b[0]:%#x b[1]:%#x c:%#x c':%#x\n", *b[0], *b[1], *c, (*c) >> 8);
	printf("d[0]:%#x d[1]:%#x d[2]:%#x d[3]:%#x\n", d[0], d[1], d[2], d[3]);

	printf("sizeof int - %d\n", sizeof(double));
	printf("sizeof st - %d\n", sizeof(st));
	
	st.a = 0xFF;

	func(st.a);

	struct stt s1, s2;

	strcpy(s1.num, "1234567");
	strcpy(s2.num, "7654321");

	s1 = s2;

	printf("s2:%s  s1:%s\n", s2.num, s1.num);

	s2.num[2] = '6';
	
	printf("s2:%s  s1:%s\n", s2.num, s1.num);

	return 0;
}

