#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <iostream>

class TVPanel
{
public:
    TVPanel() {}
    void On() { std::cout << "TV Panel: on\n"; }
    void Off() { std::cout << "TV Panel: off\n"; }
    void SetInputHDMI() { std::cout << "TV Panel: set input HDMI\n"; }
    void SetInputStreaming() { std::cout << "TV Panel: set input Streaming\n"; }
};

class PopcornPopper
{
public:
    PopcornPopper() {}
    void On() { std::cout << "PopcornPopper: On\n"; }
    void Off() { std::cout << "PopcornPopper: Off\n"; }
    void Pop() { std::cout << "PopcornPopper: Pop UP\n"; }
};

enum class LightMode
{
    eLIGHT_MODE_THEATER,
    eLIGHT_MODE_ROOM
};

class Lights
{
public:
    Lights() {}
    void On() { std::cout << "Lights: On\n"; }
    void Off() { std::cout << "Lights: Off\n"; }
    void SetMode(LightMode mode) { std::cout << "Lights: set mode " << (mode == LightMode::eLIGHT_MODE_THEATER ? "THEATER\n" : "ROOM\n"); }
};

class Tuner
{
public:
    Tuner() {}
    void On() { std::cout << "Tuner: On\n"; }
    void Off() { std::cout << "Tuner: Off\n"; }
    void SetAm() { std::cout << "Tuner: Set AM\n"; }
    void SetFm()  {std::cout << "Tuner: Set FM\n"; }
};

class StreamingPlayer
{
public:
    StreamingPlayer() {}
    void On() { std::cout << "Streaming Player: On\n"; }
    void Off() { std::cout << "Streaming Player: Off\n"; }
    void StreamNetflix() { std::cout << "Streaming Player: Play NETFLIX\n"; }
    void StreamZetflix() { std::cout << "Streaming Player: Play ZETFLIX\n"; }
};

class Amplifier
{
    Tuner           _tuner;
    StreamingPlayer _streaming_player;
public:
    Amplifier() {}
    void On() { std::cout << "Amplifier: On\n"; }
    void SetRadio() { _streaming_player.Off(); _tuner.On(); _tuner.SetFm(); }
    void SetStreaming() { _streaming_player.On(); _streaming_player.StreamZetflix(); _tuner.Off(); }
};

class DVD
{
public:
    DVD() {}
    void On() { std::cout << "DVD: On\n"; }
    void Off() { std::cout << "DVD: Off\n"; }
    void InsertDVD() { std::cout << "DVD: insert DISK\n"; }
    void Play() { std::cout << "DVD: Play\n"; }
};

#endif // SUBSYSTEM_H
