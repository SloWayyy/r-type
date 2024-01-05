/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** shoot
*/

#ifndef SHOOT_HPP_
    #define SHOOT_HPP_

    #include "IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class shoot : public IEvent {
    public:
        shoot(uint32_t player_x, uint32_t player_y): _player_x(player_x), _player_y(player_y) {};
        ~shoot() = default;
        void debug() override {
            std::cout << "shoot with " << _player_x << " and " << _player_y << std::endl;
        };
        uint32_t _player_x;
        uint32_t _player_y;
    protected:
    private:
};

#endif /* !SHOOT_HPP_ */
