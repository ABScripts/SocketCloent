#include <string>

class Connection;

class Client {
    Client();

    public:
        explicit Client(const std::string &ip, int port);
        ~Client();

        bool sendMessage(const std::string &message);
        bool connectToServer(const std::string &ip, int port);

        bool isConnectedToServer() const;
        bool receiveMessage(std::string &message) const;

    private:
        Connection *m_Connection;
};