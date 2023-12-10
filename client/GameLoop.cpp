/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop.cpp
*/

#include "GameLoop.hpp"

GameLoop::GameLoop(std::size_t port) : _TCPclient(port)
{
    std::thread clientThread([&]() {
        this->_TCPclient.run();
    });
    this->run();
    clientThread.join();
}

GameLoop::~GameLoop()
{
}

void GameLoop::run()
{
    while (1) {
    }
}
