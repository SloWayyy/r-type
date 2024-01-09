/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ShootSystem
*/

#ifndef SHOOTSYSTEM_HPP_
#define SHOOTSYSTEM_HPP_

#include "../event/shoot.hpp"
#include "../registry/registry.hpp"

class ShootSystem : public ISystem {
public:
    ShootSystem() = delete;
    ShootSystem(registry& reg)
        : _reg(reg) {};
    ~ShootSystem() = default;
    void operator()() override
    {
        for (auto &tmp : _reg._eventManager.getEvent<shoot>()) {
            std::cout << "size " << _reg._eventManager.getEvent<shoot>().size() << std::endl;
            bulletShot(_reg.addEntity(), tmp->_player_x, tmp->_player_y);
        }
    };

private:
    void bulletShot(uint32_t Entity_id, uint32_t player_x, uint32_t player_y)
    {
        auto& sprite = _reg.getComponent<Sprite>();
        auto& position = _reg.getComponent<Position>();
        auto& size = _reg.getComponent<Size>();
        auto& velocity = _reg.getComponent<Velocity>();
        auto& collision = _reg.getComponent<CollisionGroup>();
        collision.emplace_at(Entity_id, 0);
        sprite.emplace_at(Entity_id, 2, 0, 0, 32, 28);
        position.emplace_at(Entity_id, player_x + 32, player_y + 14);
        size.emplace_at(Entity_id, 1, 1);
        velocity.emplace_at(Entity_id, 0, 0, 0, 5, 0);
    }
    registry& _reg;
};

#endif /* !SHOOTSYSTEM_HPP_ */
