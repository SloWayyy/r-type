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
    registry reg;
    TCPClient tcpClient(std::stoi(av[1]), av[2], reg);
    UDPClient udpClient(4243, av[2], reg);

    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
    sf::Event event;
    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    uint32_t tmp = reg.addEntity();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();
    sprite.emplace_at(tmp, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    position.emplace_at(tmp, 0, 0);
    size.emplace_at(tmp, 1.5, 1.5);
    reg.add_system(drawEntity, std::ref(window));
    reg.add_system(network, std::ref(udpClient), std::ref(tcpClient));
    reg.add_system(moveEntity);
    reg.add_system(animeEntity, 32, 198);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                std::cout << "-------------right---------------" << std::endl;
                auto &vel = velocity.emplace_at(tmp, 0, 0, 0, 1, 0);
                udpClient.send(vel.value(), tmp, DATA_PACKET);
                // auto index = reg._typeIndex.at(typeid(Velocity));
                // Packet packet = {4242, PacketType::DATA_PACKET, 0, tmp, index, 845485485124856245};
                // udpClient._queue2.push_back(packet);
            }
        }
        window.clear();
        reg.run_system();
        window.display();
    }
}
