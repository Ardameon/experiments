#ifndef COMPLEX_H
#define COMPLEX_H
#include <ostream>
#include <istream>


class Complex
{
private:
    double r_;
    double i_;

public:
    Complex(double real = 0.0, double i = 0.0);

    Complex operator +(const Complex &c) const;
    Complex operator -(const Complex &c) const;
    Complex operator *(const Complex &c) const;
    Complex operator ~() const;

    friend Complex operator +(double real, const Complex &c);
    friend Complex operator -(double real, const Complex &c);
    friend Complex operator *(double real, const Complex &c);

    friend std::ostream &operator <<(std::ostream &os, const Complex &c);
    friend std::istream &operator >>(std::istream &is, Complex &c);
};

#endif // COMPLEX_H
