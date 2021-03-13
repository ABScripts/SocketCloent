#include <string>

class Connection;

class Server {
        Server();

    public:
        explicit Server(const std::string &ip, int port);
        ~Server();

        bool waitForClient() const;
        bool sendMessage(const std::string &message);
        bool receiveMessage(std::string &message) const;

        bool changePort(int newPort);
        bool waitForClient(const std::string &rebaseMessage) const; 

        bool openedConnection() const;
        bool clientDisconnected() const;

        bool portIsFree(int port) const;

    private:
        Connection *m_Connection;
};