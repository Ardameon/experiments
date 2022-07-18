#ifndef SET_IMPROVED_H
#define SET_IMPROVED_H

#include <iostream>
#include <vector>
#include <string>

class Component;

class Set
{
    std::vector<Component *> components_;
    int size_;

    template <typename T>
    bool ContainGen(const T &component) const;

    int AddGen(const Component &component);
    int RemGen(const Component &component);

    int FromString(std::string &str);
    bool StrSetIsValid(const std::string &str) const;
    int GetNextToken(std::string::iterator &it, std::string::iterator end);
    int ParseSet(std::string::iterator &it, std::string::iterator end, Set &set);
    void Clear();

public:
    enum {eMaxSetSize = 50};

public:
    Set();
    Set(const Set &set);
    Set(const std::string &str);
    Set(const char *str);
    Set(const char &c);
    ~Set();

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

/*===================================================================================================================*/

class Component
{
public:
    Component() {}
    virtual ~Component() {}
    virtual bool operator ==(const Set &set) const = 0;
    virtual bool operator ==(const char &element) const = 0;
    virtual bool operator ==(const Component &component) const = 0;
    virtual std::string ToString() const = 0;
    virtual Component *CreateCopy() const = 0;
};

/*===================================================================================================================*/

class ComponentElement : public Component
{
    char element_;
public:
    ComponentElement(const char &element) : element_(element) {}

    bool operator ==(const Set &set) const;
    bool operator ==(const char &element) const;
    bool operator ==(const Component &component) const;
    std::string ToString() const;
    Component *CreateCopy() const;
};

/*===================================================================================================================*/

class ComponentSet : public Component
{
    friend class Set;
    Set set_;
public:
    ComponentSet(const Set &set) : set_(set) {}

    bool operator ==(const Set &set) const;
    bool operator ==(const char &element) const;
    bool operator ==(const Component &component) const;
    std::string ToString() const;
    Component *CreateCopy() const;
};

#endif // SET_IMPROVED_H
