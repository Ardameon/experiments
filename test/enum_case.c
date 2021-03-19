#include <stdint.h>
#include <stdio.h>

typedef enum {
	ENUM1,
	ENUM2 = 10,
	ENUM3,
	ENUM4,
} e_type;

typedef enum {
	E_ELMIN = 0,
	E_EL0 = E_ELMIN,
	E_EL1,
	E_EL2,
	E_EL3,
	E_EL4,
	E_EL5,
	E_ELMAX
} e_type_min_max;

int main()
{
	e_type a = ENUM4;
	e_type_min_max am;

	uint8_t b = 3;


	switch (a)
	{
		case ENUM1: a += 1; break;
		case ENUM2: a += 2; break;
		case ENUM3: a += 3; break;
		case ENUM4: a += 4; break;
	}

	for (a = ENUM1; a <= ENUM4; a++)
	{
		printf("enum: %d\n", a);
	}

	printf("EL0: %d\n", E_EL0);
	printf("EL1: %d\n", E_EL1);
	printf("EL2: %d\n", E_EL2);
	printf("EL3: %d\n", E_EL3);
	printf("EL4: %d\n", E_EL4);
	printf("EL5: %d\n", E_EL5);

	for (am = E_ELMIN; am <= E_ELMAX; am++)
	{
		printf("enum_mm: %d\n", am);
	}

	return 0;
}
