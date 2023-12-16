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
#include "server.hpp"
#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"

void network(registry &reg, UDPServer &server, TCPServer &server2)
{
    auto &position = reg.getComponent<Position>();

    for (auto &quer : server._queries) {
        server.send(quer.second, quer.first);
    }
    server.sendToAll(position[0].value(), static_cast<uint32_t>(0), PacketType::DATA_PACKET);

}


bool isDigit(const std::string &port)
{
    return std::all_of(port.begin(), port.end(), ::isdigit);
}

int main(int ac, char const **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_server port ip" << std::endl;
        return FAILURE;
    }
    std::string portStr = av[1];
    if (!isDigit(portStr) || std::stoi(portStr) <= 0) {
        std::cerr << "Error: Port must be a digit" << std::endl;
        return FAILURE;
    }
    // Server server(std::atoi(av[1]), 4242, av[2]);
    // server.run();
    UDPServer udpServer(4242, av[2]);
    TCPServer tcpServer(std::atoi(av[1]), udpServer.getPort(), av[2]);


    registry reg;


    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto test = reg.addEntity();
    position.emplace_at(test, 50, 50);
    velocity.emplace_at(test, 0, 0, 0, 1, 0);
    reg.add_system(moveEntity);
    reg.add_system(network, std::ref(udpServer), std::ref(tcpServer));

    while (1) {
        sleep(1);
        reg.run_system();
      
    }
    
    // std::cout << "UDP server portdsfsdfsdfsfsdf: " << udpServer.getPort() << std::endl;
    return SUCCESS;
}

