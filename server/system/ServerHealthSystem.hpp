/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** HealthSystem
*/

#ifndef HEALTHSYSTEM_HPP_
    #define HEALTHSYSTEM_HPP_

    #include "../../ecs/registry/registry.hpp"
    #include "../../ecs/event/collision.hpp"
    #include "../../ecs/registry/registry.hpp"
    #include "../event/health.hpp"
    #include <optional>
    #include <iostream>

class HealthSystem : public ISystem {
    public:
        HealthSystem() = delete;
        HealthSystem(registry& reg)
            : _reg(reg) {};
        ~HealthSystem() = default;
        void operator()() override
        {
            if (_reg._eventManager.checkEvent<collision>()) {
                auto& healthVector = _reg.getComponent<Health>();
                auto& pos = _reg.getComponent<Position>();

                for (auto &tmp : _reg._eventManager.getEvent<collision>()) {
                    if (healthVector[tmp->_id1] && pos[tmp->_id1])
                        if (tmp->_id1 <= 3) {
                            healthVector[tmp->_id1].value().health -= 1;
                            auto &velocity = _reg.getComponent<Velocity>();
                            velocity[tmp->_id1].value().x_speed = 0;
                            velocity[tmp->_id1].value().y_speed = 0;
                            _reg._eventManager.addEvent<health>(tmp->_id1);
                        }
                    if (healthVector[tmp->_id2] && pos[tmp->_id2])
                        if (tmp->_id2 <= 3) {
                            healthVector[tmp->_id2].value().health -= 1;
                            auto &velocity = _reg.getComponent<Velocity>();
                            velocity[tmp->_id2].value().x_speed = 0;
                            velocity[tmp->_id2].value().y_speed = 0;
                            _reg._eventManager.addEvent<health>(tmp->_id2);
                        }
                }
            }
        };

    private:
        registry& _reg;
};

#endif /* !HEALTHSYSTEM_HPP_ */
