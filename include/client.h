#include <string>

class Connection;

class Client {
    Client();

    public:
        explicit Client(const std::string &ip, int port);
        ~Client();

        bool sendMessage(const std::string &message);

        bool receiveMessage(std::string &message) const;

        bool isConnectedToServer() const;

        bool connectToServer(const std::string &ip, int port);

    private:
        Connection *m_Connection;
};