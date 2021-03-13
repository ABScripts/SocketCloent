#include <iostream>
#include <unistd.h>
#include <string>

#include "../../shared/include/logger.h"
#include "../include/client.h"
#include <thread>

static int portRequest(const std::string &message);

const static std::string failureMessageServer { "failure" };
const static std::string successMessageServer { "success" };
const static unsigned int sleepTime { 500000 };

volatile int newPortNumber = -1;

void receiveMessagesWorkflow(Client *client)
{
    std::string message;
    while (true)
    {
        usleep(sleepTime);

        bool received = client->receiveMessage(message);

        if (!received)
        {
            Logger::error("Error while receiving message.");
        }
        else
        {
            if ( message.size() == failureMessageServer.size() ||
                 message.size() == successMessageServer.size()   )
            {
                if (message == failureMessageServer)
                {
                    Logger::error("Server rejected to change port or some error occured.");
                }
                else if (message == successMessageServer)
                {
                    Logger::progress("Changing port...");
                    
                    while (!client->connectToServer("127.0.0.1", newPortNumber))
                    {
                        usleep(sleepTime);

                        Logger::error("Failed connect to the server");
                    }

                    Logger::info("Port number changed.");
                }
            }
            else
            {
                int port = portRequest(message);
                if (port != -1)
                {
                    std::cout << "We have got port: " << port << std::endl;
                    client->sendMessage(successMessageServer);

                    while (true)  // waiting for server`s confirmation
                    {
                        bool received = client->receiveMessage(message);

                        if ( message.size() == failureMessageServer.size() ||
                             message.size() == successMessageServer.size()   )
                        {
                            if (message == failureMessageServer)
                            {
                                Logger::error("Server can`t change port or some error occured.");
                                break;
                            }
                            else if (message == successMessageServer)
                            {
                                Logger::progress("Changing port...");
                                while (!client->connectToServer("127.0.0.1", port))
                                {
                                    usleep(sleepTime);

                                    Logger::error("Failed connect to the server");
                                }

                                Logger::info("Port number changed.");
                                break;
                            }
                        }
                        else
                        {
                            std::cout << "Server> " << message << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "Server> " << message << std::endl;
                }
            }
        }
    }
}

int main()
{
    Client client("127.0.0.1", 7300);

    if (client.isConnectedToServer())
    {
        Logger::info("Connected to the server.");

        std::thread receiveMessageFromServerThread(receiveMessagesWorkflow, &client);

        std::string message;
        while (true)
        {
           usleep(sleepTime);

            std::cout << "> ";
            std::getline(std::cin, message);

            int portNumber = portRequest(message);
            if (portNumber != -1)
            {
                if (client.portIsFree(portNumber))
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

            bool sent = client.sendMessage(message);

            if (!sent)
            {
                Logger::error("Failed to send message.");
                return -1;
            }
        }   

       receiveMessageFromServerThread.join();
    }
    else
    {
        Logger::error("Failed to connect to the server.");
    }

    Logger::info("Client shutted down.");

    usleep(sleepTime * 100); // test thing
}

static int portRequest(const std::string &message)      // use regexp ?
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
