#include "set_improved.h"
#include <sstream>
#include <cctype>

Set::Set() : size_(0)
{
}

//====================================================================================================================//

Set::Set(const Set &set)
{
    *this = set;
}

//====================================================================================================================//

Set::Set(const std::string &str)
{
    std::string new_str = str;
    FromString(new_str);
}

//====================================================================================================================//

Set::Set(const char *str) : Set(std::string(str))
{
}

//====================================================================================================================//

Set::Set(const char &c) : Set()
{
    Add(c);
}

//====================================================================================================================//

Set::~Set()
{
    Clear();
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
    if (&set != this)
    {
        Clear();
        size_ = 0;

        for (auto el : set.components_)
        {
            components_.push_back(el->CreateCopy());
            size_++;
        }
    }

    return *this;
}

//====================================================================================================================//

bool Set::operator ==(const Set &set) const
{
    /* Very UNOPTIMIZED decision (TODO: find faster algorithm) */

    /* Check if instance contains all elements of set */
    for (auto el : set.components_)
    {
        if (!ContainGen(*el)) return false;
    }

    /* Check if set contains all elements of instance */
    for (auto el : components_)
    {
        if (!set.ContainGen(*el)) return false;
    }

    return true;
}

//====================================================================================================================//

bool Set::operator !=(const Set &set) const
{
    return !(*this == set);
}

//====================================================================================================================//

int Set::AddGen(const Component &component)
{
    int res = 0;

    if (size_ < eMaxSetSize && !ContainGen(component))
    {
        components_.push_back(component.CreateCopy());
        size_++;
    } else {
        res = -1;
    }

    return res;
}

//====================================================================================================================//

int Set::Add(const char &element)
{
    return AddGen(ComponentElement(element));
}

//====================================================================================================================//

int Set::Add(const Set &set)
{
    return AddGen(ComponentSet(set));
}

//====================================================================================================================//

int Set::RemGen(const Component &component)
{
    int res = -1;

    if (ContainGen(component))
    {
        for (auto el = components_.begin(); el != components_.end(); el++)
        {
            if (**el == component)
            {
                delete *el;
                components_.erase(el);
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
    return RemGen(ComponentElement(element));
}

//====================================================================================================================//

int Set::Rem(const Set &set)
{
    return RemGen(ComponentSet(set));
}

//====================================================================================================================//

Set Set::Union(const Set &set) const
{
    Set result = *this;

    for (auto el : set.components_)
    {
        if (!result.ContainGen(*el))
        {
            result.AddGen(*el);
        }
    }

    return result;
}

//====================================================================================================================//

Set Set::Complement(const Set &set) const
{
    Set result = *this;

    for (auto el : set.components_)
    {
        result.RemGen(*el);
    }

    return result;
}

//====================================================================================================================//

Set Set::Intersection(const Set &set) const
{
    Set result;

    for (auto el : components_)
    {
        if (set.ContainGen(*el))
        {
            result.AddGen(*el);
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

Set Set::PowerSet() const
{
    /* Find set of all subsets of current set */
    Set result;

    /* Add empty set */
    result.Add(result);

    for (auto el : components_)
    {
        Set tmp_set = result;

        for (auto tmp_st : tmp_set.components_)
        {
            dynamic_cast<ComponentSet *>(tmp_st)->set_.AddGen(*el);
        }

        result = result + tmp_set;
    }

    return result;
}

//====================================================================================================================//

std::string Set::ToString() const
{
    std::stringstream ss;

    if (size_)
    {
        unsigned int i;

        ss << '{';

        if (components_.size())
        {
            for (i = 0; i < components_.size() - 1; i++)
            {
                ss << components_[i]->ToString() << ", ";
            }

            ss << components_[i]->ToString();
        }

        ss << '}';
    } else {
        ss << "{}";
    }

    return ss.str();
}

//====================================================================================================================//

int Set::FromString(std::string &str)
{
    Set set;
    int res = -1;

    size_ = 0;

    if (StrSetIsValid(str))
    {
        std::string::iterator start = str.begin();

        if (!ParseSet(start, str.end(), set))
        {
            *this = set;
            res = 0;
        }
    }

    return res;
}

//====================================================================================================================//

bool Set::StrSetIsValid(const std::string &str) const
{
    int brace_cnt = 0;
    bool is_valid = false;

    auto it = str.begin();

    if (*it != '{') goto _exit;

    for (;it != str.end(); it++)
    {
        if (*it == '{') brace_cnt++;
        if (*it == '}') brace_cnt--;

        if (brace_cnt < 0) goto _exit;
    }

    if (brace_cnt) goto _exit;

    is_valid = true;

_exit:
    return is_valid;
}

//====================================================================================================================//

int Set::GetNextToken(std::string::iterator &it, std::string::iterator end)
{
    int res = -1;

    while (it != end)
    {
        if (*it == ' ') { it++; continue;}

        if (*it == ',' || *it == '}')
        {
            res = 0;
        }

        break;
    }

    return res;
}

//====================================================================================================================//

int Set::ParseSet(std::string::iterator &it, std::string::iterator end, Set &set)
{
    char element;
    int res = -1;

    if (*it != '{') return res;

    it++;

    while (it != end)
    {
        if (*it == ' ') { it++; continue; }
        if (*it == ',') { it++; continue; }

        if (*it == '}')
        {
            res = 0;
            break;
        }

        if (isalnum(*it))
        {
            element = *it;

            if (GetNextToken(++it, end)) break;

            set.Add(element);
        }

        if (*it == '{')
        {
            Set tmp_set;

            if (ParseSet(it, end, tmp_set)) break;
            if (GetNextToken(++it, end)) break;

            set.Add(tmp_set);
        }
    }

    return res;
}

//====================================================================================================================//

void Set::Clear()
{
    for (auto el : components_)
    {
        delete el;
    }

    components_.clear();
}

//====================================================================================================================//

std::ostream & operator<<(std::ostream &os, const Set &set)
{
    return os << set.ToString();
}

//====================================================================================================================//

template <typename T>
bool Set::ContainGen(const T &component) const
{
    for (auto el : components_)
    {
        if (*el == component)
        {
            return true;
        }
    }

    return false;
}

//====================================================================================================================//

bool Set::Contain(const char &element) const
{
    return ContainGen(element);
}

//====================================================================================================================//

bool Set::Contain(const Set &set) const
{
    return ContainGen(set);
}

//====================================================================================================================//

bool ComponentElement::operator ==(const Set &set) const
{
    (void)set;
    return false;
}

//====================================================================================================================//

std::string ComponentElement::ToString() const
{
    std::string string;

    string += element_;

    return string;
}

//====================================================================================================================//

Component *ComponentElement::CreateCopy() const
{
    return new ComponentElement(element_);
}

//====================================================================================================================//

bool ComponentElement::operator ==(const char &element) const
{
    return element == element_;
}

//====================================================================================================================//

bool ComponentElement::operator ==(const Component &component) const
{
    return component == element_;
}

//====================================================================================================================//

bool ComponentSet::operator ==(const Set &set) const
{
    return set == set_;
}

//====================================================================================================================//

std::string ComponentSet::ToString() const
{
    return set_.ToString();
}

//====================================================================================================================//

Component *ComponentSet::CreateCopy() const
{
    return new ComponentSet(set_);
}

//====================================================================================================================//

bool ComponentSet::operator ==(const char &element) const
{
    (void)element;
    return false;
}

//====================================================================================================================//

bool ComponentSet::operator ==(const Component &component) const
{
    return component == set_;
}

//====================================================================================================================//
