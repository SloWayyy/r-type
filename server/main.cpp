/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include "../network/udp.hpp"
#include "./system/networkSystem.hpp"
#include "tcp/tcp.hpp"
#include <asio.hpp>
#include <iostream>

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
    reg.addEntity();
    position.emplace_at(test, 50, 50);
    velocity.emplace_at(test, 0, 0, 0, 1, 0);
    reg.add_system<NetworkSystem>(std::ref(udpServer), std::ref(tcpServer));
    reg.add_system<MoveSystem>();

    while (1) {
        reg.run_system();

    }
    return 1;
}
