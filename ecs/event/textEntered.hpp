/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** textEntered
*/

#ifndef TEXTENTERED_HPP_
    #define TEXTENTERED_HPP_

    #include "IEvent.hpp"
    #include <cstdint>

class textEntered : public IEvent {
    public:
        textEntered(uint32_t key): _key(key) {};
        ~textEntered() = default;
        void debug() override {
            std::cout << "textEntered with " << _key << std::endl;
        };
        uint32_t _key;
    protected:
    private:
};

#endif /* !TEXTENTERED_HPP_ */
