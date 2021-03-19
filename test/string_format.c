#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct cport {
	uint8_t cspg;
	uint8_t slot;
	uint8_t port;
	uint8_t dummy;
};
	


int main()
{
	char line[24];
	char time[] = "22:30";
	char random_str[] = "12345#";
	char str[64];
	char ip[32];
	char ssw_answer[] = "SUCCESS\n\"222\"\n";
	char result[32];
	char *p;
	int res;
	uint8_t num;

	struct cport port;

	sprintf(line, "%-24s", time);

	printf("'%s'\n", line);

	sscanf(random_str, "%s#", line);
	
	printf("'%s'\n", line);


	sprintf(str, "u55s99p23");
	sscanf(str, "u%02hhus%03hhup%02hhu", &port.cspg, &port.slot, &port.port);
	printf("str:'%s' %02u %02u %02u\n", str, port.cspg, port.slot, port.port);
	
	sprintf(str, "u55s100p23");
	sscanf(str, "u%02hhus%03hhup%02hhu", &port.cspg, &port.slot, &port.port);
	printf("str:'%s' %02u %02u %02u\n", str, port.cspg, port.slot, port.port);

	sprintf(str, "u55s254p23");
	sscanf(str, "u%02hhus%03hhup%02hhu", &port.cspg, &port.slot, &port.port);
	printf("str:'%s' %02u %02u %02u\n", str, port.cspg, port.slot, port.port);

	sprintf(str, "u55s300p23");
	sscanf(str, "u%02hhus%03hhup%02hhu", &port.cspg, &port.slot, &port.port);
	printf("str:'%s' %02u %02u %02u\n", str, port.cspg, port.slot, port.port);

	sprintf(str, "u55s256p23");
	sscanf(str, "u%02hhus%03hhup%02hhu", &port.cspg, &port.slot, &port.port);
	printf("str:'%s' %02u %02u %02u\n", str, port.cspg, port.slot, port.port);

	sprintf(str, "127.0.0.1\r\n12334\r\n");
	sscanf(str, "%31s", ip);
	printf("str:'%s' ip:'%s'(%zd)\n", str, ip, strlen(ip));

	sprintf(str, "%s", ssw_answer);
	res = sscanf(str, "%s", result);
	res = sscanf(&str[strlen(result) + 1], "\"%hhu\"", &num);
	printf("str:'%s' result:'%s'(%zd) num:'%hhu' res: %d\n", str, result, strlen(result), num, res);

	sprintf(str, "%s", ssw_answer);
	p = strchr(str, '"');
	res = sscanf(p, "\"%hhu\"", &num);
	printf("str:'%s' num:'%hhu' res: %d\n", str, num, res);


	return 0;
}
