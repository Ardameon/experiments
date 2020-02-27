#include "increase.h"
#include "lib.h"

int increase(int number)
{
	if (!mod(number, 2))
	{
		number += 5;
	} else {
		number += 10;
	}

	return number;
}