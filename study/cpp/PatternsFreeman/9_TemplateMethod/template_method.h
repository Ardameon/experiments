#ifndef TEMPLATE_METHOD_H
#define TEMPLATE_METHOD_H

#include <iostream>
#include <string>
#include <sstream>

/*====================================================================================================================*/

class Connection
{
protected:
    std::string _address;

public:
    Connection(const std::string &address) : _address(address) {}
    virtual ~Connection() {}

    void Open()
    {
        std::cout << "Connection " << ToString() << " openned\n";
    }

    virtual void Close()
    {
        std::cout << "Connection " << ToString() << " closed\n";
    }

    virtual void SendData(int data) = 0;
    virtual std::string ToString() = 0;
};

/*====================================================================================================================*/

class TCPConnection : public Connection
{
public:
    TCPConnection(const std::string &address) : Connection(address) {}

    void SendData(int data)
    {
        std::cout << "Send stream data " << data << " over " << ToString() << std::endl;
    }

    std::string ToString()
    {
        std::stringstream ss;

        ss << "<TCP:" << _address << ">";

        return ss.str();
    }
};

/*====================================================================================================================*/

class UDPConnection : public Connection
{
public:
    UDPConnection(const std::string &address) : Connection(address) {}

    void SendData(int data)
    {
        std::cout << "Send datagram data " << data << " over " << ToString() << std::endl;
    }

    std::string ToString()
    {
        std::stringstream ss;

        ss << "<UDP:" << _address << ">";

        return ss.str();
    }
};

/*====================================================================================================================*/

class RemoteInteraction
{
protected:
    Connection *_conn;

    virtual void ConnectionCreated()
    {
        std::cout << "Remote connection " << _conn->ToString() << " created\n";
    }

    virtual Connection *CreateConnection(const std::string &address) = 0;

public:
    RemoteInteraction() : _conn(nullptr) {}

    virtual ~RemoteInteraction()
    {
        if (_conn)
        {
            delete _conn;
        }
    }

    void ConnectToRemote(const std::string &address)
    {
        _conn = CreateConnection(address);

        if (_conn)
        {
            ConnectionCreated();
        }
    }

    void DisconnectFromRemote(void)
    {
        _conn->Close();
    }

    void SendDataToRemote(int data)
    {
        _conn->SendData(data);
    }
};

/*====================================================================================================================*/

class StreamRemInteraction : public RemoteInteraction
{
public:
    StreamRemInteraction() : RemoteInteraction() {}

protected:
    Connection *CreateConnection(const std::string &address)
    {
        std::cout << "Create stream connection to remote\n";

        return new TCPConnection(address);
    }

    void ConnectionCreated()
    {
        RemoteInteraction::ConnectionCreated();
        std::cout << "Stream connection created\n";
    }
};

/*====================================================================================================================*/

class DatagramRemInteraction : public RemoteInteraction
{
public:
    DatagramRemInteraction() : RemoteInteraction() {}

protected:
    Connection *CreateConnection(const std::string &address)
    {
        std::cout << "Create datagram connection to remote\n";

        return new UDPConnection(address);
    }

    void ConnectionCreated()
    {
        RemoteInteraction::ConnectionCreated();
        std::cout << "Datagram connection created\n";
    }
};

#endif /* TEMPLATE_METHOD_H */
