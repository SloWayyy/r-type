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

std::vector<std::vector<uint8_t>> UpdateGame::updateEntity() {
    std::vector<std::vector<uint8_t>> entities;

    static int y = 50;
    uint32_t entity_id = _reg.addEntity();
    std::cout << "entity_id: " << entity_id << std::endl;
    entities.resize(4);
    entities[0].resize(sizeof(uint32_t));
    std::memcpy(entities[0].data(), &entity_id, sizeof(uint32_t));

    // Serialize and push Position into entities[1]
    auto &pos = _reg.getComponent<Position>();
    pos.emplace_at(entity_id, 200, y);
    const Position& position = pos[entity_id].value();
    entities[1].resize(sizeof(Position));
    std::memcpy(entities[1].data(), &position, sizeof(Position));

    // Serialize and push Velocity into entities[2]
    auto &velocity = _reg.getComponent<Velocity>();
    velocity.emplace_at(entity_id, 0, 0);
    const Velocity& vel = velocity[entity_id].value();
    entities[2].resize(sizeof(Velocity));
    std::memcpy(entities[2].data(), &vel, sizeof(Velocity));

    // Serialize and push Sprite into entities[3]
    // auto &sprite = _reg.getComponent<Sprite>();
    // sprite.emplace_at(entity_id, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    // const Sprite& spr = sprite[entity_id].value();
    // entities[3].resize(sizeof(Sprite));
    // std::memcpy(entities[3].data(), &spr, sizeof(Sprite));

    // Serialize and push Size into entities[4]
    auto &size = _reg.getComponent<Size>();
    size.emplace_at(entity_id, 1.5, 1.5);
    const Size& sz = size[entity_id].value();
    entities[3].resize(sizeof(Size));
    std::memcpy(entities[3].data(), &sz, sizeof(Size));

    y += 50;
    return entities;
}
