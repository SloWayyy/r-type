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
    void animeEntity(size_t incrementLeft, size_t maxWidth, registry &reg);
    void drawEntity(sf::RenderWindow &window, registry &reg);

#endif /* !SYSTEM_HPP_ */
