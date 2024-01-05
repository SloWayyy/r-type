/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** keyPressed
*/

#ifndef KEYPRESSED_HPP_
#define KEYPRESSED_HPP_
#include "IEvent.hpp"
#include <cstdint>

class keyPressed : public IEvent {
    public:
        keyPressed(uint32_t key): _key(key) {};
        ~keyPressed() = default;
        void debug() override {
            std::cout << "keyPressed with " << _key << std::endl;
        };

        uint32_t _key;
    protected:
    private:
};

#endif /* !KEYPRESSED_HPP_ */
