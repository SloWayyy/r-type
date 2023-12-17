/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include "udp/udp.hpp"
#include "tcp/tcp.hpp"
#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// #include "GameLoop.hpp"

void network(registry &reg, UDPClient &server, TCPClient &server2)
{
    auto &position = reg.getComponent<Position>();

    while (server._queue.size() > 0) {
        auto packet = server._queue.front();
        server.mtx.lock();
        server._queue.erase(server._queue.begin());
        server.mtx.unlock();
        auto header = packet.first;
        // std::cout << "Packet received: " << header.magic_number << std::endl;
        // std::cout << "Packet received: " << header.packet_type << std::endl;
        // std::cout << "Packet received: " << header.timestamp << std::endl;
        auto body = packet.second;
        position.insert_packet(0, body.c_str());
    }
}

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_client port ip" << std::endl;
        return 84;
    }
    TCPClient tcpClient(std::stoi(av[1]), av[2]);
    UDPClient udpClient(4243, av[2]);

    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
    sf::Event event;
    registry reg;
    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    auto tmp = reg.addEntity();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();
    sprite.emplace_at(tmp, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    position.emplace_at(tmp, 0, 0);
    size.emplace_at(tmp, 1.5, 1.5);
    reg.add_system(drawEntity, std::ref(window));
    reg.add_system(network, std::ref(udpClient), std::ref(tcpClient));
    // reg.add_system(animeEntity, 32, 198);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        reg.run_system();
        window.display();
    }
}
