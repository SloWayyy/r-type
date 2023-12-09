#include "system.hpp"

void System::display_drawable()
{
    auto &draw = reg.getComponent<Drawable>();

    for (auto &i : draw) {
        if (i) {
            std::cout << "Drable entity: " << i.value().isDrawable << std::endl;
        }
    }
}
