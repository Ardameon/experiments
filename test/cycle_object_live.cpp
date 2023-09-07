#include <iostream>

using namespace std;

class Test
{
public:
    Test()
    {
        cout << "Test created" << endl;
    }

    ~Test()
    {
        cout << "Test destroyed" << endl;
    }
};

int main()
{
    int i;

    for (i = 0; i < 100; i++)
    {
        cout << i << ": ";
        Test test_obj;
    }

    return 0;
}
