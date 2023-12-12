#include "registry/registry.hpp"
#include "system/system.hpp"
#include <iostream>

int main()
{
    registry r;
    System s(r);
    r.addAllComponents();

    int first_entity = r.addEntity();
    int second_entity = r.addEntity();

    Sparse_array<Drawable> &obj = r.getComponent<Drawable>();
    obj[first_entity] = Drawable(true);
    s.display_drawable();
}
