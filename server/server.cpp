/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"

Server::Server(std::size_t PortServerTCP, std::size_t PortServerUDP, std::string ip)
    : _PortServerUDP(PortServerUDP), udpServer(std::make_shared<UDPServer>(PortServerUDP, ip)),
        tcpServer(std::make_shared<TCPServer>(PortServerTCP, udpServer->getPort(), ip))
{
    std::thread serverThread([&]() {
        tcpServer->run();
    });
    std::thread clientThread([&]() {
        udpServer->run();
    });
    serverThread.join();
    clientThread.join();
}

void Server::run()
{
    while (1) {
    }
}
