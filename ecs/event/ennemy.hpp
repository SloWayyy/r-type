/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Ennemy
*/

#ifndef ENNEMY_HPP_
#define ENNEMY_HPP_

    #include "IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class ennemy : public IEvent {
    public:
        ennemy(uint32_t player_x, uint32_t player_y): _player_x(player_x), _player_y(player_y) {};
        ~ennemy() = default;
        void debug() override {
            std::cout << "ennemy with " << _player_x << " and " << _player_y << std::endl;
        };
        uint32_t _player_x;
        uint32_t _player_y;
    protected:
    private:
};

#endif /* !ENNEMY_HPP_ */
