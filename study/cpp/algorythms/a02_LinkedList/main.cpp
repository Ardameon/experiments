#include <iostream>
#include <cstdlib>
#include "mlist.h"

using namespace std;

int main()
{
    MList<int> l = {1, 2, 3, 4, 5};
    MList<int> l1 = MList<int>(10);
    MList<int> l2;

    srand(time(0));

    cout << std::boolalpha;
    cout << l << " (" << l.Size() << ")" << endl;

    cout << "Find  2: " << l.Find(2) << endl;
    cout << "Find -2: " << l.Find(-2) << endl;

    l.Erase(-5);

    cout << l << " mid: " << l.GetMiddle() << endl;
    l.Erase(2);
    cout << l << " mid: " << l.GetMiddle() << endl;
    l.Erase(1);
    cout << l << " mid: " << l.GetMiddle() << endl;
    l.Erase(5);
    cout << l << " mid: " << l.GetMiddle() << endl;

    cout << l1 << " mid: " << l1.GetMiddle() << endl;
    cout << l1.Reverse() << " mid: " << l1.GetMiddle() << endl;
    cout << l2 << " (" << l2.Size() << ")" << endl;
    cout << (l2 = l1) << " (" << l2.Size() << ")" << endl;

    return 0;
}
