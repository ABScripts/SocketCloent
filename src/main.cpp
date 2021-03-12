#include <iostream>
#include <unistd.h>
#include <thread>

#include "../include/server.h"

static const int newPortTest { 50400 };

void receiveMessagesWorkflow(Server *server_p)
{
    Server &server = *server_p;

    std::string message;
    while (!server.clientDisconnected())
    {
        usleep(50000);

        bool received = server.receiveMessage(message);

        if (!received)
        {
            std::cout << "Error while receiving message\n";
        }
        else
        {
                if (message.compare(0, 7, "newPort") == 0)
                {
                    bool changed = server.changePort(newPortTest);

                    if (!changed)
                    {
                        std::cout << "Failed to change port!\n";

                        server.sendMessage("failure");
                    }
                    else
                    {
                        std::cout << "Port changed! Waiting for client to connect...\n";

                        bool clientConnected  = server.waitForClient("success");

                        std::cout << clientConnected << "\n"; 

                        if (!clientConnected)
                        {
                            std::cerr << "Client failed to connect!";
                            return;
                        }
                    }
                }
    
                std::cout << "Client> " << message << std::endl;
            
        }
    }
}

int main() {
    try
    {
        Server server("127.0.0.1", 7300);

        if (server.openedConnection())
        {
            std::cout << "Server started\nWaiting for client to connect...\n";

            bool clientConnected  = server.waitForClient();
            if (!clientConnected)
            {
                std::cerr << "Client failed to connect!";
                return -1;
            }

            std::thread receiveMessagesFromClientThread(receiveMessagesWorkflow, &server);

            std::cout << "Client connected, waiting for messages...\n";

            std::string message;
            while (!server.clientDisconnected())
            {
                usleep(2000);
                
                std::cout << "> ";
                std::getline(std::cin, message);
                bool sent = server.sendMessage(message);

                if (!sent)
                {
                    std::cerr << "Failed to send message!\n";
                }
            
            }
            std::cerr << "Client disconnected\n";

            receiveMessagesFromClientThread.join();
        }
        else
        {
            std::cout << "Failed to open connection\n";
        }
    }
    catch(...)
    {
        usleep(10000000);
    }

}