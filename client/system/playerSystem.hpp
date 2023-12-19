/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** inputSystem
*/

#ifndef PLAYERSYSTEM_HPP_
#define PLAYERSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"


class PlayerSystem : public ISystem {
    public:
        PlayerSystem() = delete;
        PlayerSystem(registry &reg, u_int8_t playerEntity): _reg(reg),  _player(playerEntity){};
        ~PlayerSystem() = default;
        void operator()() override {
            Sparse_array<Velocity> &velocity = _reg.getComponent<Velocity>();
            if (_reg._events.find(Event_t::KEY_PRESSED) == _reg._events.end())
                return;
            try
            {
                auto &tmp = _reg._events.at(Event_t::KEY_PRESSED);
                if (tmp == sf::Keyboard::Right) {
                    velocity[_player].value().x_speed = 1;
                    velocity[_player].value().y_speed = 0;
                }
                if (tmp == sf::Keyboard::Left) {
                    velocity[_player].value().x_speed = -1;
                    velocity[_player].value().y_speed = 0;
                }
                if (tmp == sf::Keyboard::Up) {
                    velocity[_player].value().x_speed = 0;
                    velocity[_player].value().y_speed = -1;
                }
                if (tmp == sf::Keyboard::Down) {
                    velocity[_player].value().x_speed = 0;
                    velocity[_player].value().y_speed = 1;
                }
                // _reg._events.erase(Event_t::KEY_PRESSED);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        };
    private:
        registry &_reg;
        u_int8_t _player;
};

#endif /* !PLAYERSYSTEM_HPP_ */
