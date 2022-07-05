#include <iostream>

#include "adapter.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Plaine plane;
    IDuck *duck = new PlainAdapter(plane);

    duck->Fly();
    duck->Quack();

    (void)argv;
    (void)argc;

    return 0;
}

