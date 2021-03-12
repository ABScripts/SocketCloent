#include "../include/server.h"
#include "../include/peermanager.h"

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
    // check for nullptr connection first
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
    std::cerr << "Waiting for...\n";

    bool rebasing = m_Connection->tellClientToRebase(rebaseMessage);

    std::cout << "Waiting next...\n";

    bool waiting =  waitForClient();

    std::cout << "Rebasing: " << rebasing << " waiting: " << waiting << std::endl;

    return rebasing && waiting;
}

bool Server::openedConnection() const
{
    return m_Connection->opened();
}

bool Server::clientDisconnected() const
{
    return m_Connection->disconnected();
}
