#include <string>
#include <vector>

namespace network {
    class TcpSocket;
}

class Connection {
     enum Action {
        Open = 0, 
        Connect
        };

    enum Flags {
        InitialisationStatus = 0,
        ConnectionStatus,
        DisconnectionStatus
    };

    private:
        static const unsigned char m_FlagsArraySize;

    public:
        // opens/creates new connection
        Connection(const std::string &ip, int port, Action action);
        ~Connection();

        bool acceptClient() const;
        bool sendMessage(const std::string &message);
        bool receiveMessage(std::string &message) const;
        bool changePort(int newPort);

        bool opened() const;
        bool connected() const;
        bool disconnected() const;

        // test stuff
        bool tellClientToRebase(const std::string &rebaseMessage);

    private:
        network::TcpSocket *m_Socket;
        mutable std::vector<bool> m_Flags;

        // test stuff
        std::string m_Ip;
        int m_Port;

        network::TcpSocket *m_HoldingSocket;

    friend class PeerManager;
};