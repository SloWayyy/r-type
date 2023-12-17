/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#include "server.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include <thread>
#include <typeinfo>
#include <iomanip>

Server::Server(std::size_t PortServerTCP, std::size_t PortServerUDP, std::string ip)
    : _PortServerUDP(PortServerUDP), udpServer(std::make_shared<UDPServer>(PortServerUDP, ip)),
        tcpServer(std::make_shared<TCPServer>(PortServerTCP, udpServer->getPort(), ip))
{
}

void Server::run()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    registry reg;

    reg.addAllComponents<Position, Velocity, Sprite, Size>();

    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto test = reg.addEntity();
    position.emplace_at(test, 50, 50);
    velocity.emplace_at(test, 0, 0, 0, 10, 0);
    reg.add_system(moveEntity);

    while (1) {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds.count() >= 0.2) {
            for (auto &quer : udpServer->_queries) {
                this->udpServer->send(quer.second, quer.first);
            }
            start = std::chrono::system_clock::now();
            reg.run_system();
            udpServer->sendToAll(position[0].value(), static_cast<uint32_t>(0), PacketType::DATA_PACKET);
        }
    }
}
