typedef enum 
{
	eOne,
	eTwo,
	eThree,
	eFour
} e_type;


int func(e_type et)
{
	switch (et)
	{
		case eOne: return 1;
		case eTwo: return 2;
		case eThree: return 3;
	}

	return -1;
}

int main()
{
	return func(20);
}
