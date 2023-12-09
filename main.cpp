#include "ecs/registry/registry.hpp"
#include "ecs/system/system.hpp"
#include "game/game.hpp"
#include "game/gameEntity.hpp"

int main()
{
    GameEntity gameEntity;
    registry reg;
    System system(reg);

    Game game(gameEntity, reg, system);
    game.run();
}
