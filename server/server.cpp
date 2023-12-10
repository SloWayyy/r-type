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
    }
}
