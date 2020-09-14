#include "set.h"

Set::Set()
{

}

Set::Set(const std::string &str)
{
    (void)str;
}

Set::Set(const char *str)
{
    Set(std::string(str));
}

Set Set::operator+(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

Set Set::operator-(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

Set Set::operator=(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

int Set::Size() const
{
    return size_;
}

int Set::Add(const char element)
{
    (void)element;
    return 0;
}

int Set::Add(const Set &set)
{
    (void)set;
    return 0;
}

int Set::Rem(const char element)
{
    (void)element;
    return 0;
}

int Set::Rem(const Set &set)
{
    (void)set;
    return 0;
}

Set Set::Union(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

Set Set::Intersection(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

std::string Set::ToString() const
{
    std::string str;
    return str;
}

std::ostream & operator<<(std::ostream &os, const Set &set)
{
    (void)set;
    return os;
}
