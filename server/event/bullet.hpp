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
        bullet(uint32_t shoot_id, uint32_t bullet_id)  : shoot_id(shoot_id), bullet_id(bullet_id) {};
        ~bullet() = default;
        void debug() override {
            std::cout << "bullet: shoot_id -> " << shoot_id  << std::endl;
            std::cout << "bullet: bullet_id -> " << bullet_id  << std::endl;
        };
        uint32_t shoot_id;
        uint32_t bullet_id;
    protected:
    private:
};

#endif /* !BULLET_HPP_ */
