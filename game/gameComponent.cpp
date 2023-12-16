#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

struct Sprite {
    Sprite(std::string path, sf::IntRect rect)
        : rect(rect)
    {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(rect);
        std::cout << "Sprite created with path: " << path << std::endl;
    }

    Sprite(const Sprite &sprite_)
    {
        texture = sprite_.texture;
        rect = sprite_.rect;
        sprite = sprite_.sprite;
        sprite.setTexture(texture);
    }

    public:
        sf::IntRect rect;
        sf::Texture texture;
        sf::Sprite sprite;
};
