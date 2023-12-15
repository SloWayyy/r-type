#include "system.hpp"

void animeEntity(size_t incrementLeft, size_t maxWidth, registry &reg)
{
    auto &sprite = reg.getComponent<Sprite>();

    for (int i = 0; i < sprite.size(); i++) {
        if (sprite[i]) {
            sprite[i].value().rect.left += incrementLeft;
            if (sprite[i].value().rect.left >= maxWidth)
                sprite[i].value().rect.left = 0;
        }
    }
}

void moveEntity(registry &reg) {
    auto &position = reg.getComponent<Position>();
    auto &velocity = reg.getComponent<Velocity>();
    for (int i = 0; i < position.size(); i++) {
        if (position[i] && velocity[i]) {
            position[i].value().x += velocity[i].value().x_speed;
            position[i].value().y += velocity[i].value().y_speed;
        }
    }
}

void drawEntity(sf::RenderWindow &window, registry &reg)
{
    auto &sprite = reg.getComponent<Sprite>();
    auto &position = reg.getComponent<Position>();
    auto &size = reg.getComponent<Size>();

    for (int i = 0; i < sprite.size(); i++) {
        if (!sprite[i] || !position[i] || !size[i])
            continue;

        auto &sprite_value = sprite[i].value();
        auto &pos_value = position[i].value();
        auto &size_value = size[i].value();

        // sprite_value.sprite.setTexture(sprite_value.texture);
        sprite_value.sprite.setTextureRect(sprite_value.rect);
        sprite_value.sprite.setPosition(pos_value.x, pos_value.y);
        sprite_value.sprite.setScale(size_value.w, size_value.h);

        window.draw(sprite_value.sprite);
    }
}
