/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** EntityDestroyerSystem
*/

#ifndef ENTITYDESTROYERSYSTEM_HPP_
#define ENTITYDESTROYERSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../ecs/event/collision.hpp"

class EntityDestroyerSystem : public ISystem {
public:
    EntityDestroyerSystem() = delete;
    EntityDestroyerSystem(registry& reg)
        : _reg(reg) {};
    ~EntityDestroyerSystem() = default;
    void operator()() override
    {
        auto& pos = _reg.getComponent<Position>();
        for (uint32_t i = 0; i < DEFAULT_SIZE; i++) {
            if (!pos[i])
                continue;
            if (pos[i].value().x > 1920 || pos[i].value().x < 0 || pos[i].value().y > 1080 || pos[i].value().y < 0) {
                if (i <= 3)
                    continue;
                _reg.removeEntity(i);
            }
        }
        if (_reg._eventManager.checkEvent<collision>()) {
            for (auto &tmp : _reg._eventManager.getEvent<collision>()) {
                if (tmp->_id1 <= 3) {
                    if (tmp->_id2 <= 3)
                        continue;
                    _reg.removeEntity(tmp->_id2);
                    continue;
                } else if (tmp->_id2 <= 3) {
                    _reg.removeEntity(tmp->_id1);
                    continue;
                } else {
                    _reg.removeEntity(tmp->_id1);
                    _reg.removeEntity(tmp->_id2);
                }
            }
        }
    };

private:
    registry& _reg;
};
#endif /* !ENTITYDESTROYERSYSTEM_HPP_ */
