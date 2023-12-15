#include "ecs/registry/registry.hpp"
#include "ecs/system/system.hpp"
#include "game/game.hpp"
#include "game/gameEntity.hpp"

int main()
{
    GameEntity gameEntity;
    registry reg;

    Game game(gameEntity, reg);
    game.run();
}
