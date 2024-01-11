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
        shoot(uint32_t entity_id) : entity_id(entity_id) {};
        ~shoot() = default;
        void debug() override {
            std::cout << "event shoot: entity id -> " << entity_id  << std::endl;
        };
        uint32_t entity_id;
    protected:
    private:
};

#endif /* !SHOOT_HPP_ */
