/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include <iostream>
#include <asio.hpp>
#include "tcp/tcp.hpp"
#include "udp/udp.hpp"
#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"

void network(registry &reg, UDPServer &serverUDP, TCPServer &serverTCP)
{
    auto &position = reg.getComponent<Position>();

    serverUDP.mtxSendPacket.lock();
    // verif si les clients ont tous repondu au packet sionn on renvoi
    serverUDP.mtxSendPacket.unlock();
    serverUDP.mtxQueue.lock();
    if (serverUDP._queue.size() > 0) {
        std::cout << "Queue, il y a des choses a traiter" << std::endl;
        // traiter les packets
        // ajouter dans la queueSendPacket aussi
        serverUDP.saveData();
    }
    serverUDP.mtxQueue.unlock();
}

bool isDigit(const std::string &port)
{
    return std::all_of(port.begin(), port.end(), ::isdigit);
}

int main(int ac, char const **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_server port ip" << std::endl;
        return -1;
    }
    std::string portStr = av[1];
    if (!isDigit(portStr) || std::stoi(portStr) <= 0) {
        std::cerr << "Error: Port must be a digit" << std::endl;
        return -1;
    }
    registry reg;
    UDPServer udpServer(4242, av[2], reg);
    TCPServer tcpServer(std::atoi(av[1]), udpServer.getPort(), av[2]);

    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto test = reg.addEntity();
    position.emplace_at(test, 50, 50);
    velocity.emplace_at(test, 0, 0, 0, 1, 0);
    reg.add_system(network, std::ref(udpServer), std::ref(tcpServer));
    reg.add_system(moveEntity);

    while (1) {
        usleep(50000);
        reg.run_system();

    }
    return 1;
}
