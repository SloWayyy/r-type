/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ShootSystem
*/

#ifndef SHOOTSYSTEM_HPP_
    #define SHOOTSYSTEM_HPP_

    #include "../registry/registry.hpp"
    #include "../event/shoot.hpp"

class ShootSystem : public ISystem {
    public:
        ShootSystem() = delete;
        ShootSystem(registry &reg): _reg(reg) {};
        ~ShootSystem() = default;
        void operator()() override {
            for_each(_reg._eventManager.getEvent<shoot>().begin(), _reg._eventManager.getEvent<shoot>().end(), [this](auto &tmp) {
                auto &position = _reg.getComponent<Position>();
                bulletShot(4, position[tmp->entity_id].value().x, position[tmp->entity_id].value().y);
            });
        };
    private:
        void bulletShot(uint32_t Entity_id, uint32_t player_x, uint32_t player_y) {
            auto& sprite = _reg.getComponent<Sprite>();
            auto& position = _reg.getComponent<Position>();
            auto &size = _reg.getComponent<Size>();
            auto &velocity = _reg.getComponent<Velocity>();
            sprite.emplace_at(4, 2, 0, 0, 32, 28);
            position.emplace_at(4, player_x + 32, player_y + 14);
            size.emplace_at(4, 1, 1);
            velocity.emplace_at(4, 0, 0, 0, 5, 0);
        }
        registry &_reg;
};

#endif /* !SHOOTSYSTEM_HPP_ */
