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
        if (elapsed_seconds.count() >= 1) {
            auto now = std::chrono::system_clock::to_time_t(end);
            std::time_t result = std::time(nullptr);
            std::cout << result << " seconds since the Epoch" << std::endl;

            std::cout << "JE SUIS LA !! - Timestamp: " << std::put_time(std::localtime(&now), "%F %T") << std::endl;

            std::string message = "JE SUIS LA ! - Timestamp: ";
            message += std::ctime(&now);
            // this->udpServer->sendToAll(message);
            start = std::chrono::system_clock::now();
        }
    }
}
