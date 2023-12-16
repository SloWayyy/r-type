/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** entity
*/

#include "gameEntity.hpp"

sf::Sprite GameEntity::createGameEntity(std::string path)
{
    sf::Texture texture;
    sf::Sprite sprite;

    if (!texture.loadFromFile(path))
        std::cout << "Error loading texture" << std::endl;
    sprite.setTexture(texture);
    return sprite;
}
