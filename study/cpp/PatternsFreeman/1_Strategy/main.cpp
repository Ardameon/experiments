#include "duck.hpp"

int main(int argc, char const *argv[])
{
    Duck *duck = new MellorDuck;
    duck->Show();
    duck->PerformFly();
    duck->PerformQuak();
    delete duck;

    duck = new RocketDuck;
    duck->Show();
    duck->PerformFly();
    duck->PerformQuak();
    delete duck;

    duck = new BathDuck;
    duck->Show();
    duck->PerformFly();
    duck->PerformQuak();
    delete duck;

    return 0;
}
