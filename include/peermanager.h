#include <string>
#include <functional>

#include "../include/connection.h"

class PeerManager {
    private:
        static Connection *getConnection(const std::string &ip, int port, Connection::Action action);

    public:
        // opens new connection, should be used by servers to create new connection
        static Connection *openConnection(const std::string &ip, int port);
        // try connecting to the existing connection, should be used by clients
        static Connection *connectToServer(const std::string &ip, int port);
};