/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../registry/registry.hpp"

    void animeEntity(registry &reg, size_t incrementLeft, size_t maxWidth);

class MoveSystem : public ISystem {
    public:
        MoveSystem() = delete;
        MoveSystem(registry &reg): _reg(reg) {};
        ~MoveSystem() = default;
        void operator()() override {
            auto &position = _reg.getComponent<Position>();
            auto &velocity = _reg.getComponent<Velocity>();
            for (long unsigned int i = 0; i < position.size(); i++) {
                if (position[i] && velocity[i]) {
                    position[i].value().x += velocity[i].value().x_speed;
                    position[i].value().y += velocity[i].value().y_speed;
                }
            }
        };
    private:
        registry &_reg;
};


class DrawSystem : public ISystem {
    public:
        DrawSystem() = delete;
        DrawSystem(registry &reg, sf::RenderWindow &window): _reg(reg), _window(window) {};
        ~DrawSystem() = default;
        void operator()() override {
            auto &sprite = _reg.getComponent<Sprite>();
            auto &position = _reg.getComponent<Position>();
            auto &size = _reg.getComponent<Size>();

            for (long unsigned int i = 0; i < sprite.size(); i++) {
                if (!sprite[i] || !position[i] || !size[i])
                    continue;

                auto &sprite_value = sprite[i].value();
                auto &pos_value = position[i].value();
                auto &size_value = size[i].value();

                sprite_value.sprite.setTextureRect(sprite_value.rect);
                sprite_value.sprite.setPosition(pos_value.x, pos_value.y);
                sprite_value.sprite.setScale(size_value.w, size_value.h);

                _window.draw(sprite_value.sprite);
            }
        };
    private:
        registry &_reg;
        sf::RenderWindow &_window;
};

#endif /* !SYSTEM_HPP_ */
