#ifndef ADAPTER_H
#define ADAPTER_H

#include <iostream>
#include <list>

/*==================================================================================================*/

class IDuck
{
public:
    IDuck() {}
    virtual ~IDuck() {}
    virtual void Fly() = 0;
    virtual void Quack() = 0;
};

class Duck : public IDuck
{
public:
    Duck() {}

    void Fly()
    {
        std::cout << "I fly as Duck\n";
    }

    void Quack()
    {
        std::cout << "I quack as Duck\n";
    }
};

class Plaine
{
public:
    Plaine() {}

    void Fly()
    {
        std::cout << "I fly fastaaaaa as jetplaine!\n";
    }

    void Rumble()
    {
        std::cout << "My engines are RUMBLING!\n";
    }
};

class PlainAdapter : public IDuck
{
    Plaine &_plaine;

public:
    PlainAdapter(Plaine &plaine) : _plaine(plaine) {}

    void Fly()
    {
        _plaine.Fly();
    }

    void Quack()
    {
        _plaine.Rumble();
    }
};

#endif /* ADAPTER_H */
