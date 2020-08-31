#include <iostream>
#include "move.h"

using namespace std;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Move mv1;
    Move mv2(2, 4);

    cout << "Mv1: ";
    mv1.Show();
    cout << "Mv2: ";
    mv2.Show();

    mv1 = mv1.Add(mv2);

    cout << "Mv1: ";
    mv1.Show();

    cout << "Added: ";
    mv1.Add(Move(12, -4)).Show();


    return 0;
}
