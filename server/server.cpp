/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"

Server::Server(std::size_t Port)
    : tcpServer(std::make_shared<TCPServer>(Port))
{
    std::thread serverThread([&]() {
        tcpServer->run();
    });
    run();
    serverThread.join();
}

void Server::run()
{
    while (1) {
        if (tcpServer->getNbrClientsConnected() >= 1)
            tcpServer->sendMessageToAllClients("MAR THOMAS");
        std::cout << "Number of clients connected: " << tcpServer->getNbrClientsConnected() << std::endl;
    }   
}
