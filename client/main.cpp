#include <iostream>
#include <unistd.h>
#include <string>

#include "../include/client.h"
#include <thread>

/*
    Types of print messages:
    
    [i] - information
    [~] - something in progress
    [!] - error
*/

const static std::string infoPrint = "[i]";
const static std::string errorPrint = "[!]";
const static std::string progressPrint = "[~]";

const static std::string failureMessageServer = "failure";
const static std::string successMessageServer = "success";

static void out(std::ostream &ostream, const std::string & signPrint, const std::string &message)
{
    ostream << signPrint << " " << message << std::endl;
}

static void info(const std::string &message)
{
    out(std::cout, infoPrint, message);
}

static void error(const std::string &message)
{
    out(std::cerr, errorPrint, message);
}

static void progress(const std::string &message)
{
    out(std::cout, progressPrint, message);
}

void receiveMessagesWorkflow(Client *client)
{
    std::string message;
    while (true)
    {
        usleep(50000);

        bool received = client->receiveMessage(message);

        if (!received)
        {
            error("Error while receiving message.");
        }
        else
        {
            if ( (message.size()-1) == failureMessageServer.size() ||
                 (message.size()-1) == successMessageServer.size()   )
            {
                if (message.compare(0, 7, "failure") == 0 )
                //if (message == failureMessageServer)
                {
                    error("Server rejected to change port or some error occured.");
                }
                //else if (message == successMessageServer)
                else if (message.compare(0, 7, "success") == 0)
                {
                     progress("Changing port...");
                   // std::cout << "WWWWWW!\n";
                    
                    while (!client->connectToServer("127.0.0.1", 50400))
                    {
                        usleep(500000);

                        error("Failed connect to the server");

                        // std::cerr << "Failed connect to the server!";
                    }

                    // Client * newClient = new Client("127.0.0.1", 50400);


                    // if (newClient->isConnectedToServer())
                    // {
                    //     std::cerr << "Client connected!\n";
                    //     client_p = newClient;
                    // }
                    // else 
                    // {
                    //     std::cout << "Failed to connect!\n";
                    // }
                    // bool connected = client.connectToServer("127.0.0.1", 50400);

                    // if (connected)
                    // {
                    info("Port number changed.");
                       //std::cerr << "Port number changed!\n"; 
                    // }
                    // else
                    // {
                    //     std::cerr << "Failed to  change port!\n";
                    // }


                }

              //  std::cout << message << " != " << failureMessageServer << " && " << message << " != " << successMessageServer << "\n";
            }
            else
            {
               // std::cout << "Mesasge size: " << message.size() << "\n";
                std::cout << "Server> " << message << std::endl;
            }
        }
    }
}

int main()
{
    std::cout << failureMessageServer.size();
    Client client("127.0.0.1", 7300);

    if (client.isConnectedToServer())
    {
        info("Connected to the server.");

        std::thread receiveMessageFromServerThread(receiveMessagesWorkflow, &client);

        std::string message;
        while (true)
        {
           usleep(50000);

            std::cout << "> ";
            std::getline(std::cin, message);

            bool sent = client.sendMessage(message);

            if (!sent)
            {
                error("Failed to send message.");
                return -1;
            }
        }   

        // std::cerr << "Waiting for receiving...\n";

       receiveMessageFromServerThread.join();
    }
    else
    {
        error("Failed to connect to the server.");
    }

    info("Client shutted down.");
    // std::cerr << "Closed connection\n";

    usleep(10000000);
}