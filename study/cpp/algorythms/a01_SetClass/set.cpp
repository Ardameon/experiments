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

Set::Set(const char &c) : Set()
{
    Add(c);
}

//====================================================================================================================//

Set Set::operator +(const Set &set) const
{
    return Union(set);
}

//====================================================================================================================//

Set Set::operator -(const Set &set) const
{
    return Complement(set);
}

//====================================================================================================================//

Set &Set::operator =(const Set &set)
{
    elements_.clear();
    sets_.clear();
    size_ = 0;

    if (set != *this)
    {
        for (auto &el : set.elements_)
        {
            Add(el);
        }

        for (auto &st : set.sets_)
        {
            Add(st);
        }
    }

    return *this;
}

//====================================================================================================================//

bool Set::operator ==(const Set &set) const
{
    /* Very UNOPTIMIZED decision (TODO: find faster algorithm) */

    /* Check if instance contains all elements of set */
    for (auto &el : set.elements_)
    {
        if (!Contain(el)) return false;
    }

    /* Check if set contains all elements of instance */
    for (auto &el : elements_)
    {
        if (!set.Contain(el)) return false;
    }

    /* Check if instance contains all subsets of set */
    for (auto &el : set.sets_)
    {
        if (!Contain(el)) return false;
    }

    /* Check if set contains all subsets of instance */
    for (auto &el : sets_)
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

template<typename T>
int Set::RemGen(const T &element, std::vector<T> &vector)
{
    int res = -1;

    if (Contain(element))
    {
        for (auto el = vector.begin(); el != vector.end(); el++)
        {
            if (*el == element)
            {
                vector.erase(el);
                size_--;
                res = 0;
                break;
            }
        }
    }

    return res;
}

//====================================================================================================================//

int Set::Rem(const char &element)
{
    return RemGen(element, elements_);
}

//====================================================================================================================//

int Set::Rem(const Set &set)
{
    return RemGen(set, sets_);
}

//====================================================================================================================//

Set Set::Union(const Set &set) const
{
    Set result = *this;

    for (auto &el : set.elements_)
    {
        if (!result.Contain(el))
        {
            result.Add(el);
        }
    }

    for (auto &st : set.sets_)
    {
        if (!result.Contain(st))
        {
            result.Add(st);
        }
    }

    return result;
}

//====================================================================================================================//

Set Set::Complement(const Set &set) const
{
    Set result = *this;

    for (auto &el : set.elements_)
    {
        if (result.Contain(el))
        {
            result.Rem(el);
        }
    }

    for (auto &st : set.sets_)
    {
        if (result.Contain(st))
        {
            result.Rem(st);
        }
    }

    return result;
}

//====================================================================================================================//

Set Set::Intersection(const Set &set) const
{
    Set result;

    for (auto &el : elements_)
    {
        if (set.Contain(el))
        {
            result.Add(el);
        }
    }

    for (auto &st : sets_)
    {
        if (set.Contain(st))
        {
            result.Add(st);
        }
    }

    return result;
}

//====================================================================================================================//

Set Set::SymDiff(const Set &set) const
{
    return Union(set) - Intersection(set);
}

//====================================================================================================================//

std::string Set::ToString() const
{
    std::stringstream ss;

    if (size_)
    {
        unsigned int i;

        ss << '{';

        if (elements_.size())
        {
            for (i = 0; i < elements_.size() - 1; i++)
            {
                ss << elements_[i] << ", ";
            }

            ss << elements_[i];

            if (sets_.size())
            {
                ss << ", ";
            }
        }

        if (sets_.size())
        {
            for (i = 0; i < sets_.size() - 1; i++)
            {
                ss << sets_[i] << ", ";
            }

            ss << sets_[i];
        }

        ss << '}';

#if 0
        /* Not suitable code, output Set strings always contan trailing spaces (e. g. '{a, b, {0, 1} } ') */

        for (auto &element : elements_)
        {
            ss << element << ", ";
        }

        for (auto &set : sets_)
        {
            ss << set << ", ";
        }

        // here we move stream insert position to skip last ',' and ' ' symbols
        ss.seekp(ss.tellp() - std::streampos(2));

        // here we replace ',' sybmol to '}', but ' ' symbol is still left (TODO: remove trailing ' ')
        ss << '}';
#endif
    } else {
        ss << "{}";
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
