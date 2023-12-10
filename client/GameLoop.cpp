/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop.cpp
*/

#include "GameLoop.hpp"

GameLoop::GameLoop(std::size_t port) : _client(port)
{
}

GameLoop::~GameLoop()
{
}

void GameLoop::run()
{
    while (1) {
    }
}