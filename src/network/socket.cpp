#include <exception>
// #include <string.h>

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
            std::cerr << "Magic\n";
                socket = new TcpUnixSocket();
            #elif _WIN32
                socket = new WinSocket();
            #endif 
        }
        catch(const std::bad_alloc& _)
        {
            std::cerr << "Bad alloc\n";
            socket = nullptr;
        }
        std::cerr << "Returnung shit...\n";
        return socket;
    }

    TcpSocket::TcpSocket() 
        : m_LastError(Error::Error_None)
    {
        std::cerr << "Making socket...\n";
        // memset(m_Buffer, 0, m_BufferSize);
        // memset(&m_DestinationHint, 0, sizeof(sockaddr_in));
        std::cerr << "Made socket for TcpSocket...\n";
    }

    TcpSocket::~TcpSocket()
    {

    }

    TcpSocket::Error TcpSocket::getLastError() const
    {
        return m_LastError;
    }

}