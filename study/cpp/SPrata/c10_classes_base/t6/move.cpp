#include "move.h"
#include <iostream>

Move::Move(double x, double y)
{
    x_ = x;
    y_ = y;
}

void Move::Show() const
{
    using namespace std;

    cout << "(" << x_ << ", " << y_ << ")\n";
}

Move Move::Add(const Move &adding) const
{
    return Move(x_ + adding.x_, y_ + adding.y_);
}

void Move::Reset(double x, double y)
{
    x_ = x;
    y_ = y;
}
