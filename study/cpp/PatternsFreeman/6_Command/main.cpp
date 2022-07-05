#include <iostream>

#include "command.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Radio radio;
    TV tv;
    HomeHelper home_helper;

    CommandRadioOn cmd_radio_on(radio);
    CommandRadioOff cmd_radio_off(radio);
    CommandRadioSetStationAtrium cmd_radio_set_atrium(radio);
    CommandRadioSetStationFreeSpace cmd_radio_set_free_space(radio);

    CommandTVOn cmd_tv_on(tv);
    CommandTVOff cmd_tv_off(tv);
    CommandTVPlayNews cmd_tv_news(tv);
    CommandTVPlaySerries cmd_tv_serries(tv);

    home_helper.AddCommand(cmd_tv_on);
    home_helper.AddCommand(cmd_radio_on);
    home_helper.AddCommand(cmd_tv_off);

    home_helper.ExecuteCommands();
    home_helper.UndoLastExecuted();

    home_helper.AddCommand(cmd_tv_off);
    home_helper.AddCommand(cmd_tv_serries);
    home_helper.AddCommand(cmd_radio_off);
    home_helper.AddCommand(cmd_radio_set_free_space);
    home_helper.AddCommand(cmd_tv_news);
    home_helper.AddCommand(cmd_radio_set_atrium);

    home_helper.ExecuteCommands();
    home_helper.UndoLastExecuted();

    (void)argv;
    (void)argc;

    return 0;
}

