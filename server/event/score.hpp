/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** score
*/

#ifndef SCORE_HPP_
    #define SCORE_HPP_

    #include "../../ecs/event/IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class score : public IEvent {
    public:
        score(uint32_t entity_id)  : entity_id(entity_id) {};
        ~score() = default;
        void debug() override {
            std::cout << "Score: entity_id -> " << entity_id  << std::endl;
        };
        uint32_t entity_id;
    protected:
    private:
};

#endif /* !SCORE_HPP_ */
