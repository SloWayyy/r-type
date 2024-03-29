/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ShootSystem
*/

#ifndef SHOOTSYSTEM_HPP_
    #define SHOOTSYSTEM_HPP_

    #include "../../ecs/event/shoot.hpp"
    #include "../../ecs/registry/registry.hpp"
    #include "../event/bullet.hpp"

class ServerShootSystem : public ISystem {
    public:
        ServerShootSystem() = delete;
        ServerShootSystem(registry& reg)
            : _reg(reg) {};
        ~ServerShootSystem() = default;
        void operator()() override
        {
            auto& position = _reg.getComponent<Position>();
            for (auto &tmp : _reg._eventManager.getEvent<shoot>()) {
                if (!position[tmp->entity_id])
                    continue;
                uint32_t id = _reg.addEntity();
                bulletShot(id, position[tmp->entity_id].value().x, position[tmp->entity_id].value().y, tmp->entity_id);
                _reg._eventManager.addEvent<bullet>(tmp->entity_id, id);
            }
        };

    private:
        void bulletShot(uint32_t Entity_id, uint32_t player_x, uint32_t player_y, uint32_t player_id)
        {
            auto& sprite = _reg.getComponent<Sprite>();
            auto& position = _reg.getComponent<Position>();
            auto& size = _reg.getComponent<Size>();
            auto& velocity = _reg.getComponent<Velocity>();
            auto& collision = _reg.getComponent<CollisionGroup>();
            auto &hitbox = _reg.getComponent<HitBox>();
            auto &owner = _reg.getComponent<Owner>();
            hitbox.emplace_at(Entity_id, 14, 14);
            collision.emplace_at(Entity_id, 0);
            sprite.emplace_at(Entity_id, 4, 0, 0, 32, 28);
            position.emplace_at(Entity_id, player_x + 32, player_y + 14);
            size.emplace_at(Entity_id, 1, 1);
            velocity.emplace_at(Entity_id, 5, 0);
            owner.emplace_at(Entity_id, player_id);
        }
        registry& _reg;
};

#endif /* !SHOOTSYSTEM_HPP_ */
