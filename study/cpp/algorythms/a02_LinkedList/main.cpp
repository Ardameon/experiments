#include <iostream>
#include <cstdlib>
#include <ctime>
#include "simplelist.h"
#include "mlist.h"

using std::cout;
using std::endl;

int main()
{
#if 0
    MList<int> l = {1, 2, 3, 4, 5};
    MList<int> l1 = MList<int>(10);
    MList<int> l2;
    MList<int> l3 = l1;

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

    cout << "=== l1 ===" << endl;

    cout << l1 << " mid: " << l1.GetMiddle() << endl;
    cout << l1.Reverse() << " mid: " << l1.GetMiddle() << endl;
    cout << l2 << " (" << l2.Size() << ")" << endl;
    cout << (l2 = l1) << " (" << l2.Size() << ")" << endl;
#else
    SimpleList l = {1, 2, 3, 4, 5};
    SimpleList l1 = SimpleList(10);
    SimpleList l2;
    SimpleList l3 = l1;

    srand(time(0));

    cout << std::boolalpha;
    cout << l << " (" << l.Size() << ")" << endl;

    cout << "Find  2: " << (l.Find(2) ? true : false) << endl;
    cout << "Find -2: " << (l.Find(-2) ? true : false) << endl;

    l.Erase(l.Find(-5));

    cout << l << " mid: " << l.GetMiddle()->data << endl;
    l.Erase(l.Find(2));
    cout << l << " mid: " << l.GetMiddle()->data << endl;
    l.Erase(l.Find(1));
    cout << l << " mid: " << l.GetMiddle()->data << endl;
    l.Erase(l.Find(5));
    cout << l << " mid: " << l.GetMiddle()->data << endl;

    cout << "=== l1 ===" << endl;

    cout << l1 << " mid: " << l1.GetMiddle()->data << endl;
    cout << l1.Reverse() << " mid: " << l1.GetMiddle()->data << endl;
    cout << l2 << " (" << l2.Size() << ")" << endl;
    cout << (l2 = l1) << " (" << l2.Size() << ")" << endl;
#endif

    return 0;
}
