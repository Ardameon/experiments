#include <iostream>
#include "set.h"

using namespace std;

int main()
{
    Set set_a, set_b, set_c, set_d, set_e, set_f;

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

    set_f = set_e;

    /* Show bool as false/true */
    cout << std::boolalpha;

    cout << "Set A: '" << set_a << '\'' << endl;
    cout << "Set B: '" << set_b << '\'' << endl;
    cout << "Set C: '" << set_c << '\'' << endl;
    cout << "Set D: '" << set_d << '\'' << endl;
    cout << "Set E: '" << set_e << '\'' << endl;
    cout << "Set F: '" << set_f << '\'' << endl;

    cout << "A contains B: " << set_a.Contain(set_b) << endl;
    cout << "A contains C: " << set_a.Contain(set_c) << endl;
    cout << "C contains A: " << set_c.Contain(set_a) << endl;
    cout << "D == B: " << (set_d == set_b) << endl;
    cout << "B == D: " << (set_b == set_d) << endl;
    cout << "C == D: " << (set_c == set_d) << endl;
    cout << "A == E: " << (set_a == set_e ) << endl;
    cout << "A != E: " << (set_a != set_e ) << endl;

    cout << "Remove 'a' from A:" << endl;
    set_a.Rem('a');
    cout << "Set A: '" << set_a << '\'' << endl;

    cout << "Remove 'c' from E:" << endl;
    set_e.Rem('c');
    cout << "Set E: '" << set_e << '\'' << endl;

    cout << "Remove " << set_b << " from F:" << endl;
    set_f.Rem(set_b);
    cout << "Set F: '" << set_f << '\'' << endl;

    cout << "Intersection of A and E: " << set_a.Intersection(set_e) << endl;
    cout << "Union of A and F: " << set_a.Union(set_f) << endl;
    cout << "A + F:            " << set_a + set_f << endl;
    cout << "Complement of F in A: " << set_a.Complement(set_f) << endl;
    cout << "A - F:                " << set_a - set_f << endl;
    cout << "SymDiff of A and A:   " << set_a.SymDiff(set_a) << endl;
    cout << "SymDiff of A and F:   " << set_a.SymDiff(set_f) << endl;
    cout << "SymDiff of A and E:   " << set_a.SymDiff(set_e) << endl;

    Set set_g = Set(set_a);
    cout << "G = A:     " << set_g << endl;

    cout << "{1} + {2}: " << Set('1') + Set('2') << endl;

    /* Show bool as 0/1 */
    cout << std::noboolalpha;

    Set set_h("{a, {b, c}, {    }   , {1, {2, }, 3, 4}}");

    cout << "Set H (from string): " << set_h << endl;

    Set set_i;

    set_i.Add(set_i);
    set_i.Add(set_i);

    cout << "Set I: " << set_i << endl;

    Set set_j = "{a, b, c}";
    Set power_set;

    cout << "PowerSet of " << set_j << ":\n\t" << (power_set = set_j.PowerSet())
         << "\n\tsize " << power_set.Size() << endl;
    set_j = "{1, 2}";
    cout << "PowerSet of " << set_j << ":\n\t" << (power_set = set_j.PowerSet())
         << "\n\tsize " << power_set.Size() << endl;
    set_j = "{1, 2, {3, 4}}";
    cout << "PowerSet of " << set_j << ":\n\t" << (power_set = set_j.PowerSet())
         << "\n\tsize " << power_set.Size() << endl;

    return 0;
}
