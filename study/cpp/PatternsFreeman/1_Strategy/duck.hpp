#include <string>
#include <iostream>

class IFlyBehavior
{
public:
    virtual void Fly() = 0;
    virtual ~IFlyBehavior() {};
};

class FlyWithWings : public IFlyBehavior
{
public:
    void Fly()
    {
        std::cout << "Fly with wings\n";
    }
    
};

class FlyNoWay : public IFlyBehavior
{
public:
    void Fly()
    {
        std::cout << "Can't fly\n";
    }
    
};

class FlyWithRocket : public IFlyBehavior
{
public:
    void Fly()
    {
        std::cout << "Fly with rocket\n";
    }
    
};

/* ================================================================================================================== */

class IQuakBehavior
{
public:
    virtual ~IQuakBehavior() {};
    virtual void QuakSound() = 0;
};

class Quak : public IQuakBehavior
{
public:
    void QuakSound()
    {
        std::cout << "Quack\n";
    }    
};

class Skweak : public IQuakBehavior
{
public:
    void QuakSound()
    {
        std::cout << "Skweak\n";
    }    
};

class SilentQuak : public IQuakBehavior
{
public:
    void QuakSound()
    {
        std::cout << "SilentQuak\n";
    }    
};

/* ================================================================================================================== */

class Duck
{
public:
    Duck() : _fly_behavior(NULL), _quak_behavior(NULL) {}
    
    virtual ~Duck()
    {
        if (_fly_behavior) delete _fly_behavior;
        if (_quak_behavior) delete _quak_behavior;
    }

    void PerformFly()
    {
        if (_fly_behavior) _fly_behavior->Fly();
    }

    void PerformQuak()
    {
        if (_quak_behavior) _quak_behavior->QuakSound();
    }

    virtual void Show() = 0;

protected:
    IFlyBehavior *_fly_behavior;
    IQuakBehavior *_quak_behavior;
};

class MellorDuck : public Duck
{
public:
    MellorDuck() : Duck()
    {
        _fly_behavior = new FlyWithWings;
        _quak_behavior = new Quak;
    }

    void Show()
    {
        std::cout << "Mellor Duck\n";
    }
};

class RocketDuck : public Duck
{
public:
    RocketDuck() : Duck()
    {
        _fly_behavior = new FlyWithRocket;
        _quak_behavior = new SilentQuak;
    }

    void Show()
    {
        std::cout << "Rocket Duck\n";
    }
};

class BathDuck : public Duck
{
public:
    BathDuck() : Duck()
    {
        _fly_behavior = new FlyNoWay;
        _quak_behavior = new Skweak;
    }

    void Show()
    {
        std::cout << "Bath Duck\n";
    }
};