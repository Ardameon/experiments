#include "set.h"
#include <sstream>

Set::Set() : size_(0)
{
}

//====================================================================================================================//

Set::Set(const std::string &str)
{
    (void)str;
}

//====================================================================================================================//

Set::Set(const char *str)
{
    Set(std::string(str));
}

//====================================================================================================================//

Set Set::operator+(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

Set Set::operator-(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

Set Set::operator=(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

template <typename T>
int Set::AddGen(const T &element, std::vector<T> &vector)
{
    int res = 0;

    if (size_ < eMaxSetSize)
    {
        vector.push_back(element);
        size_++;
    } else {
        res = -1;
    }

    return res;
}

//====================================================================================================================//

int Set::Add(const char &element)
{
    return AddGen(element, elements_);
}

//====================================================================================================================//

int Set::Add(const Set &set)
{
    return AddGen(set, sets_);
}

//====================================================================================================================//

int Set::Rem(const char &element)
{
    (void)element;
    int res = 0;
    return res;
}

//====================================================================================================================//

int Set::Rem(const Set &set)
{
    (void)set;
    return 0;
}

//====================================================================================================================//

Set Set::Union(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

Set Set::Intersection(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

std::string Set::ToString() const
{
    std::stringstream ss;

    if (size_)
    {
        ss << '{';

        for (auto element : elements_)
        {
            ss << element << ", ";
        }

        for (auto set : sets_)
        {
            ss << set << ", ";
        }

        // here we move stream insert position to skip last ',' and ' ' symbols
        ss.seekp(ss.tellp() - std::streampos(2));

        // here we replace ',' sybmol to '}', but ' ' symbol is still left (TODO: remove trailing ' ')
        ss << '}';
    }

    return ss.str();
}

//====================================================================================================================//

std::ostream & operator<<(std::ostream &os, const Set &set)
{
    return os << set.ToString();
}

