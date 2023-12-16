/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../registry/registry.hpp"

    void moveEntity(registry &reg);
    void animeEntity(registry &reg, size_t incrementLeft, size_t maxWidth);
    void drawEntity(registry &reg, sf::RenderWindow &window);

#endif /* !SYSTEM_HPP_ */
