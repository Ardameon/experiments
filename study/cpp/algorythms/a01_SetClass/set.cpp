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

Set Set::operator +(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

Set Set::operator -(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

Set Set::operator =(const Set &set) const
{
    (void)set;
    Set result;
    return result;
}

//====================================================================================================================//

bool Set::operator ==(const Set &set) const
{
    /* Very UNOPTIMIZED decision (TODO: find faster algorithm) */

    /* Check if instance contains all elements of set */
    for (auto el : set.elements_)
    {
        if (!Contain(el)) return false;
    }

    /* Check if set contains all elements of instance */
    for (auto el : elements_)
    {
        if (!set.Contain(el)) return false;
    }

    /* Check if instance contains all subsets of set */
    for (auto el : set.sets_)
    {
        if (!Contain(el)) return false;
    }

    /* Check if set contains all subsets of instance */
    for (auto el : sets_)
    {
        if (!set.Contain(el)) return false;
    }

    return true;
}

//====================================================================================================================//

bool Set::operator !=(const Set &set) const
{
    return !(*this == set);
}

//====================================================================================================================//

template <typename T>
int Set::AddGen(const T &element, std::vector<T> &vector)
{
    int res = 0;

    if (size_ < eMaxSetSize && !Contain(element))
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

//====================================================================================================================//

template<typename T>
bool Set::ContainGen(const T &element, const std::vector<T> &vector) const
{
    for (auto el : vector)
    {
        if (element == el)
        {
            return true;
        }
    }

    return false;
}

//====================================================================================================================//

bool Set::Contain(const char &element) const
{
    return ContainGen(element, elements_);
}

//====================================================================================================================//

bool Set::Contain(const Set &set) const
{
    return ContainGen(set, sets_);
}

