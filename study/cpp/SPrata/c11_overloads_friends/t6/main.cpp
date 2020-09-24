#include <iostream>
#include "stonewt.h"

using namespace std;

int main()
{
    Stonewt cat = 9.71;
    Stonewt my = Stonewt(141);
    Stonewt object(4, 2.33);
    Stonewt bag = Stonewt(32.11, StonewtMode::eLbs);

    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.precision(2);

    cout << "========= Task 5 ==================\n";

    cout << "Cat weight: "; cat.ShowStn();
    cout << "My weight: "; my.ShowStn();
    cout << "My weight in lbs:"; my.ShowLbs();
    cout << "object weight: "; object.ShowStn();
    cout << "object weight lbs: "; object.ShowLbs();

    cout << "Bag weight in lbs: " << bag << endl;
    bag.SetMode(StonewtMode::eStone);
    cout << "Bag weight in stones: " << bag << endl;

    cout << "Bag + My = " << bag + my << " or " << (bag + my).SetMode(StonewtMode::eLbs) << endl;
    cout << "3 * Cat = " << 3 * cat << " or " << (3 * cat).SetMode(StonewtMode::eLbs) << endl;
    cout << "Cat - object = " << cat - object << " or " << (cat - object).SetMode(StonewtMode::eLbs) << endl;
    cout << "object - cat = " << object - cat << " or " << (object - cat).SetMode(StonewtMode::eLbs) << endl;

    cout << "========== Task 6 =================\n";

    /* List initializing */
    Stonewt arr[6] = {21.2, 216.347, Stonewt(2, 77, StonewtMode::eLbs)};

    for (int i = 0; i < 6; i++)
    {
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }


    return 0;
}
