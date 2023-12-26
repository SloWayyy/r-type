/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** updateGame
*/

#ifndef UPDATEGAME_HPP_
    #define UPDATEGAME_HPP_

#include "../../ecs/registry/registry.hpp"

class UpdateGame {
    public:
        UpdateGame(registry &reg);
        ~UpdateGame() = default;

        std::pair<size_t, Position> updateEntity();
    private:
        registry &_reg;
};

#endif /* !UPDATEGAME_HPP_ */
