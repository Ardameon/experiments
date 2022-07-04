#include <iostream>

#include "singleton.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Singletone *p1, *p2;
//    Singletone sg; /* can't do that */

    p1 = Singletone::GetInstance();
    p2 = Singletone::GetInstance();

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    (void)argv;
    (void)argc;

    return 0;
}

