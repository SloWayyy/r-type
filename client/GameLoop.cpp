/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop.cpp
*/

#include "GameLoop.hpp"
#include "../ecs/registry/registry.hpp"
#include "../ecs/system/system.hpp"
#include <SFML/Window.hpp>

GameLoop::GameLoop(std::size_t TCPport, std::size_t UDPport, std::string ip)
    : _TCPclient(TCPport, ip),
    _UDPclient(UDPport, ip),
    _TCPport(TCPport),
    _UDPport(UDPport)
{
    std::thread clientThread([&]() {
        this->_TCPclient.run();
    });
    std::thread serverThread([&]() {
        this->_UDPclient.run();
    });
    this->run();
    clientThread.join();
    serverThread.join();
}

GameLoop::~GameLoop()
{
}

void GameLoop::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
    sf::Event event;
    // Position position = {0, 0};
    // _UDPclient.send(position, 1, DATA_PACKET);
    registry reg;
    reg.addAllComponents<Position, Velocity, Sprite, Size>();
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();
    reg.add_system(drawEntity, std::ref(window));
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        reg.run_system();
        window.display();
    }
        // std::cout << "Game is running" << std::endl;
}
