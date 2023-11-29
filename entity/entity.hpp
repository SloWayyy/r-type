/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** entity
*/

#ifndef ENTITY_HPP
    #define ENTITY_HPP

    #include <iostream>

    class Entity {
        public:
            explicit Entity(size_t number) : entity(number) {};
            ~Entity() = default;
            operator size_t() const { return entity; };
        private:
            size_t entity;
    };

#endif /* !hpp */
