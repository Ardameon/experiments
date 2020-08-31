#include <iostream>
#include <cstring>
#include <cstdint>


using namespace std;

class TestClass
{
    private:
        int privateVar;
        int privateMethod();

    public:
        int publicVar;
        int publicMethod();

    protected:
        int protectedVar;
        int protectedMethod();
        
};

int TestClass::publicMethod()
{
    return 2;
}

inline char *inline_func()
{
    char *a = new char[20];

    strcpy(a, "Dota");

    return a;
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

    TestClass obj;
    uint32_t a;

    cout << "Hello world\n" << obj.publicMethod() << '\n' << inline_func();

    a = obj.publicVar = 2;

    return a;
}
