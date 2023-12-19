/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** inputSystem
*/

#ifndef INPUTSYSTEM_HPP_
#define INPUTSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"


class InputSystem : public ISystem {
    public:
        InputSystem() = delete;
        InputSystem(registry &reg, sf::RenderWindow &window): _reg(reg), _window(window) {};
        ~InputSystem() = default;
        void operator()() override {
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    _reg._events.insert({Event_t::KEY_PRESSED, sf::Keyboard::Right});
                    return;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    _reg._events.insert({Event_t::KEY_PRESSED, sf::Keyboard::Left});
                    return;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    _reg._events.insert({Event_t::KEY_PRESSED, sf::Keyboard::Up});
                    return;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    _reg._events.insert({Event_t::KEY_PRESSED, sf::Keyboard::Down});
                    return;
                }
            }
        };
    private:
        registry &_reg;
        sf::RenderWindow &_window;
};

#endif /* !INPUTSYSTEM_HPP_ */
