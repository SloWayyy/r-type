/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** EnnemySystem
*/

#ifndef ENNEMYSYSTEM_HPP_
    #define ENNEMYSYSTEM_HPP_

    #include "../../ecs/registry/registry.hpp"
    #include "../event/spawnedEnnemy.hpp"
    #include <random>

class EnnemySystem : public ISystem {
    public:
        EnnemySystem() = delete;
        EnnemySystem(registry &reg): _reg(reg) {};
        ~EnnemySystem() = default;
        void operator()() override {
            if (inside_clock < target_clock) {
                inside_clock++;
                return;
            }
            auto& velocity = _reg.getComponent<Velocity>();
            if (!(velocity[0] && velocity[1] && velocity[2] && velocity[3]))
                return;
            uint32_t Entity_id = _reg.addEntity();
            CreateEnnemy(Entity_id);
            _reg._eventManager.addEvent<spawnedEnnemy>(Entity_id);
            inside_clock = 0;
        };
    private:
        void CreateEnnemy(uint32_t Entity_id) {
            auto &velocity = _reg.getComponent<Velocity>();
            auto& sprite = _reg.getComponent<Sprite>();
            auto& position = _reg.getComponent<Position>();
            auto &size = _reg.getComponent<Size>();
            auto &collision = _reg.getComponent<CollisionGroup>();
            auto &hitbox = _reg.getComponent<HitBox>();
            hitbox.emplace_at(Entity_id, 45, 23);
            collision.emplace_at(Entity_id, 3);
            sprite.emplace_at(Entity_id, 3, 0, 0, 576, 430);
            position.emplace_at(Entity_id, 750, (rand() % 750));
            size.emplace_at(Entity_id, 1, 1);
            velocity.emplace_at(Entity_id, 0, 0, 0, -3, 0);
        }
        registry &_reg;
        uint32_t inside_clock = 0;
        uint32_t target_clock = 60;
};

#endif /* !ENNEMYSYSTEM_HPP_ */
