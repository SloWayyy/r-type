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
#include "../../ecs/event/shoot.hpp"
#include "../../ecs/event/ennemy.hpp"

class PlayerSystem : public ISystem {
    public:
        PlayerSystem() = delete;
        PlayerSystem(registry &reg): _reg(reg) {};
        ~PlayerSystem() = default;
        void operator()() override {
            for_each(_reg._eventManager.getEvent<keyPressed>().begin(), _reg._eventManager.getEvent<keyPressed>().end(), [this](auto &tmp) {
                auto &velocity = _reg.getComponent<Velocity>();
                auto &position = _reg.getComponent<Position>();
                switch(tmp->_key) {
                    case sf::Keyboard::Right:
                        velocity[_reg._player].value().x_speed = 1;
                        velocity[_reg._player].value().y_speed = 0;
                        break;
                    case sf::Keyboard::Left:
                        velocity[_reg._player].value().x_speed = -1;
                        velocity[_reg._player].value().y_speed = 0;
                        break;
                    case sf::Keyboard::Up:
                        velocity[_reg._player].value().x_speed = 0;
                        velocity[_reg._player].value().y_speed = -1;
                        break;
                    case sf::Keyboard::Down:
                        velocity[_reg._player].value().x_speed = 0;
                        velocity[_reg._player].value().y_speed = 1;
                        break;
                    case sf::Keyboard::Space:
                        _reg._eventManager.addEvent<shoot>(position[_reg._player].value().x, position[_reg._player].value().y);
                        break;
                    case sf::Keyboard::X:
                        _reg._eventManager.addEvent<ennemy>(position[_reg._player].value().x, position[_reg._player].value().y);
                        break;
                    default:
                        break;
                }
            });
        };
    private:
        registry &_reg;
};

#endif /* !PLAYERSYSTEM_HPP_ */
