#ifndef ADAPTER_H
#define ADAPTER_H

#include <iostream>

#include "subsystem.h"

/*==================================================================================================*/

class HomeTheaterFacade
{
    TVPanel         _tv;
    PopcornPopper   _popcorn;
    Amplifier       _amplifier;
    DVD             _dvd_player;
    Lights          _lights;

public:
    HomeTheaterFacade() {}

    void PlayStreaming()
    {
        std::cout << "=== Home theater: Play Streaming ======================================\n";

        _dvd_player.Off();
        _popcorn.Off();
        _tv.On();
        _tv.SetInputStreaming();
        _lights.On();
        _lights.SetMode(LightMode::eLIGHT_MODE_ROOM);
        _amplifier.On();
        _amplifier.SetStreaming();
    }

    void PlayRadio()
    {
        std::cout << "=== Home theater: Radio ======================================\n";
        _tv.Off();
        _lights.Off();
        _dvd_player.Off();
        _popcorn.Off();
        _amplifier.On();
        _amplifier.SetRadio();
    }

    void PlayDVD()
    {
        std::cout << "=== Home theater: Play DVD ======================================\n";

        _tv.On();
        _lights.On();
        _lights.SetMode(LightMode::eLIGHT_MODE_THEATER);
        _popcorn.On();
        _popcorn.Pop();
        _dvd_player.On();
        _dvd_player.InsertDVD();
        _amplifier.On();
    }
};

#endif /* ADAPTER_H */
