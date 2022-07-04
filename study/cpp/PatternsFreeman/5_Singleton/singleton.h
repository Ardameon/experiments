#ifndef SINGLETONE_H
#define SINGLETONE_H

#include <iostream>

class Singletone
{
    Singletone()
    {
        std::cout << __PRETTY_FUNCTION__ << " invoked\n";
    }

public:
    static Singletone *GetInstance()
    {
        static Singletone *_instance = new Singletone;

//        if (!_instance)
//        {
//            _instance = new Singletone;
//        }

        return _instance;
    }

    ~Singletone()
    {
        std::cout << __PRETTY_FUNCTION__ << " invoked\n";
    }
};

#endif /* SINGLETONE_H */
