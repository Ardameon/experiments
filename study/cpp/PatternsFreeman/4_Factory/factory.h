#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <vector>

#include "components.h"

class IIngredientFactory
{
public:
    IIngredientFactory() {}
    virtual ~IIngredientFactory() {}
    virtual Meat *CreateMeat() = 0;
    virtual Fish *CreateFish() = 0;
    virtual Sauce *CreateSauce() = 0;
    virtual std::vector<std::string> CreateVeggiesOlivie() = 0;
    virtual std::vector<std::string> CreateVeggiesSeldy() = 0;
};

/* ================================================================================================================= */

class MoscowIngredientFactory : public IIngredientFactory
{
public:
    virtual Meat *CreateMeat()
    {
        return new Chiken;
    }

    virtual Fish *CreateFish()
    {
        return new Trout;
    }

    virtual Sauce *CreateSauce()
    {
        return new SoureCream;
    }

    virtual std::vector<std::string> CreateVeggiesOlivie()
    {
        return {"GrandPotato", "Eggs", "Carrot", "Onion", "Cucumber"};
    }

    virtual std::vector<std::string> CreateVeggiesSeldy()
    {
        return {"GrandPotato", "Carrot", "BeetRoot"};
    }
};

/* ================================================================================================================= */

class NovosibirskIngredientFactory : public IIngredientFactory
{
public:
    virtual Meat *CreateMeat()
    {
        return new DoctorskayaKolbasa;
    }

    virtual Fish *CreateFish()
    {
        return new Herring;
    }

    virtual Sauce *CreateSauce()
    {
        return new Mayonnaise;
    }

    virtual std::vector<std::string> CreateVeggiesOlivie()
    {
        return {"Potato", "Eggs", "Carrot", "Onion", "Cucumber"};
    }

    virtual std::vector<std::string> CreateVeggiesSeldy()
    {
        return {"Potato", "Carrot", "Beet"};
    }
};

#endif /*  */
