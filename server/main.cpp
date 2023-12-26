/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include "../network/tcpServer/tcpServer.hpp"
#include "../network/udp/udp.hpp"
#include "./system/serverNetworkSystem.hpp"
#include <asio.hpp>
#include <iostream>

int handlingArgs(int ac, char const **av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./server [port] [ip]" << std::endl;
        return -1;
    }
    if (std::isdigit(*av[1]) == 0) {
        std::cerr << "Port must be a number" << std::endl;
        return -1;
    }
}

int main(int ac, char const **av)
{
    if (handlingArgs(ac, av) == -1)
        return -1;

    registry reg;

    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    Udp udpServer(4242, av[2], reg);
    TCPServer tcpServer(std::atoi(av[1]), udpServer.getPort(), av[2]);
    reg.add_system<NetworkSystem>(std::ref(udpServer), std::ref(tcpServer));

    while (1) {
        reg.run_system();
    }
    return 0;
}
