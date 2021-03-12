#include <string>
#include <functional>

#include "../include/connection.h"

class PeerManager {
    private:
        static Connection *getConnection(const std::string &ip, int port, Connection::Action action);

    public:
        static Connection *openConnection(const std::string &ip, int port);
        static Connection *connectToServer(const std::string &ip, int port);
};