#include <iostream>

#include "template_method.h"

using namespace std;

int main(int argc, char const *argv[])
{
    StreamRemInteraction stream;
    DatagramRemInteraction datagram;

    stream.ConnectToRemote("111.22.17.10");
    stream.SendDataToRemote(1);
    stream.SendDataToRemote(2);
    stream.SendDataToRemote(3);
    stream.DisconnectFromRemote();

    datagram.ConnectToRemote("172.16.0.222");
    datagram.SendDataToRemote(5);
    datagram.SendDataToRemote(6);
    datagram.SendDataToRemote(7);
    datagram.DisconnectFromRemote();

    (void)argc;
    (void)argv;

    return 0;
}

