/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** updateGame
*/

#include "./updateGame.hpp"
#include <utility>

UpdateGame::UpdateGame(registry& reg)
    : _reg(reg)
{
}

std::vector<std::vector<uint8_t>> UpdateGame::updateEntity()
{
    std::vector<std::vector<uint8_t>> entities;

    static int y = 50;
    uint32_t entity_id = _reg.addEntity();
    entities.resize(8);
    entities[0].resize(sizeof(uint32_t));
    std::memcpy(entities[0].data(), &entity_id, sizeof(uint32_t));

    auto& pos = _reg.getComponent<Position>();
    pos.emplace_at(entity_id, 200, y);
    const Position& position = pos[entity_id].value();
    entities[1].resize(sizeof(Position));
    std::memcpy(entities[1].data(), &position, sizeof(Position));

    auto& velocity = _reg.getComponent<Velocity>();
    velocity.emplace_at(entity_id, 0, 0);
    const Velocity& vel = velocity[entity_id].value();
    entities[2].resize(sizeof(Velocity));
    std::memcpy(entities[2].data(), &vel, sizeof(Velocity));

    auto& size = _reg.getComponent<Size>();
    size.emplace_at(entity_id, 1.5, 1.5);
    const Size& sz = size[entity_id].value();
    entities[3].resize(sizeof(Size));
    std::memcpy(entities[3].data(), &sz, sizeof(Size));

    auto& hitboxs = _reg.getComponent<HitBox>();
    hitboxs.emplace_at(entity_id, 32, 32);
    const HitBox& hitbox = hitboxs[entity_id].value();
    entities[4].resize(sizeof(HitBox));
    std::memcpy(entities[4].data(), &hitbox, sizeof(HitBox));

    auto& collision = _reg.getComponent<CollisionGroup>();
    collision.emplace_at(entity_id, 0);
    const CollisionGroup& col = collision[entity_id].value();
    entities[5].resize(sizeof(CollisionGroup));
    std::memcpy(entities[5].data(), &col, sizeof(CollisionGroup));

    auto& anime = _reg.getComponent<Anime>();
    anime.emplace_at(entity_id, 32, 198, 5);
    const Anime& an = anime[entity_id].value();
    entities[6].resize(sizeof(Anime));
    std::memcpy(entities[6].data(), &an, sizeof(Anime));

    auto& health = _reg.getComponent<Health>();
    health.emplace_at(entity_id, 10);
    const Health& he = health[entity_id].value();
    entities[7].resize(sizeof(Health));
    std::memcpy(entities[7].data(), &he, sizeof(Health));

    y += 80;
    return entities;
}
