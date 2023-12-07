/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"

Server::Server(std::size_t Port)
    : tcpServer(std::make_shared<TCPServer>(Port)), udpServer(std::make_shared<UDPSERVER>(Port + 1))
{
}

void Server::run()
{
    
    tcpServer->createSocket();
    tcpServer->run();
}
