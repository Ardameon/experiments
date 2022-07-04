#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

class BaseComponent
{
    std::string _name;

public:
    BaseComponent(const std::string &name) : _name(name) {}
    virtual ~BaseComponent() {}

    const std::string &GetName() const { return _name; }
};

/* ================================================================================================================= */

class Sauce : public BaseComponent
{
public:
    Sauce(const std::string &name) : BaseComponent(name) {}
};

class Mayonnaise : public Sauce
{
public:
    Mayonnaise() : Sauce("Mayonnaise") {}
};

class Caesar : public Sauce
{
public:
    Caesar() : Sauce("Caesar") {}
};


class SoureCream : public Sauce
{
public:
    SoureCream() : Sauce("Soure Cream") {}
};

/* ================================================================================================================= */

class Fish : public BaseComponent
{
public:
    Fish(const std::string &name) : BaseComponent(name) {}
};

class Herring : public Fish
{
public:
    Herring() : Fish("Herring") {}
};

class Trout : public Fish
{
public:
    Trout() : Fish("Trout") {}
};

/* ================================================================================================================= */

class Meat : public BaseComponent
{
public:
    Meat(const std::string &name) : BaseComponent(name) {}
};

class DoctorskayaKolbasa : public Meat
{
public:
    DoctorskayaKolbasa() : Meat("DoctorskayaKolbasa") {}
};

class Chiken : public Meat
{
public:
    Chiken() : Meat("Chiken") {}
};

/* ================================================================================================================= */



#endif // COMPONENTS_H
