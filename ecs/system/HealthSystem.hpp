/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** HealthSystem
*/

#ifndef HEALTHSYSTEM_HPP_
    #define HEALTHSYSTEM_HPP_

    #include "../registry/registry.hpp"
    #include "../event/collision.hpp"
    #include "../registry/registry.hpp"
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
                auto& health = _reg.getComponent<Health>();
                auto& pos = _reg.getComponent<Position>();

                for (auto &tmp : _reg._eventManager.getEvent<collision>()) {
                    std::cout << "collision between " << tmp->_id1 << " and " << tmp->_id2 << std::endl;
                    if (tmp->_id1 <= 3) {
                        health[tmp->_id1].value().health -= 1;
                        if (health[tmp->_id1].value().health <= 0) {
                            std::cout << "first destroying entity " << tmp->_id1 << std::endl;
                            auto& pos = _reg.getComponent<Position>();
                            pos[tmp->_id1] = std::optional<Position>();
                        }
                    }
                    if (tmp->_id2 <= 3) {
                        std::cout << "second destroying entity " << tmp->_id2 << std::endl;
                        health[tmp->_id2].value().health -= 1;
                        if (health[tmp->_id2].value().health <= 0) {
                            auto& pos = _reg.getComponent<Position>();
                            pos[tmp->_id2] = std::optional<Position>();
                        }
                    }
                }
            }
        };

    private:
        registry& _reg;
};

#endif /* !HEALTHSYSTEM_HPP_ */
