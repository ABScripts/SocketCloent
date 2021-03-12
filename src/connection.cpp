#include <unistd.h>

#include "../include/connection.h"
#include "../include/network/socket.h"

const unsigned char Connection::m_FlagsArraySize = 2;

using namespace network;

Connection::Connection(const std::string &ip, int port, Action action)
    : m_Flags (m_FlagsArraySize, false)
    , m_Ip (ip)           
    , m_Port (port)
    , m_HoldingSocket (nullptr)
{
    m_Socket = network::TcpSocket::create();

    if (m_Socket->getLastError() != TcpSocket::Error::Error_CreationSocket)     // if opened OK
    {
        if (action == Action::Open)                                             // decide what to do next - open it for clients
        {
            bool isBinded = m_Socket->bind(ip, port);

            if (isBinded)
            {
                bool isListening = m_Socket->listen();   // i am not sure whether to use the previous bool variable 

                if (isListening)
                {
                    m_Flags[Flags::InitialisationStatus] = true;
                    return; 
                }
            }
        }
        else if (action == Action::Connect)                                     // decide what to do next - connect to the server
        {
           bool isConnected = m_Socket->connect(ip, port);      // maybe I shold create a shared bool-var for these results ?

            if (isConnected)
            {
                m_Flags[Flags::ConnectionStatus] = true;
                return;
            }
        }
    }

    m_Ip = "";      // if we failed at whether opening/connecting ./to the socket, mark those
    m_Port = 0;     // fields accordingly - so actually no active ip/port for that connection

    delete m_Socket;

    m_Socket = nullptr;
}

Connection::~Connection()
{
    if (m_Socket != nullptr)
    {
        delete m_Socket;
        m_Socket = nullptr;
    }
}


bool Connection::acceptClient() const
{
    bool accepted = false;
    if (m_Socket != nullptr)
    {
        accepted = m_Socket->accept();
    }

    return accepted;
}

bool Connection::sendMessage(const std::string &message)
{
    bool sent = false;
    if (m_Socket != nullptr)
    {
        if (opened() || connected())
        {
            sent = m_Socket->send(message);
        }
    }

    return sent;
}

bool Connection::receiveMessage(std::string &message) const
{
    bool received = false;
    if (m_Socket != nullptr)
    {
        if (opened() || connected())
        {
            message = m_Socket->receive();

            TcpSocket::Error error = m_Socket->getLastError();

            if (error != TcpSocket::Error::Error_ReceivingMessage)
            {
                received = true;
            }
            
            if (error == TcpSocket::Error::Error_ClientDisconnected)
            {
                m_Flags[Flags::DisconnectionStatus] = true;
            }
        }
    }

    return received;
}

bool Connection::changePort(int newPort)
{
    if (m_Socket != nullptr)
    {
        if (m_Port == newPort)
        {
            //std::cerr << "Exact same port already. Abort...\n";
            return false;
        }

        TcpSocket *newSocket = network::TcpSocket::create();

        if (newSocket->getLastError() != TcpSocket::Error::Error_CreationSocket) // if new socket was created
        {
            bool isBinded = newSocket->bind(m_Ip, newPort);                     // try to bind that one

            if (!isBinded)
            {
                //std::cerr << "Failed to bind new port. Abort...\n";
                // delete newSocket; -> should be added
                return false;
            }
            else                                                        // if we succesfully created a new socket we can close the previous one
            {
                bool listening = newSocket->listen();

                if (!listening)
                {
                   // std::cerr << "Failed to listen for new socket [port changing]!\n";
                    // delete newSocket;
                    return false;
                }

                m_HoldingSocket = m_Socket;                     // save old socket to notify client

                m_Socket = newSocket;                          // point to the new socket
                m_Port = newPort;                                   
            }
        }
        return true;
    }
    return false;
}

bool Connection::tellClientToRebase(const std::string &rebaseMessage) // void ?
{
    if (m_HoldingSocket && m_Socket)
    {
 //   std::cerr << "Started rebasing...\n";
        while ( !m_HoldingSocket->send(rebaseMessage) )             // try to send message to client 
        {
            std::cerr << "Sending message to client...\n";
            usleep(1000000);
        }

        std::cerr << "Message sent!\n";

    // std::cerr << "Started closing old socket...\n";
    //    if (m_HoldingSocket != nullptr)
    //    {
    //        std::cerr << "Try deleting\n";
        delete m_HoldingSocket; // do in the different thread
        m_HoldingSocket = nullptr;
        std::cerr << "Deleted\n";
    // }
        return true;
    }
    return false;
//    bool closed = m_HoldingSocket->close();

//     if (!closed)                                                                // if we can not close the previous socket, 
//     {                                                                           // we should not open a new one
//         std::cerr << "Failed to close old port, invalid descriptor. Abort...\n";

//         // newSocket->close();                         // if invalid descriptor provided for that one - don`t care
//         // try run new thread here to run this socket ???
//         return false;
//     }   
//     else
//     {
       
//          //  std::cerr << "Closed rebasing...\n";
//    } 
     //  std::cerr << "No problems with tellThe...\n";
}

bool Connection::opened() const 
{
    return m_Flags[Flags::InitialisationStatus];
}

bool Connection::connected() const
{
    return m_Flags[Flags::ConnectionStatus];
}

bool Connection::disconnected() const
{
    return m_Flags[Flags::DisconnectionStatus];
}
