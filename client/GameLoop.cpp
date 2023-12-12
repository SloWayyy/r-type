/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop.cpp
*/

#include "GameLoop.hpp"

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
    while (1) {
    }
}
