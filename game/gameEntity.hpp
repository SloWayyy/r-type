/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** entity
*/

#ifndef GAME_ENTITY_HPP_
    #define GAME_ENTITY_HPP_

    #include <iostream>
    #include <SFML/Audio.hpp>
    #include <SFML/Graphics.hpp>

    class GameEntity {
        public:
            GameEntity() = default;
            ~GameEntity() = default;
            sf::Sprite createGameEntity(std::string path);
            void setShip(size_t ship) {_ship = ship;};
            size_t &getShip() {return _ship;};
        private:
            size_t _ship;
    };
#endif /* !gameEntity_HPP_ */
