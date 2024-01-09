/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** bullet
*/

#ifndef BULLET_HPP_
    #define BULLET_HPP_

    #include "../../ecs/event/IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class bullet : public IEvent {
    public:
        bullet(uint32_t entity_id) : entity_id(entity_id) {};
        ~bullet() = default;
        void debug() override {
            std::cout << "bullet: entity id -> " << entity_id  << std::endl;
        };
        uint32_t entity_id;
    protected:
    private:
};

#endif /* !BULLET_HPP_ */
