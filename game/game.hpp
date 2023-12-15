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
           Game(GameEntity &gameEntity, registry &reg) : gameEntity(gameEntity), reg(reg) {};
           ~Game() = default;
           void run();
           void init();
        private:
            registry &reg;
            GameEntity &gameEntity;
     };

#endif /* !GAME_HPP_ */
