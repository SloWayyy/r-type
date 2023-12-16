/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"
#include <thread>
#include <typeinfo>
#include <iomanip>

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
    this->run();
    serverThread.join();
    clientThread.join();
}

void Server::run()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    while (1) {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds.count() >= 0.2) {
            for (auto &quer : udpServer->_queries) {
                this->udpServer->send(quer.second, quer.first);
            }
            start = std::chrono::system_clock::now();
        }
    }
}
