#include "socket.h"

namespace network {

    class TcpUnixSocket : public TcpSocket {
        public:
            TcpUnixSocket();
            ~TcpUnixSocket();

            virtual bool listen() override;
            virtual bool connect(const std::string &ip, int port) override; 
            
            virtual bool send(const std::string &string) override;
            virtual bool bind(const std::string &ip, int port) override;
            
            virtual bool accept() const override;
            virtual std::string receive() const override;

            virtual bool close() const override;

        private:
            bool operationStatus(int retVal) const;

        private:
            // used mutable to make it possible to use that field in accept method
            mutable int m_Descriptor; 
    };

}
