#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "../../include/network/unixsock.h"

namespace network {

    TcpUnixSocket::TcpUnixSocket()
    {
        m_Descriptor = socket(AF_INET, SOCK_STREAM, 0);

        if (m_Descriptor == -1)
        {
            m_LastError = Error::Error_CreationSocket;
        }
    }

    TcpUnixSocket::~TcpUnixSocket()
    {
        close();    
    }

    bool TcpUnixSocket::close() const 
    {
        int res = 0;    
        while ( res = ::close(m_Descriptor) ) // try to close the old one
        {
            if (errno != EINTR && errno != EIO) // those problems could be resolved in repeat call to close
            {
                break;                          // if some else kind of problems occured we could not close the socket
            }
        }

        return operationStatus(res);
    }

    bool TcpUnixSocket::bind(const std::string &ip, int port)
    {
        m_DestinationHint.sin_family = AF_INET;
        m_DestinationHint.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &m_DestinationHint.sin_addr);
 
        int res = ::bind(m_Descriptor, (sockaddr*)&m_DestinationHint, sizeof(sockaddr_in)); 

        return operationStatus(res);         // I could also use    return res ?;
    }

    bool TcpUnixSocket::listen()
    {   
        // Should I wrap around lib methods in namespace to avoid using :: in that case
        // OR
        // Just change my TcpUnixSocket method`s name?

        int res = ::listen(m_Descriptor, SOMAXCONN);

        return operationStatus(res);
    }   

    bool TcpUnixSocket::accept() const
    {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        int tempDescriptor = ::accept(m_Descriptor, (sockaddr*)&client, &clientSize);

        if (tempDescriptor != -1)               // if it is a valid socket descriptor
        {
            close();                            // close the previously opened one
            
            m_Descriptor = tempDescriptor;
        }

        return operationStatus(tempDescriptor);
    }

    bool TcpUnixSocket::connect(const std::string &ip, int port)
    {
        m_DestinationHint.sin_family = AF_INET;
        m_DestinationHint.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &m_DestinationHint.sin_addr);

        int res = ::connect(m_Descriptor, (sockaddr*)&m_DestinationHint, sizeof(sockaddr_in)); 

        return operationStatus(res);
    }   

    bool TcpUnixSocket::send(const std::string &string)
    {
        const char *message = string.c_str();
        size_t size = string.size() + 1;

        int res = ::send(m_Descriptor, message, size, 0);

        return operationStatus(res);
    }
            

    std::string TcpUnixSocket::receive() const 
    {
        // TODO: 
        // Receive all the data from the socket - hint: use timer to accomplish that
        memset((void *)&m_Buffer, 0, m_BufferSize);

        ssize_t bytesReceived = recv(m_Descriptor, (void*)&m_Buffer, m_BufferSize, 0);

        if (bytesReceived == -1)
        {
            m_LastError = Error::Error_ReceivingMessage;
        }
        else if (bytesReceived == 0)
        {
            m_LastError = Error::Error_ClientDisconnected;
        }
        else
        {
            return std::string(m_Buffer, bytesReceived);
        }

        return {};
    }

    bool TcpUnixSocket::operationStatus(int retVal) const
    {
        if (retVal == -1)
        {
            return false;
        }
        return true;
    }

}
