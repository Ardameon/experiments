#include "complexx.h"
#include <iostream>

Complex::Complex(double real, double i)
{
    r_ = real;
    i_ = i;
}

Complex Complex::operator +(const Complex &c) const
{
    return Complex(r_ + c.r_, i_ + c.i_);
}

Complex Complex::operator -(const Complex &c) const
{
    return Complex(r_ - c.r_, i_ - c.i_);
}

Complex Complex::operator *(const Complex &c) const
{
    return Complex((r_ * c.r_) - (i_ * c.i_), (r_ * c.i_) + (i_ * c.r_));
}

Complex Complex::operator ~() const
{
    return Complex(r_, -i_);
}

Complex operator +(double real, const Complex &c)
{
    return c + real;
}

Complex operator -(double real, const Complex &c)
{
    return Complex(real) - c;
}

Complex operator *(double real, const Complex &c)
{
    return c * real;
}

std::ostream &operator <<(std::ostream &os, const Complex &c)
{
    using std::ios_base;

    ios_base::fmtflags orig = os.setf(ios_base::fixed, ios_base::floatfield);
    std::streamsize prec = os.precision(1);

    os << "(" << c.r_ << ", " << c.i_ << ")";

    os.setf(orig, ios_base::floatfield);
    os.precision(prec);

    return os;
}

std::istream &operator >>(std::istream &is, Complex &c)
{
    double r, i;

    std::cout << "real: ";
    if (is >> r)
    {
        std::cout << "imaginary: ";
        if (is >> i)
        {
            c = Complex(r, i);
        }
    }

    return is;
}
