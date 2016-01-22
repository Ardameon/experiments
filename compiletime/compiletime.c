#include <stdio.h>
#include <linux/types.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

#ifndef ARR_SIZE
#define ARR_SIZE 5
#endif

struct st {
	char array[ARR_SIZE];
};

struct st_one {
	u_int8_t a:4,
			b:1,
			c:2,
			d:1;
};

int func_with_static()
{
	static int initial = 1;

	printf("%s: initial: %d\n", __func__, initial);

	if(initial) initial = 0;
}

typedef enum {
	MODE_1,
	MODE_2,
	MODE_3
} mode_e;

int main(void)
{
	struct st str;
	struct st_one str1 = {0};
	struct st_one **pp;
	int a, b, c;
	int i;
	char ch_arr[64] = "SETUP 12300123 192.168.23.22:5555 GG";
	char msg[10], ip[30], mode[5];
	int call_id;
	int len;
	mode_e me;

	printf("Arr size: %lu\n", sizeof(str.array));

	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);

	int k = 255;

	str1.a = k;


	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);

	
	assert(str1.d == 1);

	c = a=2,b=2;

	c = (a == 2),(a == 1);

	printf("val = %d\n", c);

	for(i = 0; i < 4; i++)
		func_with_static();

	len = strlen(ch_arr);
	ch_arr[len] = '\r';
	ch_arr[len + 1] = '\n';

	printf("ch_arr: '%s'\n", ch_arr);
	len = sscanf(ch_arr, "%s %d %s %s", msg, &call_id, ip, mode);
	printf("msg:'%s' id:'%d' ip:'%s' mode:'%s' len %d\n", msg, call_id, ip, mode, len);
	printf("mode_len: %d\n", (int)strlen(mode));

	printf("sizeof pp:%d p:%d s:%d struct:%d\n", sizeof(pp), sizeof(*pp), sizeof(*pp[0]), sizeof(struct st_one));

	me = 244;

	printf("me: %d\n", me);

	return 0;
}
