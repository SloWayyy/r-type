/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** updateGame
*/

#ifndef UPDATEGAME_HPP_
    #define UPDATEGAME_HPP_

#include "../../ecs/registry/registry.hpp"
#include <any>
#include <vector>

class UpdateGame {
    public:
        UpdateGame(registry &reg);
        ~UpdateGame() = default;

        std::vector<std::vector<uint8_t>> updateEntity(uint32_t entity_id);
    private:
        registry &_reg;
};

#endif /* !UPDATEGAME_HPP_ */
