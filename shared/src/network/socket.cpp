#include <exception>
#include <string.h>

#include "../../include/network/socket.h"
#ifdef __linux__
    #include "../../include/network/unixsock.h"
#elif _WIN32
    #include "../../include/network/winsock.h"
#endif 

namespace network {

    TcpSocket* TcpSocket::create() 
    {
        using namespace network;

        TcpSocket *socket;
        try
        {
            #ifdef __linux__
                socket = new TcpUnixSocket();
            #elif _WIN32
                socket = new WinSocket();
            #endif 
        }
        catch(const std::bad_alloc& _)
        {
            socket = nullptr;
        }

        return socket;
    }

    TcpSocket::TcpSocket() 
        : m_LastError(Error::Error_None)
    {
        memset(m_Buffer, 0, m_BufferSize);
        memset(&m_DestinationHint, 0, sizeof(sockaddr_in));
    }

    TcpSocket::~TcpSocket()
    {

    }

    TcpSocket::Error TcpSocket::getLastError() const
    {
        return m_LastError;
    }

}