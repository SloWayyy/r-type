/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** updateGame
*/

#include "./updateGame.hpp"
#include <utility>

UpdateGame::UpdateGame(registry &reg) : _reg(reg)
{
}

std::pair<size_t, Position> UpdateGame::updateEntity()
{
    size_t entity = _reg.addEntity();

    auto &pos = _reg.getComponent<Position>();
    pos.emplace_at(entity, 200, 50);
    Position position = pos[entity].value();
    auto pair = std::make_pair(entity, position);
    return pair;
}
