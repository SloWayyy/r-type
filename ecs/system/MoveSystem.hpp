/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#define AVERAGE_SIZE_SPRITE 32

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../registry/registry.hpp"
    #include "../event/collision.hpp"

class MoveSystem : public ISystem {
    public:
        MoveSystem() = delete;
        MoveSystem(registry &reg): _reg(reg) {};
        ~MoveSystem() = default;

        bool isColliding(Sparse_array<Position> &position, Sparse_array<Velocity> &velocity, Sparse_array<HitBox> &hitbox, Sparse_array<CollisionGroup> &collisionGroup, Sparse_array<Health> &life) {
            for (long unsigned int i = 0; i < DEFAULT_SIZE; i++) {
                if (position[i] && velocity[i] && hitbox[i] && (!life[i] || life[i].value().health > 0)) {
                    for (long unsigned int j = i + 1; j < DEFAULT_SIZE; j++) {
                        if (position[j] && velocity[j] && i != j && hitbox[j] && (!life[j] || life[j].value().health > 0)) {
                            if (collisionGroup[i] && collisionGroup[j])
                                if (collisionGroup[i].value().collisionGroup == collisionGroup[j].value().collisionGroup)
                                    continue;
                            if (position[i].value().x + velocity[i].value().x_speed < position[j].value().x + hitbox[j].value().w &&
                                position[i].value().x + velocity[i].value().x_speed + hitbox[i].value().w > position[j].value().x &&
                                position[i].value().y + velocity[i].value().y_speed < position[j].value().y + hitbox[j].value().h &&
                                position[i].value().y + velocity[i].value().y_speed + hitbox[i].value().h > position[j].value().y) {
                                    _reg._eventManager.addEvent<collision>(i, j);
                                    velocity[i].value().x_speed = 0;
                                    velocity[i].value().y_speed = 0;
                            }
                        }
                    }
                }
            }
            return false;
        }

        void operator()() override {
            auto &position = _reg.getComponent<Position>();
            auto &velocity = _reg.getComponent<Velocity>();
            auto &hitbox = _reg.getComponent<HitBox>();
            auto &collision = _reg.getComponent<CollisionGroup>();
            auto &health = _reg.getComponent<Health>();
            isColliding(position, velocity, hitbox, collision, health);

            for (long unsigned int i = 0; i < position.size(); i++) {
                if (position[i] && velocity[i]) {
                    position[i].value().x += velocity[i].value().x_speed;
                    position[i].value().y += velocity[i].value().y_speed;
                }
            }
        };
    private:
        registry &_reg;
};

#endif /* !SYSTEM_HPP_ */
