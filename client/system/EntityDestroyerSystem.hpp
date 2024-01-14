/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** EntityDestroyerSystem
*/

#ifndef ENTITYDESTROYERSYSTEM_HPP_
    #define ENTITYDESTROYERSYSTEM_HPP_

    #include "../../ecs/event/collision.hpp"
    #include "../../ecs/event/destroyEntity.hpp"
    #include "../../ecs/registry/registry.hpp"
    #include "../../ecs/system/ISystem.hpp"

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
                if (pos[i].value().x > 1080 || pos[i].value().x < -30 || pos[i].value().y > 1920 || pos[i].value().y < 0) {
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
