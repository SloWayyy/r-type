/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** collision
*/

#ifndef COLLISION_HPP_
    #define COLLISION_HPP_

    #include "IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class collision : public IEvent {
    public:
        collision(uint32_t id1, uint32_t id2): _id1(id1), _id2(id2) {};
        ~collision() = default;
        void debug() override {
            std::cout << "collision with " << _id1 << " and " << _id2 << std::endl;
        };
        uint32_t _id1;
        uint32_t _id2;
    protected:
    private:
};

#endif /* !COLLISION_HPP_ */
