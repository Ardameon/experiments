#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <list>

class TV
{
public:
    TV() {}

    void On()
    {
        std::cout << "TV on\n";
    }

    void Off()
    {
        std::cout << "TV off\n";
    }

    void SetSerries()
    {
        std::cout << "TV set serries\n";
    }

    void SetNews()
    {
        std::cout << "TV set news\n";
    }
};


enum class RadioStation
{
    eATRIUM     = 102,
    eFREE_SPACE = 177
};

class Radio
{
public:
    Radio() {}

    void On()
    {
        std::cout << "Radio on\n";
    }

    void Off()
    {
        std::cout << "Radio off\n";
    }

    void SetStation(int frequency)
    {
        std::cout << "Radio set station at " << frequency << " FM\n";
    }

    void SetStation(RadioStation station)
    {
        SetStation(static_cast<int>(station));
    }
};

/*==================================================================================================*/

class ICommand
{
public:
    ICommand() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() {}
};

/*==================================================================================================*/

class CommandTV : public ICommand
{
protected:
    TV &_tv;

public:
    CommandTV(TV &tv) : _tv(tv) {}
};

class CommandTVOn : public CommandTV
{
public:
    CommandTVOn(TV &tv) : CommandTV(tv) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.On();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.Off();
    }
};

class CommandTVOff : public CommandTV
{
public:
    CommandTVOff(TV &tv) : CommandTV(tv) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.Off();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.On();
    }
};

class CommandTVPlaySerries : public CommandTV
{
public:
    CommandTVPlaySerries(TV &tv) : CommandTV(tv) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.On();
        _tv.SetSerries();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.Off();
    }
};

class CommandTVPlayNews : public CommandTV
{
public:
    CommandTVPlayNews(TV &tv) : CommandTV(tv) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.On();
        _tv.SetNews();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _tv.Off();
    }
};

/*==================================================================================================*/

class CommandRadio : public ICommand
{
protected:
    Radio &_radio;

public:
    CommandRadio(Radio &radio) : _radio(radio){}
};

class CommandRadioOn : public CommandRadio
{
public:
    CommandRadioOn(Radio &radio) : CommandRadio(radio) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.On();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.Off();
    }
};


class CommandRadioOff : public CommandRadio
{
public:
    CommandRadioOff(Radio &radio) : CommandRadio(radio) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.Off();
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.On();
    }
};

class CommandRadioSetStationAtrium : public CommandRadio
{
public:
    CommandRadioSetStationAtrium(Radio &radio) : CommandRadio(radio) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.On();
        _radio.SetStation(RadioStation::eATRIUM);
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.Off();
    }
};

class CommandRadioSetStationFreeSpace : public CommandRadio
{
public:
    CommandRadioSetStationFreeSpace(Radio &radio) : CommandRadio(radio) {}

    void execute()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.On();
        _radio.SetStation(RadioStation::eFREE_SPACE);
    }

    void undo()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        _radio.Off();
    }
};

/*==================================================================================================*/

class HomeHelper
{
    std::list<ICommand *> _command_list;
    ICommand *_last_executed;

public:
    HomeHelper() : _last_executed(nullptr) {}

    void AddCommand(ICommand &command)
    {
        _command_list.push_front(&command);
    }

    void ExecuteCommands()
    {
        std::cout << "Start commands execution - commands: " << _command_list.size() << std::endl;

        while (!_command_list.empty())
        {
            _last_executed = _command_list.back();
            _last_executed->execute();

            _command_list.pop_back();
        }

        std::cout << "Stop commands execution" << std::endl;
    }

    void UndoLastExecuted()
    {
        if (_last_executed)
        {
            _last_executed->undo();
            _last_executed = nullptr;
        }
    }
};

/*==================================================================================================*/

#endif /* COMMAND_H */
