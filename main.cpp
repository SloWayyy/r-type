#include "entity/entity.hpp"
#include "registry/registry.hpp"
#include "registry/sparse_array/sparse_array.hpp"
// #include "component/component.cpp"
#include "system/move.cpp"

int main()
{
    registry r;
    r.addComponent<Position>();
    r.addComponent<Velocity>();
    r.addComponent<Drawable>();
    r.addComponent<Controller>();

    int player_id = r.addEntity();
    int player_id2 = r.addEntity();

    auto &pos = r.getComponent<Position>();

    pos[player_id2] = Position(10, 12);
    std::cout << pos[player_id2] << std::endl;
}
