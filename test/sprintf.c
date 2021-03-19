#include <stdio.h>
#include <stdlib.h>


int main()
{
	char *buf = malloc(4096);
	char buf1[256] = {0};
	int i;
	char *p = buf;





	for (i = 0; i < 250; i++)
	{
		buf1[i] = 'T';
	}

	p += sprintf(p, "%s", buf1);
	p += sprintf(p, "%s", buf1);
	p += sprintf(p, "%s", buf1);

	sprintf(p, "%.*s\n", 350, buf1);

	printf("buf: '%s'\n", buf);

	return 0;

}
