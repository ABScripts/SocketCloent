#include "../include/client.h"
#include "../include/peermanager.h"

#include <iostream>

Client::Client(const std::string &ip, int port)
    : m_Connection { PeerManager::connectToServer(ip, port) }
{
    
}

Client::~Client()
{
    if (m_Connection != nullptr)
    {
        delete m_Connection;
        m_Connection = nullptr;
    }
}

bool Client::sendMessage(const std::string &message)
{
    return m_Connection->sendMessage(message);
}

bool Client::receiveMessage(std::string &message) const
{
    return m_Connection->receiveMessage(message);
}

bool Client::isConnectedToServer() const
{
    return m_Connection->isConnected();
}

bool Client::connectToServer(const std::string &ip, int port)
{
    if (m_Connection != nullptr)
    {
        delete m_Connection;
        m_Connection = nullptr;
    }

    m_Connection = PeerManager::connectToServer(ip, port);

    if (!isConnectedToServer())
    {
        return false;
    }

    return true;
}

bool Client::portIsFree(int port) const
{
    return m_Connection->isAllowedChangingPortTo(port);
}