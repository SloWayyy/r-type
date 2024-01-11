/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** spawnedEnnemy
*/

#ifndef SPAWNEDENNEMY_HPP_
    #define SPAWNEDENNEMY_HPP_

    #include "../../ecs/event/IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class spawnedEnnemy : public IEvent {
    public:
        spawnedEnnemy(uint32_t ennemy_id)  : ennemy_id(ennemy_id) {};
        ~spawnedEnnemy() = default;
        void debug() override {
            std::cout << "ennemy: ennemy_id -> " << ennemy_id  << std::endl;
        };
        uint32_t ennemy_id;
    protected:
    private:
};

#endif /* !SPAWNEDENNEMY_HPP_ */
