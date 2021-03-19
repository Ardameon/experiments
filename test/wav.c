#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

int fun(int arg[2])
{
	printf("arg0: %d\n", arg[0]);
	printf("arg1: %d\n", arg[1]);

	return 0;
}

int main()
{
	char WAVEstr[] = "WAVE";
	char RIFFstr[] = "RIFF";
	uint32_t WAVE;
	uint8_t *WAVEptr = WAVEstr;

	printf("%c %c %c %c\n", WAVEstr[0], WAVEstr[1], WAVEstr[2], WAVEstr[3]);
	
	memcpy(&WAVE, WAVEstr, sizeof(WAVE));

	printf("%c %c %c %c\n", (WAVE >> 24) & 0xFF, (WAVE >> 16) & 0xFF, (WAVE >> 8) & 0xFF, (WAVE &  0xFF));

	WAVE = htonl(WAVE);
	
	printf("%c %c %c %c\n", (WAVE >> 24) & 0xFF, (WAVE >> 16) & 0xFF, (WAVE >> 8) & 0xFF, (WAVE &  0xFF));
	
	printf("%c %c %c %c\n", WAVEptr[0], WAVEptr[1], WAVEptr[2], WAVEptr[3]);

	int a[3] = {1, 2, 3};

	fun(a);

	return 0;
}
