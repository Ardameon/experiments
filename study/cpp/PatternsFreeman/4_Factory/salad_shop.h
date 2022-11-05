#ifndef SALAD_SHOP_H
#define SALAD_SHOP_H

#include <iostream>

#include "factory.h"
#include "salad.h"

enum class SaladType
{
    Olivie,
    SeldyPodShuboy
};

/* ================================================================================================================= */

class SaladShop
{
protected:
    IIngredientFactory *_factory;

public:
    SaladShop(IIngredientFactory *factory) : _factory(factory) {}
    virtual ~SaladShop() 
    {
        std::cout << "SaladShop Destroyed" << std::endl;
    }

    virtual ISalad *CreateSalad(SaladType type)
    {
        ISalad *salad = nullptr;

        if (_factory)
        {
            switch (type)
            {
                case SaladType::Olivie:
                    salad = new Olivie(*_factory);
                    break;

                case SaladType::SeldyPodShuboy:
                    salad = new SeldyPodShooboy(*_factory);
                    break;

                default:
                    break;
            }

            if (salad)
            {
                salad->Mix();
                salad->Fill();
                salad->Box();
            }
        }

        return salad;
    }
};

/* ================================================================================================================= */

class MoscowSaladShop : public SaladShop
{
public:
    MoscowSaladShop() : SaladShop(new MoscowIngredientFactory) {}
    virtual ~MoscowSaladShop() 
    {
        if (_factory)
        {
            std::cout << "Moscow SaladShop Destroyed" << std::endl;
            delete _factory;
        }
    }
};


/* ================================================================================================================= */

class NovosibirskSaladShop : public SaladShop
{
public:
    NovosibirskSaladShop() : SaladShop(new NovosibirskIngredientFactory) {}
    virtual ~NovosibirskSaladShop() 
    {
        if (_factory)
        {
            std::cout << "Novosibirsk SaladShop Destroyed" << std::endl;
            delete _factory;
        }
    }
};

/* ================================================================================================================= */

#endif // SALAD_SHOP_H
