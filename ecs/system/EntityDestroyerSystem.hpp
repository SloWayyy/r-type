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
    };

private:
    registry& _reg;
};
#endif /* !ENTITYDESTROYERSYSTEM_HPP_ */
