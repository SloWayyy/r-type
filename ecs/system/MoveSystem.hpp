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

        bool isColliding(Sparse_array<Position> &position, Sparse_array<Velocity> &velocity) {
            for (long unsigned int i = 0; i < position.size(); i++) {
                if (position[i] && velocity[i]) {
                    for (long unsigned int j = 0; j < position.size(); j++) {
                        if (position[j] && velocity[j] && i != j) {
                            if (position[i].value().x < position[j].value().x + AVERAGE_SIZE_SPRITE &&
                                position[i].value().x + AVERAGE_SIZE_SPRITE > position[j].value().x &&
                                position[i].value().y < position[j].value().y + AVERAGE_SIZE_SPRITE &&
                                position[i].value().y + AVERAGE_SIZE_SPRITE > position[j].value().y) {
                                _reg._eventManager.addEvent<collision>(i, j);
                                return true;
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

            if (isColliding(position, velocity))
                return;

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
