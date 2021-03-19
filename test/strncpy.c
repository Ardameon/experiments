#include <string.h>
#include <ctype.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	char buf[] = "string_with"; 
	char buf_short_no_term[] = "string";
	char b[3] = "ppp";
	char new_buf[32];
	char new_buf_short[10];
	int i;
	
	buf_short_no_term[strlen(buf_short_no_term)] = '!';

	printf("sizeof: %d\n", sizeof(buf));

	memset(new_buf, '1', sizeof(new_buf));

	buf[6] = '\0';
	
	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");

	strncpy(new_buf, buf, sizeof(new_buf));

	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");

	buf[6] = '|';
	
	memset(new_buf, '1', sizeof(new_buf));
	
	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");
	
	strncpy(new_buf, buf, sizeof(new_buf));

	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");


	snprintf(new_buf_short, sizeof(new_buf_short), "This str is longer then size of buf short");
	
	for (i = 0; i < 10; i++)
	{
		printf("%c[%02x] ", new_buf_short[i], new_buf_short[i]);
	}

	printf("\n");

	memset(new_buf_short, '1', sizeof(new_buf_short));
	
	for (i = 0; i < 10; i++)
	{
		printf("%c[%02x] ", new_buf_short[i], new_buf_short[i]);
	}

	printf("\n");

	strncpy(new_buf_short, buf, sizeof(new_buf_short) - 1);

	for (i = 0; i < 10; i++)
	{
		printf("%c[%02x] ", new_buf_short[i], new_buf_short[i]);
	}

	printf("\n");

	memset(new_buf_short, '1', sizeof(new_buf_short));
	
	for (i = 0; i < 10; i++)
	{
		printf("%c[%02x] ", new_buf_short[i], new_buf_short[i]);
	}

	printf("\n");

	strncpy(new_buf_short, buf_short_no_term, sizeof(new_buf_short));

	for (i = 0; i < 10; i++)
	{
		printf("%c[%02x] ", new_buf_short[i], new_buf_short[i]);
	}

	printf("\n");

	printf("%d\n", buf_short_no_term[sizeof(buf_short_no_term)]);

	memset(new_buf, '1', sizeof(new_buf));
	
	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");
	
	strncpy(new_buf, b, sizeof(b));

	for (i = 0; i < 32; i++)
	{
		printf("%c[%02x] ", new_buf[i], new_buf[i]);
	}

	printf("\n");

	return 0; 
}
