#include <iostream>
#include <unistd.h>
#include <thread>

#include "../include/server.h"
#include "../include/logger.h"

const static unsigned int sleepTime { 500000 };

static const int newPortTest { 50400 };

static int portRequest(const std::string &message)
{
    if (message.size() >= 10 && message.size() <= 15)
    {
        if (message.compare(0, 9, "NewPort-<") == 0)
        {
            std::string number;
            number.reserve();
            for (unsigned int i = 9; i < message.size(); ++i)
            {
                if (message[i] == '>')
                {
                    return std::stoi(number);
                }
                number += message[i];
            }
        }
    }

    return -1;
}

void receiveMessagesWorkflow(Server *server_p)
{
    Server &server = *server_p;
    unsigned int newPortNumber = -1;

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
                newPortNumber = portRequest(message);
                if (newPortNumber != -1)
                {
                    Logger::info("Port requested: " + std::to_string(newPortNumber));
                    bool changed = server.changePort(newPortNumber);

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