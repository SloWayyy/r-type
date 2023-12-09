#include "../component/component.cpp"
#include "../registry/registry.hpp"

void Move(registry &reg)
{
    auto &pos = reg.getComponent<Position>();
    auto &vel = reg.getComponent<Velocity>();
    auto &draw = reg.getComponent<Drawable>();
    auto &ctrl = reg.getComponent<Controller>();

    // for (size_t i = 0; i < draw.pos; i++) {
    //     if (draw[i] && ctrl[i]) {
    //         pos[i].x += vel[i].v * vel[i].t + vel[i].a * vel[i].t * vel[i].t / 2;
    //         pos[i].y += vel[i].v * vel[i].t + vel[i].a * vel[i].t * vel[i].t / 2;
    //         vel[i].t++;
    //     }
    // }
};
