#include <iostream>

#include "iterator.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Array arr = {1, 2, 3, 4, 5, 6, 7, 8};
    List list = {1, 2, 3, 4, 5, 6, 7, 8};
    Iterator *it;

    cout << "Array: ";

    it = arr.CreateIterator();
    it->Begin();

    cout << it->Current() << " ";

    while (it->HasNext())
    {
        it->Next();
        cout << it->Current() << " ";
    }

    delete it;

    cout << endl;
    cout << "List:  ";

    it = list.CreateIterator();
    it->Begin();

    cout << it->Current() << " ";

    while (it->HasNext())
    {
        it->Next();
        cout << it->Current() << " ";
    }

    delete it;

    (void)argc;
    (void)argv;

    return 0;
}

