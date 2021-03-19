#include <iostream>

using namesapace std;

class Part
{
public:
	Part(){};
	~Part(){};
	int FindTheWay(){ return 1; }
};

class Base
{
private:
	int a;
	int b;
	Part *p;

public:
	 Base();
	~Base();
	
};

Base::Base()
{
	p = new Part();

	if (p != NULL) 
}