/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include "../network/tcpClient/tcpClient.hpp"
#include "../network/udp/udp.hpp"
#include "./system/inputSystem.hpp"
#include "./system/networkSystem.hpp"
#include "./system/playerSystem.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <iostream>

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_client port ip" << std::endl;
        return 84;
    }
    registry reg;
    TCPClient tcpClient(std::stoi(av[1]), av[2], reg);
    Udp udpClient(4243, av[2], reg, true);

    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
    sf::Event event;
    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    uint32_t tmp = reg.addEntity();
    reg.addEntity();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();
    sprite.emplace_at(tmp, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    position.emplace_at(tmp, 0, 0);
    size.emplace_at(tmp, 1.5, 1.5);
    sprite.emplace_at(1, "../game/assets/spaceShipBlue.png", sf::IntRect(198, 0, 32, 32));
    position.emplace_at(1, 50, 50);
    size.emplace_at(1, 1.5, 1.5);
    reg.add_system<DrawSystem>(std::ref(window));
    reg.add_system<MoveSystem>();
    reg.add_system<PlayerSystem>();
    reg.add_system<NetworkSystem>(std::ref(udpClient), std::ref(tcpClient));
    // reg.add_system<InputSystem>(std::ref(window));
    // reg.add_system(animeEntity, 32, 198);
    InputSystem inputSystem(reg, window);


    velocity.emplace_at(tmp, 0, 0, 0, 0, 0);
    velocity.emplace_at(1, 0, 0, 0, 0, 0);

    auto current_time = std::chrono::high_resolution_clock::now();
    float refresh_rate = 1.0f / 60.0f;
    float elapsed_time = 0.0f;


    while (window.isOpen()) {
        auto new_time = std::chrono::high_resolution_clock::now();
        auto time_diff = new_time - current_time;
        current_time = new_time;

        float delta_time = std::chrono::duration<float>(time_diff).count();
        elapsed_time += delta_time;


        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        inputSystem();
        if (elapsed_time >= refresh_rate) {
            window.clear();
            reg.run_system();
            window.display();
            elapsed_time = 0.0f;
        }
    }
}
