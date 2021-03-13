#include <iostream>
#include <unistd.h>
#include <thread>

#include "../include/server.h"
#include "../../shared/include/logger.h"

const static std::string failureMessageServer { "failure" };
const static std::string successMessageServer { "success" };
const static unsigned int sleepTime { 500000 };

volatile  int newPortNumber = -1;

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

void changePortFlow(Server *server, int portNumber)
{
    bool changed = server->changePort(portNumber);

    if (!changed)
    {
        Logger::error("Failed to change port!");
        server->sendMessage(failureMessageServer);
    }
    else
    {
        Logger::progress("Port changed! Waiting for client to connect...");

        bool clientConnected  = server->waitForClient(successMessageServer); 

        if (!clientConnected)
        {
            Logger::error("Client failed to connect!");
            return;
        }

        Logger::info("Client connected to changed port.");
    }
}

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
            if (message.size() == failureMessageServer.size() ||
                message.size() == successMessageServer.size()  )
            {
                if (message == failureMessageServer)
                {
                    Logger::info("Client rejected to change port.");
                    continue;
                }
                else if (message == successMessageServer)
                {
                    changePortFlow(server_p, newPortNumber);
                }
            }
            else
            {
                int port = portRequest(message);
                if (port != -1)
                {
                    Logger::info("Port requested: " + std::to_string(port));

                    changePortFlow(server_p, port);
                }
                else
                {
                    std::cout << "Client> " << message << std::endl;
                }
            }
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

            int portNumber = portRequest(message);
            if (portNumber != -1)
            {
                if (server.portIsFree(portNumber))
                {
                    newPortNumber = portNumber;
                    std::cout << portNumber << " is free\n";
                }
                else
                {
                    Logger::error("Port #" + std::to_string(portNumber) + " is already taken.");
                    continue;
                }
            }

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
