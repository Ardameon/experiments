#ifndef SALAD_H
#define SALAD_H

#include <vector>
#include <string>
#include <iostream>

#include "components.h"
#include "factory.h"

/* ================================================================================================================= */

class ISalad
{
protected:
    std::string _name;
    std::vector<std::string> _veggies;
    Sauce *_sauce;
    Meat  *_meat;
    Fish  *_fish;

public:
    ISalad(const std::string &name) : _name(name),
        _sauce(nullptr), _meat(nullptr), _fish(nullptr) {}

    virtual void PrepareComponents()
    {
        using std::cout;
        using std::endl;

        cout << _name << ": Prepare components:\n";

        if (_meat)
            cout << "\tMeat: " << _meat->GetName() << endl;

        if (_sauce)
            cout << "\tSauce: " << _sauce->GetName() << endl;

        if (_fish)
            cout << "\tFish: " << _fish->GetName() << endl;

        cout << "\tVeggies: ";
        for (auto &veggie : _veggies)
        {
            cout << veggie << " ";
        }
        cout << endl;
    }

    virtual void Mix()               = 0;
    virtual void Fill()              = 0;
    virtual void Box()               = 0;

    const std::string &GetInfo() const { return _name; }
};

/* ================================================================================================================= */

class Olivie : public ISalad
{
    IIngredientFactory &_factory;

public:
    Olivie(IIngredientFactory &factory) : ISalad("Olivie"), _factory(factory)
    {
        PrepareComponents();
    }

    void PrepareComponents()
    {
        _meat    = _factory.CreateMeat();
        _veggies = _factory.CreateVeggiesOlivie();
        _sauce   = _factory.CreateSauce();

        ISalad::PrepareComponents();
    }

    void Mix()
    {
        std::cout << _name << ": mix all\n";
    }

    void Fill()
    {
        std::cout << _name << ": fill with sauce\n";
    }

    void Box()
    {
        std::cout << _name << ": box in round can\n";
    }
};

/* ================================================================================================================= */

class SeldyPodShooboy : public ISalad
{
    IIngredientFactory &_factory;

public:
    SeldyPodShooboy(IIngredientFactory &factory) : ISalad("SeldyPodShooboy"), _factory(factory)
    {
        PrepareComponents();
    }

    void PrepareComponents()
    {
        _fish    = _factory.CreateFish();
        _veggies = _factory.CreateVeggiesSeldy();
        _sauce   = _factory.CreateSauce();

        ISalad::PrepareComponents();
    }

    void Mix()
    {
        std::cout << _name << ": mix all veggies and fish\n";
    }

    void Fill()
    {
        std::cout << _name << ": cover by sauce\n";
    }

    void Box()
    {
        std::cout << _name << ": box in square can\n";
    }
};

/* ================================================================================================================= */


#endif // SALAD_H
