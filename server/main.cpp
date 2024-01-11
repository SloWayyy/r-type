/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include "../ecs/registry/registry.hpp"
#include "../ecs/system/MoveSystem.hpp"
#include "../ecs/system/ShootSystem.hpp"
#include "../network/tcpServer/tcpServer.hpp"
#include "../network/udp/udp.hpp"
#include "./system/EnnemySystem.hpp"
#include "./system/EntityDestroyerSystem.hpp"
#include "./system/ServerShootSystem.hpp"
#include "./system/messageSystem.hpp"
#include "./system/serverEventLoaderSystem.hpp"
#include "./system/serverNetworkSystem.hpp"
#include "./updateGame/updateGame.hpp"
#include <asio.hpp>
#include <iostream>

int handlingArgs(int ac, char const** av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./server [port] [ip]" << std::endl;
        return -1;
    }
    if (std::isdigit(*av[1]) == 0) {
        std::cerr << "Port must be a number" << std::endl;
        return -1;
    }
    return 0;
}

int main(int ac, char const** av)
{
    if (handlingArgs(ac, av) == -1)
        return -1;

    registry reg;
    reg.addAllComponents<Position, Velocity, Size, HitBox, CollisionGroup, Anime, Health, Sprite>();
    UpdateGame updateGame(reg);
    Udp udpServer(4242, av[2], reg, updateGame);
    TCPServer tcpServer(std::atoi(av[1]), udpServer.getPort(), av[2]);
    reg.addEntity();
    reg.addEntity();
    reg.addEntity();
    reg.addEntity();
    reg.add_system<ServerEventLoaderSystem>(std::ref(udpServer));
    reg.add_system<messageSystem>(std::ref(tcpServer));
    reg.add_system<ServerShootSystem>();
    reg.add_system<MoveSystem>();
    reg.add_system<EnnemySystem>();
    reg.add_system<EntityDestroyerSystem>();
    reg.add_system<NetworkSystem>(std::ref(udpServer), std::ref(tcpServer));

    auto current_time = std::chrono::high_resolution_clock::now();
    float refresh_rate = 1.0f / 60.0f;
    float elapsed_time = 0.0f;

    while (1) {
        auto new_time = std::chrono::high_resolution_clock::now();
        auto time_diff = new_time - current_time;
        current_time = new_time;

        float delta_time = std::chrono::duration<float>(time_diff).count();
        elapsed_time += delta_time;

        if (elapsed_time >= refresh_rate) {
            reg.run_system();
            elapsed_time = 0.0f;
        }
    }
    return 0;
}
