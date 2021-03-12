#include <iostream>
#include <unistd.h>
#include <string>

#include "../include/client.h"
#include <thread>

const static std::string failureMessageServer = "failure";
const static std::string successMessageServer = "success";

void receiveMessagesWorkflow(Client *client)
{
//    Client &client = *client_p;

    std::string message;
    while (true)
    {
        usleep(50000);

        bool received = client->receiveMessage(message);

        if (!received)
        {
            std::cout << "Error while receiving message\n";
        }
        else
        {
            if (message.size() == 8)
            {
                if (message.compare(0, 7, "failure") == 0 )
                {
                    std::cout << "Server rejected to change port or some error occured\n";
                }
                else if (message.compare(0, 7, "success") == 0)
                {
                    std::cout << "Changing port...\n";
                    std::cout << "WWWWWW!\n";
                    
                    while (!client->connectToServer("127.0.0.1", 50400))
                    {
                        usleep(500000);

                        std::cerr << "Failed connect to the server!";
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
                       std::cerr << "Port number changed!\n"; 
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
                std::cout << "Mesasge size: " << message.size() << "\n";
                std::cout << "Server> " << message << std::endl;
            }
        }
    }
}

int main()
{
    Client client("127.0.0.1", 7300);

    if (client.isConnectedToServer())
    {
        std::cout << "Connected to the server\n";

        std::thread receiveMessageFromServerThread(receiveMessagesWorkflow, &client);

        std::string message;
        while (true)
        {
           // usleep(50000);

            // std::cout << "> ";
            // std::getline(std::cin, message);

            bool sent = client.sendMessage("newPort");
            break;
            // if (!sent)
            // {
            //     std::cerr << "Failed to send message!";
            //     return -1;
            // }
        }   

        std::cerr << "Waiting for receiving...\n";

       receiveMessageFromServerThread.join();
    }
    else
    {
        std::cerr << "Failed to connect to the server!";
    }

    std::cerr << "Closed connection\n";

    usleep(10000000);
}