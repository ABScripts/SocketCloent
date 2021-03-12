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
    return m_Connection->connected();
}

bool Client::connectToServer(const std::string &ip, int port)
{
    std::cerr << "Deleting old conncetion!\n";

    if (m_Connection != nullptr)
    {
        delete m_Connection;
        m_Connection = nullptr;
        std::cerr << "Old connection deleted!\n";
    }
    else 
    {
        std::cerr << "Failed to delete old connection!";
    }

    m_Connection = PeerManager::connectToServer(ip, port);

    if (!isConnectedToServer())
    {
      std::cerr << "Failed cnnect to the server!\n";
        return false;
    }

    std::cerr << "Connected to the server!\n";
    return true;
}