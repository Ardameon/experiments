#ifndef STONEWT_H
#define STONEWT_H

#include <ostream>
#include <istream>

enum class StonewtMode
{
    eStone,
    eLbs
};

class Stonewt
{    
private:
    enum {eLbsPerStn = 14};
    int stone_;
    double pds_left_;
    double pounds_;
    StonewtMode mode_;

public:
    Stonewt(double lbs, StonewtMode mode = StonewtMode::eStone);
    Stonewt(int stn, double lbs, StonewtMode mode = StonewtMode::eStone);
    Stonewt();
    ~Stonewt();
    Stonewt &SetMode(StonewtMode mode) {mode_ = mode; return *this;}
    void ShowLbs() const;
    void ShowStn() const;

    Stonewt operator +(const Stonewt &obj) const;
    Stonewt operator -(const Stonewt &obj) const;
    Stonewt operator *(double mult) const;

    bool operator >(const Stonewt &obj) const;
    bool operator <(const Stonewt &obj) const;
    bool operator ==(const Stonewt &obj) const;
    bool operator !=(const Stonewt &obj) const;
    bool operator >=(const Stonewt &obj) const;
    bool operator <=(const Stonewt &obj) const;

    friend Stonewt operator +(double lbs, const Stonewt &obj);
    friend Stonewt operator -(double lbs, const Stonewt &obj);
    friend Stonewt operator *(double lbs, const Stonewt &obj);

    friend std::ostream &operator <<(std::ostream &os, const Stonewt &obj);
    friend std::istream &operator >>(std::istream &is, Stonewt &obj);
};

#endif // STONEWT_H
