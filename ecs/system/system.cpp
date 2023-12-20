#include "system.hpp"

void animeEntity(registry &reg, size_t incrementLeft, size_t maxWidth)
{
    auto &sprite = reg.getComponent<Sprite>();

    for (long unsigned int i = 0; i < sprite.size(); i++) {
        if (sprite[i]) {
            sprite[i].value().rect.left += incrementLeft;
            if (sprite[i].value().rect.left >= maxWidth)
                sprite[i].value().rect.left = 0;
        }
    }
}