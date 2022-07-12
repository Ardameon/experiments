#include <iostream>

#include "facade.h"

using namespace std;

int main(int argc, char const *argv[])
{
    HomeTheaterFacade home_theater;

    home_theater.PlayStreaming();
    home_theater.PlayRadio();
    home_theater.PlayDVD();

    (void)argv;
    (void)argc;

    return 0;
}

