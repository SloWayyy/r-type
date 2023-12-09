#include "system.hpp"

void System::drawEntity(sf::RenderWindow &window)
{
    auto &sprite = reg.getComponent<Sprite>();
    auto &position = reg.getComponent<Position>();
    auto &size = reg.getComponent<Size>();

    for (int i = 0; i < sprite.size(); i++) {
        if (sprite[i] && position[i] && size[i]) {
            sprite[i].value().sprite.setPosition(position[i].value().x, position[i].value().y);
            sprite[i].value().sprite.setScale(size[i].value().w, size[i].value().h);
            window.draw(sprite[i].value().sprite);
        }
    }
}
