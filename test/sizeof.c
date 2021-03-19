#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define SIZE 5

struct strc {
	char a;
	char b;
	char c;
	char d;
} __attribute__ ((packed));

struct hdr {
	uint8_t a;
	uint8_t b;
	uint8_t data[0];
} __attribute__ ((packed));

typedef enum {
	eNUM_0 = 0,
	eNUM_1 = 1,
	eNUM_2 = 2,
	eNUM_3 = 3,
	eNUM_4 = 4,
	eNUM_MAX = 250
} small_type_e;

typedef enum {
	eNUML_0 = 0,
	eNUML_1 = 1,
	eNUML_2 = 2,
	eNUML_3 = 3,
	eNUML_4 = 4,
	eNUML_MAX = 256
} large_type_e;

typedef enum {
	eNUMVL_0 = 0,
	eNUMVL_1 = UINT_MAX + 1
} very_large_type_e;


int main()
{
	struct hdr h;
	struct hdr *p;
	char a[] = "abc";
	char b[4] = {'a','b','c', '\0'};
	uint8_t arr[6];
	uint8_t arr1[4] = {0x41, 0x42, 0x51, 0x52};
	int aaa[SIZE];
	small_type_e small_enum;
	large_type_e large_enum;
	int val;
	char *b_p = b;

	printf("1) %p  2) %p\n", arr, &arr);
	printf("sizeof: %d\n", sizeof(arr));
	printf("sizeof a: %d\n", sizeof(a));
	printf("sizeof b: %d\n", sizeof(b));
	printf("sizeof b_p: %d\n", sizeof(b_p));

	printf("%lu %llu %lu\n", ULONG_MAX, ULLONG_MAX, UINT_MAX);

	printf("sizeof hedr: %d\n", sizeof(h));

	arr[0] = 0x11, arr[1] = 0x22;
	
	p = arr1;

	memcpy(&arr[2], p, sizeof(*p) + 2);

	printf("%#02x %#02x %#02x %#02x %#02x %#02x\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
	
	printf("Arr sizeof: %d\n", sizeof(aaa));

	printf("small_enum:%lu large_enum:%lu very_large_enum:%lu\n", sizeof(small_type_e), sizeof(large_type_e), sizeof(very_large_type_e));
	val = eNUMVL_1;
	printf("val:%lu int_max:%lu int:%lu\n", eNUMVL_1, UINT_MAX, val);

	return 0;
}
