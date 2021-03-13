#include "../include/server.h"
#include "../../shared/include/peermanager.h"

#include <iostream>

Server::Server(const std::string &ip, int port)
    : m_Connection { PeerManager::openConnection(ip, port) }
{

}

Server::~Server()
{
    delete m_Connection;
}

bool Server::sendMessage(const std::string &message)
{
    return m_Connection->sendMessage(message);
}

bool Server::receiveMessage(std::string &message) const
{
    return m_Connection->receiveMessage(message);
}

bool Server::changePort(int newPort)
{
    return m_Connection->changePort(newPort);
}

bool Server::waitForClient() const
{
    return m_Connection->acceptClient();
}

bool Server::waitForClient(const std::string &rebaseMessage) const
{
    bool rebasing = m_Connection->tellClientToRebase(rebaseMessage);
    bool waiting =  waitForClient();

    return rebasing && waiting;
}

bool Server::openedConnection() const
{
    return m_Connection->isOpened();
}

bool Server::clientDisconnected() const
{
    return m_Connection->isDisconnected();
}

bool Server::portIsFree(int port) const
{
    return m_Connection->isAllowedChangingPortTo(port);
}
