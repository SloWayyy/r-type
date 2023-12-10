/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"

Server::Server(std::size_t PortServerTCP, std::size_t PortServerUDP)
    : _PortServerUDP(PortServerUDP), udpServer(std::make_shared<UDPServer>(PortServerUDP)), tcpServer(std::make_shared<TCPServer>(PortServerTCP, udpServer->getPort()))
{
    std::thread serverThread([&]() {
        tcpServer->run();
    });
    std::thread clientThread([&]() {
        udpServer->run();
    });
    run();
    serverThread.join();
    clientThread.join();
}

void Server::run()
{
    while (1) {
    }
}
