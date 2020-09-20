#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <string>


class Set
{
    std::vector<char> elements_;
    std::vector<Set> sets_;
    int size_;

    template <typename T>
    int AddGen(const T &element, std::vector<T> &vector);

public:
    enum {eMaxSetSize = 50};

public:
    Set();
    Set(const std::string &str);
    Set(const char *str);

    Set operator +(const Set &set) const;
    Set operator -(const Set &set) const;
    Set operator =(const Set &set) const;

    int Size() const {return size_;}

    int Add(const char &element);
    int Add(const Set &set);

    int Rem(const char &element);
    int Rem(const Set &set);

    Set Union(const Set &set) const;
    Set Intersection(const Set &set) const;

    std::string ToString() const;

    friend std::ostream &operator<<(std::ostream & os, const Set &set);
};

#endif // SET_H
