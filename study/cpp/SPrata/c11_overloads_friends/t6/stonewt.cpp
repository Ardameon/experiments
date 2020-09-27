#include <iostream>
#include "stonewt.h"

using std::cout;

Stonewt::Stonewt(double lbs, StonewtMode mode)
{
    stone_ = (int)lbs / eLbsPerStn;
    pds_left_ = lbs - (stone_ * eLbsPerStn);
    pounds_ = lbs;
    mode_ = mode;
}

Stonewt::Stonewt(int stn, double lbs, StonewtMode mode)
{
    stone_ = stn;
    pds_left_ = lbs;
    pounds_ = stone_ * eLbsPerStn + pds_left_;
    mode_ = mode;
}

Stonewt::Stonewt()
{
    stone_ = pounds_ = pds_left_ = 0;
    mode_ = StonewtMode::eStone;
}

Stonewt::~Stonewt()
{
}

void Stonewt::ShowLbs() const
{
    cout << pounds_ << " pounds\n";
}

void Stonewt::ShowStn() const
{
    cout << stone_ << " stone, " << pds_left_ << " pounds\n";
}

Stonewt Stonewt::operator +(const Stonewt &obj) const
{
    return Stonewt(pounds_ + obj.pounds_);
}

Stonewt Stonewt::operator -(const Stonewt &obj) const
{
    double diff = pounds_ - obj.pounds_;
    return diff > 0 ? diff : 0.0;
}

Stonewt Stonewt::operator *(double mult) const
{
    return Stonewt(pounds_ * mult);
}

bool Stonewt::operator >(const Stonewt &obj) const
{
    return pounds_ > obj.pounds_;
}

bool Stonewt::operator <(const Stonewt &obj) const
{
    return pounds_ < obj.pounds_;
}

bool Stonewt::operator ==(const Stonewt &obj) const
{
    return pounds_ == obj.pounds_;
}

bool Stonewt::operator !=(const Stonewt &obj) const
{
    return !(*this == obj);
}

bool Stonewt::operator >=(const Stonewt &obj) const
{
    return (*this == obj) || (*this > obj);
}

bool Stonewt::operator <=(const Stonewt &obj) const
{
    return (*this == obj) || (*this < obj);
}

Stonewt operator +(double lbs, const Stonewt &obj)
{
    return obj + lbs;
}

Stonewt operator -(double lbs, const Stonewt &obj)
{
    return Stonewt(lbs) - obj;
}

Stonewt operator *(double lbs, const Stonewt &obj)
{
    return obj * lbs;
}

std::ostream &operator <<(std::ostream &os, const Stonewt &obj)
{
    if (obj.mode_ == StonewtMode::eStone)
    {
        os << obj.stone_ << " stone, " << obj.pds_left_ << " pounds";
    } else {
        os << obj.pounds_ << " pounds";
    }

    return os;
}

std::istream &operator >>(std::istream &is, Stonewt &obj)
{
    double lbs;

    is >> lbs;

    obj = Stonewt(lbs, obj.mode_);

    return is;
}
