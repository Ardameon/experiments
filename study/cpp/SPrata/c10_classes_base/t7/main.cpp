#include "plorg.h"
#include <iostream>

using std::cout;

int main()
{
    Plorg p1;
    Plorg p2 = {"Nemesis", 13};
    Plorg p3 = Plorg("Interceptor Predator Multiapp", -12);
    Plorg p4("Eve");
    Plorg p5("Josef Vissarionovich Dzhugashvilly");

    cout << "p1 - ";
    p1.Show();

    cout << "p2 - ";
    p2.Show();

    p2.SetCI(120);
    cout << "p2 changed - ";
    p2.Show();

    cout << "p3 - ";
    p3.Show();

    cout << "p4 - ";
    p4.Show();

    cout << "p5 - ";
    p5.Show();

    return 0;
}
