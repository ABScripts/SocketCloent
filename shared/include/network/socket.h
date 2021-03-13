#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

#include <iostream>
#include <string>
#include <vector>

namespace network {

    class TcpSocket {
        public:
            static TcpSocket * create(); 

            enum class Error {
                Error_None = 0,
                Error_CreationSocket,
                Error_ClientDisconnected,
                Error_ReceivingMessage
            };

            virtual ~TcpSocket();
            
        protected:
            static constexpr unsigned int m_BufferSize { 4096 };
            
            mutable Error m_LastError;
            char m_Buffer[m_BufferSize];
            sockaddr_in m_DestinationHint;

        protected:
            TcpSocket();

        public:
            virtual bool listen() = 0;
            virtual bool connect(const std::string &ip, int port) = 0; 
            
            virtual bool send(const std::string &string) = 0;
            virtual bool bind(const std::string &ip, int port) = 0;
            
            virtual bool accept() const = 0;
            virtual std::string receive() const = 0;

            virtual bool close() const = 0;

            Error getLastError() const;
    };

}

#endif