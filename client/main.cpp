/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "../ecs/registry/registry.hpp"
#include "../ecs/system/HealthSystem.hpp"
#include "../ecs/system/MoveSystem.hpp"
#include "../ecs/system/ShootSystem.hpp"
#include "../network/tcpClient/tcpClient.hpp"
#include "../network/udp/udp.hpp"
#include "./system/EntityDestroyerSystem.hpp"
#include "./system/animeSystem.hpp"
#include "./system/audioSystem.hpp"
#include "./system/clientNetworkSystem.hpp"
#include "./system/loggerSystem.hpp"
#include "./system/messageSystem.hpp"
#include "./system/playerSystem.hpp"
#include "./system/sfmlSystem.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <iostream>
#include <regex>

bool isNumber(const std::string& number)
{
    return !number.empty() && std::find_if(number.begin(), number.end(), [](unsigned char c) { return !std::isdigit(c); }) == number.end();
}

bool isIp(const std::string& ip)
{
    std::regex pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    return std::regex_match(ip, pattern);
}

int handlingArgs(int ac, char const** av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./server [port] [ip]" << std::endl;
        return -1;
    }
    if (!isNumber(av[1])) {
        std::cerr << "Port must be a number" << std::endl;
        return -1;
    }
    if (std::atoi(av[1]) <= 0 || std::atoi(av[1]) > 65535) {
        std::cerr << "Port must be between 0 and 65535" << std::endl;
        return -1;
    }
    if (!isIp(av[2])) {
        std::cerr << "Ip must be a valid ip" << std::endl;
        return -1;
    }
    return 0;
}

int main(int ac, char const** av)
{
    if (handlingArgs(ac, av) == -1)
        return 84;

    registry reg;
    reg.addAllComponents<Position, Velocity, Size, HitBox, CollisionGroup, Anime, Health, Score, Owner, Sprite>();
    TCPClient tcpClient(std::stoi(av[1]), av[2], reg);
    Udp udpClient(av[2], reg);
    reg.add_system<SfmlSystem>("../game/assets", 800, 600, "R-Type");
    auto& sprite = reg.getComponent<Sprite>();
    reg.addEntity();
    reg.addEntity();
    reg.addEntity();
    reg.addEntity();
    sprite.emplace_at(0, 0, 192, 0, 32, 32);
    sprite.emplace_at(1, 1, 192, 0, 32, 32);
    sprite.emplace_at(2, 1, 192, 0, 32, 32);
    sprite.emplace_at(3, 1, 192, 0, 32, 32);

    reg.add_system<messageSystem>(tcpClient);
    reg.add_system<PlayerSystem>();
    reg.add_system<MoveSystem>();
    reg.add_system<AnimeSystem>();
    reg.add_system<NetworkSystem>(std::ref(udpClient), std::ref(tcpClient));
    reg.add_system<EntityDestroyerSystem>();
    reg.add_system<LoggerSystem>();
    reg.add_system<AudioSystem>();
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
}
