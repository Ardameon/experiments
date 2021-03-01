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

    template <typename T>
    int RemGen(const T &element, std::vector<T> &vector);

    template <typename T>
    bool ContainGen(const T &element, const std::vector<T> &vector) const;

    int FromString(std::string &str);
    bool StrSetIsValid(const std::string &str) const;
    int GetNextToken(std::string::iterator &it, std::string::iterator end);
    int ParseSet(std::string::iterator &it, std::string::iterator end, Set &set);

public:
    enum {eMaxSetSize = 50};

public:
    Set();
    Set(const std::string &str);
    Set(const char *str);
    Set(const char &c);

    Set operator +(const Set &set) const;
    Set operator -(const Set &set) const;
    Set &operator =(const Set &set);
    bool operator ==(const Set &set) const;
    bool operator !=(const Set &set) const;

    int Size() const {return size_;}

    int Add(const char &element);
    int Add(const Set &set);

    int Rem(const char &element);
    int Rem(const Set &set);

    bool Contain(const char &element) const;
    bool Contain(const Set &set) const;

    Set Union(const Set &set) const;
    Set Complement(const Set &set) const;
    Set Intersection(const Set &set) const;
    Set SymDiff(const Set &set) const;
    Set PowerSet() const;

    std::string ToString() const;

    friend std::ostream &operator<<(std::ostream & os, const Set &set);
};

#endif // SET_H
