/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** spawnedEnnemy
*/

#ifndef DESTROYENTITY_HPP_
    #define DESTROYENTITY_HPP_

    #include "../../ecs/event/IEvent.hpp"
    #include <cstdint>
    #include <iostream>

class destroyEntity : public IEvent {
    public:
        destroyEntity(uint32_t entity_id)  : entity_id(entity_id) {};
        ~destroyEntity() = default;
        void debug() override {
            std::cout << "destroyed entity_id -> " << entity_id  << std::endl;
        };
        uint32_t entity_id;
    protected:
    private:
};

#endif /* !DESTROYENTITY_HPP_ */
