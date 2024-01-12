/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** health
*/

#ifndef HEALTH_HPP_
    #define HEALTH_HPP_

    #include "../../ecs/event/IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class health : public IEvent {
    public:
        health(uint32_t entity_id)  : entity_id(entity_id) {};
        ~health() = default;
        void debug() override {
            std::cout << "health: entity_id -> " << entity_id  << std::endl;
        };
        uint32_t entity_id;
    protected:
    private:
};

#endif /* !HEALTH_HPP_ */
