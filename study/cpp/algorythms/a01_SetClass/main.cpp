#include <iostream>
#include "set.h"

using namespace std;

int main()
{
    Set set_a, set_b, set_c, set_d, set_e;

    set_a.Add('a');
    set_a.Add('b');
    set_a.Add('c');

    set_b.Add('0');
    set_b.Add('1');

    set_a.Add(set_b);

    set_c.Add('a');
    set_c.Add('c');

    set_d.Add('1');
    set_d.Add('0');

    set_e.Add('a');
    set_e.Add(set_b);
    set_e.Add('c');
    set_e.Add('c');
    set_e.Add('c');
    set_e.Add('b');

    /* Show bool as false/true */
    cout << std::boolalpha;

    cout << "Set A: '" << set_a << '\'' << endl;
    cout << "Set B: '" << set_b << '\'' << endl;
    cout << "Set C: '" << set_c << '\'' << endl;
    cout << "Set D: '" << set_d << '\'' << endl;
    cout << "Set E: '" << set_e << '\'' << endl;

    cout << "A contains B: " << set_a.Contain(set_b) << endl;
    cout << "A contains C: " << set_a.Contain(set_c) << endl;
    cout << "C contains A: " << set_c.Contain(set_a) << endl;
    cout << "D == B: " << (set_d == set_b) << endl;
    cout << "B == D: " << (set_b == set_d) << endl;
    cout << "C == D: " << (set_c == set_d) << endl;
    cout << "A == E: " << (set_a == set_e ) << endl;
    cout << "A != E: " << (set_a != set_e ) << endl;

    /* Show bool as 0/1 */
    cout << std::noboolalpha;

    return 0;
}
