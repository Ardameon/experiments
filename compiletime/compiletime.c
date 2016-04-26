#include <stdio.h>
#include <linux/types.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

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


void thread_cleanup_func(void *arg)
{
	(void)arg;
	printf("Cleanup func!\n");
}

void *thread_routine(void *arg)
{
	(void)arg;
	int a = 1;
	printf("thread start\n");

	pthread_cleanup_push(&thread_cleanup_func, NULL);

	while(a)
	{
		usleep(100000);
	}

	pthread_cleanup_pop(1);

	return NULL;
}

int main(void)
{
	struct st str;
	struct st_one str1 = {0};
	struct st_one **pp;
	int a, b, c;
	int i, j;
	char ch_arr[64] = "SETUP 12300123 192.168.23.22:5555 GG";
	char msg[10], ip[30], mode[5];
	int call_id;
	int len;
	int sw = 3;
	char *carr[] = {"aaaaa", "bbbbb"};
	mode_e me;
	pthread_t thread;

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

	printf("sizeof: char*:%d int*:%d short*:%d char:%d int:%d short:%d long:%d long long:%d size_t:%d\n",
	sizeof(char *), sizeof(int *), sizeof(short *), sizeof(char), sizeof(int),
	sizeof(short), sizeof(long), sizeof(long long), sizeof(size_t));

	sw = 4;
	switch(sw)
	{
		default: printf("sw setting default\n");
		case 1: printf("sw == 1\n"); break;
		case 2: printf("sw == 2\n"); break;
		case 3: printf("sw == 3\n"); break;
	}

	for(i = 0; i < 3; i++)
	{
		printf("cycle i = %d\n", i);
		for(j = 0; j < 3; j++)
		{
			printf("cycle j = %d\n", j);
			if(j == 2 && i == 1) goto _exit;
		}
	}
_exit:


	pthread_create(&thread, NULL, &thread_routine, NULL);

	sleep(3);

	pthread_cancel(thread);

	printf("CANCEL\n");

	sleep(1);

	printf("%s %s\n", carr[0], carr[1]);

	return 0;
}
