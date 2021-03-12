#include <iostream>
#include <unistd.h>
#include <thread>

#include "../include/server.h"
#include "../include/logger.h"

const static unsigned int sleepTime { 500000 };

static const int newPortTest { 50400 };

void receiveMessagesWorkflow(Server *server_p)
{
    Server &server = *server_p;

    std::string message;
    while (!server.clientDisconnected())
    {
        usleep(sleepTime);

        bool received = server.receiveMessage(message);

        if (!received)
        {
            Logger::error("Error while receiving message");
        }
        else
        {
                if (message.compare(0, 7, "newPort") == 0)
                {
                    bool changed = server.changePort(newPortTest);

                    if (!changed)
                    {
                        Logger::error("Failed to change port!");
                        server.sendMessage("failure");
                    }
                    else
                    {
                        Logger::progress("Port changed! Waiting for client to connect...");

                        bool clientConnected  = server.waitForClient("success");

                        std::cout << clientConnected << "\n"; 

                        if (!clientConnected)
                        {
                            Logger::error("Client failed to connect!");
                            return;
                        }
                    }
                }
    
                std::cout << "Client> " << message << std::endl;
            
        }
    }
}

int main() {
    
        Server server("127.0.0.1", 7300);

        if (server.openedConnection())
        {
            Logger::info("Server started");
            Logger::progress("Waiting for client to connect...");

            bool clientConnected  = server.waitForClient();
            if (!clientConnected)
            {
                Logger::error("Client failed to connect.");
                return -1;
            }

            std::thread receiveMessagesFromClientThread(receiveMessagesWorkflow, &server);

            Logger::progress("Client connected, waiting for messages...");

            std::string message;
            while (!server.clientDisconnected())
            {
                usleep(sleepTime);
                
                std::cout << "> ";
                std::getline(std::cin, message);
                bool sent = server.sendMessage(message);

                if (!sent)
                {
                    Logger::error("Failed to send message.");
                }
            }

            Logger::error("Client disconnected");

            receiveMessagesFromClientThread.join();
        }
        else
        {
            Logger::error("Failed to open connection");
        }

    usleep(sleepTime * 100);
}