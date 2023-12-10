/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** game
*/

#ifndef GAME_HPP_
    #define GAME_HPP_

    #include <SFML/Audio.hpp>
    #include <SFML/Graphics.hpp>
    #include <functional>
    #include "gameEntity.hpp"
    #include "../ecs/registry/registry.hpp"
    #include "../ecs/system/system.hpp"

    class Game {
        public:
           Game(GameEntity &gameEntity, registry &reg, System &sys) : gameEntity(gameEntity), reg(reg), sys(sys) {};
           ~Game() = default;
           void run();
           void init();
        private:
            GameEntity &gameEntity;
            registry &reg;
            System &sys;
     };

#endif /* !GAME_HPP_ */
