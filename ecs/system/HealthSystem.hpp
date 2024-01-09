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
                    if (tmp->_id1 <= 3) {
                        health[tmp->_id1].value().health -= 1;
                        std::cout << "health " << health[tmp->_id1].value().health << std::endl;
                        if (health[tmp->_id1].value().health <= 0)
                            pos[tmp->_id1].value() = std::
                        // if (health[tmp->_id2].value() <= 0)
                        //     _reg.removeEntity(tmp->_id2);
                    }
                    if (tmp->_id2 <= 3) {
                        health[tmp->_id2].value().health -= 1;
                        std::cout << "health2 " << health[tmp->_id2].value().health << std::endl;
                        // if (Health[tmp->_id1].value() <= 0)
                        //     _reg.removeEntity(tmp->_id1);
                    }
                }
            }
        };

    private:
        registry& _reg;
};

#endif /* !HEALTHSYSTEM_HPP_ */
