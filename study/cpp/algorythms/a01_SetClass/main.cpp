#include <iostream>
#include "set.h"

using namespace std;

int main()
{
    Set setA;
    Set setB;

    setA.Add('a');
    setA.Add('b');
    setA.Add('c');

    setB.Add('0');
    setB.Add('1');

    setA.Add(setB);

    cout << "Hello set: '" << setA << '\'' << endl;

    return 0;
}
