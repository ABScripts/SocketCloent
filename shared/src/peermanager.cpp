#include <exception>
#include <iostream>

#include "../include/peermanager.h"

Connection * PeerManager::getConnection(const std::string &ip, int port, Connection::Action action)
{
    Connection *connection;
    try
    {
        connection = new Connection(ip, port, action);
    }
    catch(const std::bad_alloc&)
    {
        connection = nullptr;
    }

    return connection;
}

Connection *PeerManager::openConnection(const std::string &ip, int port)
{
    return getConnection(ip, port, Connection::Action::Open);
}

Connection *PeerManager::connectToServer(const std::string &ip, int port)
{   
    return getConnection(ip, port, Connection::Action::Connect);
}
